#pragma once

#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/base/bit_alg.h>
#include<ccdk/mpl/type_traits/has_constructor.h>
#include<ccdk/mpl/util/move.h>
#include<ccdk/mpl/util/fill.h>
#include<ccdk/mpl/util/copy.h>
#include<ccdk/mpl/util/construct.h>
#include<ccdk/mpl/util/swap.h>
#include<ccdk/mpl/function/operator.h>
#include<ccdk/mpl/iterator/round_index_iterator.h>
#include<ccdk/mpl/iterator/reverse_iterator.h>
#include<ccdk/mpl/iterator/algorithm/distance.h>
#include<ccdk/mpl/units/ratio.h>
#include<ccdk/memory/allocator/simple_new_allocator.h>
#include<ccdk/memory/adapter/array_allocate_adapter.h>
#include<ccdk/container/container_mudule.h>

ccdk_namespace_ct_start

using namespace ccdk::mpl;

template<
	typename T,
	typename Size = uint32,
	typename Alloc = mem::simple_new_allocator<T,Size>
>
class circular_buffer : protected Alloc
{
public:
	using this_type = circular_buffer;
	using inc_ratio = units::ratio<2, 1>; //always allocate 2^n memory so that we can do fast mod
	static constexpr uint32 KLeastElements = 16;  

	/* common */
	using value_type      = T;
	using pointer         = T *;
	using const_pointer   = T const*;
	using reference       = T & ;
	using const_reference = T const&;
	using size_type       = Size;
	using difference_type = ptr::diff_t;
	using allocator_type  = mem::array_allocate_adapter<KLeastElements, inc_ratio, Alloc>;

	/* iterator */
	using iterator               = it::iterator<round_index_category, T, Size>;
	using const_iterator         = it::iterator<round_index_category, const T, Size>;
	using reverse_iterator       = it::reverse_iterator<iterator>;
	using const_reverse_iterator = it::reverse_iterator<const_iterator>;

	template<typename T2, typename Size2, typename Alloc2>
	friend class circular_buffer;

private: 
	pointer   content;  // memory
	size_type ibegin;    // ibegin element index
	size_type iend;      // iend  element index, remain 1 element when last+2 == first
	size_type cap;      // cap is awalys 2^n, but store 2^n-1 to caculate fast
	
#define CHECK_POW2() ccdk_assert( !(difference_type(cap) & (difference_type(cap)+1)) )
	// help index fn
	CCDK_FORCEINLINE size_type prev(size_type idx) const noexcept { return (idx + cap) & cap; }
	CCDK_FORCEINLINE size_type next(size_type idx) const noexcept { return (idx + 1) & cap; }
	CCDK_FORCEINLINE size_type round(size_type idx) const noexcept { return (ibegin + idx) & cap; }
	CCDK_FORCEINLINE size_type rround(size_type idx) const noexcept { return (iend + cap - idx) & cap; }
	CCDK_FORCEINLINE size_type allocate_capcity() const noexcept { return cap + 1; }
	CCDK_FORCEINLINE bool full() const noexcept { return next(iend) == ibegin; }
	CCDK_FORCEINLINE bool out_of_capcity(size_type n) const noexcept { return n >= allocate_capcity(); }
public:

//////////////////////////////////////////////////////////////////////////
//// ctor / de-ctor

	// de-ctor
	CCDK_FORCEINLINE ~circular_buffer() noexcept { destroy(); }

	// default ctor
	CCDK_FORCEINLINE constexpr circular_buffer() noexcept
		: content{ nullptr }, ibegin{ 0 }, iend{ 0 }, cap(-1) {}
	CCDK_FORCEINLINE constexpr circular_buffer(ptr::nullptr_t) noexcept
		: content{ nullptr }, ibegin{ 0 }, iend{ 0 }, cap(-1) {}

	//fill-n ctor
	CCDK_FORCEINLINE constexpr circular_buffer(size_type n, T const& t = T()) noexcept {
		ccdk_assert(n > 0);
		allocate_fill(n, t);
	}
	
	//copy-n ctor
	template<
		typename InputIt,
		typename = check_t< is_iterator<InputIt>> >
	CCDK_FORCEINLINE circular_buffer(InputIt ibegin, size_type n) noexcept {
		ccdk_assert(n > 0);
		allocate_copy( ibegin, n);
	}

	//copy range
	template<
		typename InputIt,
		typename = check_t< is_iterator<InputIt>> >
	CCDK_FORCEINLINE circular_buffer(InputIt ibegin, InputIt iend)
		: circular_buffer{ ibegin, it::distance(ibegin, iend) } {}

	//array ctor
	template<uint32 N>
	CCDK_FORCEINLINE circular_buffer(T const(&arr)[N])
		: circular_buffer{ arr,N } {}

	//copy ctor
	CCDK_FORCEINLINE circular_buffer(circular_buffer const& other) {
		allocate(other.size());
		round_copy(other.content, other.ibegin,
			other.iend, other.allocate_capcity(), other.size());
	}

	//template copy ctor
	template<typename Size2, typename Alloc2 >
	CCDK_FORCEINLINE circular_buffer( circular_buffer<T, Size2, Alloc2> const& other) {
		allocate(other.size());
		round_copy(other.content, other.ibegin,
			other.iend, other.allocate_capcity(), other.size());
	}

	// move ctor
	CCDK_FORCEINLINE circular_buffer(circular_buffer && other) noexcept
		:content{ other.content }, ibegin{ other.ibegin }, iend{ other.iend },
		cap{ other.cap }{
		other.rvalue_reset();
	}

	// template move
	template<typename Size2, typename Alloc2>
	CCDK_FORCEINLINE circular_buffer(circular_buffer<T, Size2, Alloc2> && other) noexcept
		:content{ other.content }, ibegin{ other.ibegin }, iend{ other.iend },
		cap{ other.cap }{
		other.rvalue_reset();
	}

////////////////////////////////////////////////////////////////////////////////////////
//// swap / assign

	//swap
	CCDK_FORCEINLINE void swap(circular_buffer& other) noexcept {
		util::swap(content, other.content);
		util::swap(ibegin, other.ibegin);
		util::swap(iend, other.iend);
		util::swap(cap, other.cap);
	}

	// copy assign, avoid self assign
	CCDK_FORCEINLINE this_type& operator=(circular_buffer const& other) {
		ccdk_if_not_this(other) { 
			if (out_of_capcity(other.size())) reallocate(other.size());
			else destruct_content();
			round_copy(other.content, other.ibegin, 
				other.iend, other.allocate_capcity(), other.size());
		}
		return *this;
	}

	//template move assign
	template<typename Size2, typename Alloc2>
	CCDK_FORCEINLINE this_type& operator=(circular_buffer<T, Size2, Alloc2> const& other) {
		if (out_of_capcity(other.size())) reallocate(other.size());
		else destruct_content();
		round_copy(other.content, other.ibegin, 
			other.iend, other.allocate_capcity(), other.size());
		return *this;
	}

	//move assgin
	CCDK_FORCEINLINE this_type& operator=(circular_buffer && other) {
		ccdk_if_not_this(other) { 
			destroy();
			rvalue_set(other.content, other.ibegin, other.iend, other.cap);
			other.rvalue_reset();
		}
		return *this;
	}

	//template move assign
	template<typename Size2>
	CCDK_FORCEINLINE this_type& operator=(circular_buffer<T, Size2> && other) {
		destroy();
		rvalue_set(other.content, other.ibegin,  other.iend, other.cap);
		other.rvalue_reset();
		return *this;
	}

	//initialize list assign
	CCDK_FORCEINLINE this_type& operator=(std::initializer_list<T> const& lst) {
		return assign(lst.begin(), lst.size());
	}

	//fill with t
	CCDK_FORCEINLINE this_type& assign(size_type n, T const& t = T()) {
		return fill_or_reallocate_fill(t, n);
	}

	//assign copy-n
	template<typename InputIt,
		typename = check_t< is_iterator<InputIt>> >
	CCDK_FORCEINLINE this_type& assign(InputIt beginIt, ptr::size_t n) {
		return copy_or_reallocate_copy(beginIt, n);
	}

	//assign copy range
	template<
		typename InputIt,
		typename = check_t< is_iterator<InputIt>> >
	CCDK_FORCEINLINE this_type& assign(InputIt beginIt, InputIt endIt) {
		return assign(beginIt, it::distance(beginIt, endIt));
	}

	// access elements
	CCDK_FORCEINLINE reference operator[](size_type index) noexcept { return at(index); }
	CCDK_FORCEINLINE const_reference operator[](size_type index) const noexcept { return at(index); }
	CCDK_FORCEINLINE reference at(size_type index) noexcept { ccdk_assert(index < size()); return content[round(index)]; }
	CCDK_FORCEINLINE const_reference at(size_type index) const noexcept { ccdk_assert(index < size()); return content[round(index)]; }
	CCDK_FORCEINLINE reference front() noexcept { ccdk_assert(!empty()); return content[ibegin]; }
	CCDK_FORCEINLINE const_reference front() const noexcept { ccdk_assert(!empty()); return content[ibegin]; }
	CCDK_FORCEINLINE reference back() noexcept { ccdk_assert(!empty());  return content[prev(iend)]; }
	CCDK_FORCEINLINE const_reference back() const noexcept { ccdk_assert(!empty());  return content[prev(iend)]; }
	CCDK_FORCEINLINE reference back(size_type index) noexcept { ccdk_assert(!empty());  return content[rround(index)]; }
	CCDK_FORCEINLINE const_reference back(size_type index) const noexcept { ccdk_assert(!empty());  return content[rround(index)]; }

	/* attribute */
	CCDK_FORCEINLINE size_type size() const noexcept { CHECK_POW2(); return (iend + allocate_capcity() - ibegin) & cap; }
	CCDK_FORCEINLINE size_type capcity() const noexcept { return content ? cap:0; }
	CCDK_FORCEINLINE size_type max_size() const noexcept { size_type(-1); }
	CCDK_FORCEINLINE bool empty() const noexcept { return ibegin==iend; }
	

	/* iterator */
	CCDK_FORCEINLINE iterator begin() noexcept { return {content, cap, ibegin }; }
	CCDK_FORCEINLINE iterator end() noexcept { return { content, cap, iend }; }
	CCDK_FORCEINLINE const_iterator begin() const noexcept { return { content, cap, ibegin }; }
	CCDK_FORCEINLINE const_iterator end() const noexcept { return { content, cap, iend }; }
	CCDK_FORCEINLINE const_iterator cbegin() const noexcept { return { content, cap, ibegin }; }
	CCDK_FORCEINLINE const_iterator cend() const noexcept { return { content, cap, iend }; }
	CCDK_FORCEINLINE reverse_iterator rbegin() noexcept { return { content, cap, prev(iend) }; }
	CCDK_FORCEINLINE reverse_iterator rend() noexcept { return { content, cap,  prev(ibegin) }; }
	CCDK_FORCEINLINE const_reverse_iterator rbegin() const noexcept { return { content, cap, prev(iend) }; }
	CCDK_FORCEINLINE const_reverse_iterator rend() const noexcept { return { content, cap,  prev(ibegin) }; }
	CCDK_FORCEINLINE const_reverse_iterator crbegin() const noexcept { return { content, cap, prev(iend) }; }
	CCDK_FORCEINLINE const_reverse_iterator crend() const noexcept { return { content, cap, prev(ibegin) }; }

	// pop back
	CCDK_FORCEINLINE this_type& pop_back()  {
		if (!empty()) {
			iend = prev(iend);
			util::destruct<T>(content + iend);
		}
		return *this;
	}
	
	// pop front
	CCDK_FORCEINLINE this_type& pop_front() {
		if (!empty()) {
			util::destruct<T>(content + ibegin );
			ibegin = next(ibegin);
		}
		return *this;
	}

	// emplace at back
	template<typename ... Args, typename  = check_t<has_constructor<T,Args...>> >
	CCDK_FORCEINLINE this_type& emplace_back(Args&&... args){
		//null or full
		if (empty() || full()) { allocate_or_reallocate_move(); }
		util::construct<T>(content + iend, util::forward<Args>(args)...);
		iend = next(iend);
		return *this;
	}

	//pusb back
	CCDK_FORCEINLINE this_type& push_back(T const& t) { return emplace_back(t);}
	CCDK_FORCEINLINE this_type& push_back(T&& t) { return emplace_back(util::move(t));}

	// emplace at front
	template<typename ... Args, typename = check_t<has_constructor<T, Args...>> >
	CCDK_FORCEINLINE this_type& emplace_front(Args&&... args) {
		// null or full
		if ( empty() || full() ) { allocate_or_reallocate_move(); }
		ibegin = prev(ibegin);
		util::construct<T>(content + ibegin, util::forward<Args>(args)...);
		return *this;
	}

	// push front
	CCDK_FORCEINLINE this_type& push_front(T const& t) {return emplace_front(t);}
	CCDK_FORCEINLINE this_type& push_front(T&& t) {return emplace_front(util::move(t));}
	
	// clear
	CCDK_FORCEINLINE this_type& clear() { destruct_content(); return *this; }

//////////////////////////////////////////////////////////////////////////////
//// transform / loop over

	template<typename FN>
	CCDK_FORCEINLINE void foreach(FN Fn) const noexcept{
		if (!empty()) {
			if (iend < ibegin) {
				for (size_type i = ibegin; i <= cap; ++i) {
					Fn(content[i]);
				}
				for (size_type i = 0; i < iend; ++i) {
					Fn(content[i]);
				}
			}
			else {
				for (size_type i = ibegin; i < iend; ++i) {
					Fn(content[i]);
				}
			}
		}
	}

private:

	CCDK_FORCEINLINE void rvalue_reset() noexcept {
		content = nullptr;
		ibegin = iend = 0;
		cap = size_type(-1);
	}

	CCDK_FORCEINLINE void rvalue_set(pointer inContent, size_type inBegin, 
		size_type inEnd, size_type inCap) noexcept {
		content = inContent;
		ibegin = inBegin;
		iend = inEnd;
		cap = inCap;
	}

	/* if len > 0 , destruct objects */
	CCDK_FORCEINLINE void destruct_content() noexcept {
		if (!empty()) {
			if (iend<ibegin) {                                        /* tail is round to head */
				util::destruct_n(content + ibegin, cap + 1 - ibegin);  /* destruct tail objects */
				util::destruct_n(content, iend);                      /* destruct head objects */
			}
			else {
				util::destruct_n(content + ibegin, size());
			}
			ibegin = iend = 0;
		}
	}

	//allocate n or more memory
	void allocate(size_type n) {
		size_type actual_size = ubit_alg<size_type>::clp2(n+1);
		content = allocator_type::allocate(*this, actual_size, cap);
		--cap;
		CHECK_POW2();
	}

	//reallocate with n 
	void reallocate(size_type n) {
		destroy();
		allocate(n);
	}

	//recycle memory
	CCDK_FORCEINLINE void deallocate() noexcept {
		if (content) {
			allocator_type::deallocate(*this, content, allocate_capcity());
			cap = -1;
		}
	}

	//destruct content and release memory
	CCDK_FORCEINLINE void destroy() noexcept {
		destruct_content();
		deallocate();
	}

	//fill content[0,n)
	CCDK_FORCEINLINE void allocate_fill(size_type n, T const& v) {
		allocate(n);
		util::construct_fill_n(content, v, n);
		iend = n;
	}

	//copy [ibegin, ibegin+n) to content[0, n)
	template<typename InputIt>
	CCDK_FORCEINLINE void allocate_copy(InputIt ibegin, size_type n) {
		allocate(n);
		util::construct_copy_n(content, ibegin, n);
		iend = n;
	}

	// self clone, which is fast than iterator copy
	void round_copy(pointer src, size_type inBegin, 
		size_type inEnd, size_type inCap, size_type size) {
		if (inCap == 0) return;
		//tail part round to front
		if (inEnd < inBegin) {
			//copy src [inFirst, inCap) to dest[0, inCap - inFirst)
			size_type head_size = inCap - inBegin;
			util::construct_copy_n(content, src + inBegin, head_size);
			//copy src [0, inLast+1) to content [ inCap - inFirst, inCap)
			util::construct_copy_n(content + head_size, src, inEnd);
		}
		else {
			util::construct_copy_n(content, src + inBegin, size);
		}
		iend = size;
	}

	// move content to new allocated memory
	void allocate_or_reallocate_move() {

		size_type new_cap;
		pointer memory = allocator_type::allocate_inc(*this, cap+1, &new_cap);
		if (!empty()) {
			size_type original_size = size();
			if (iend < ibegin) {
				size_type head_size = allocate_capcity() - ibegin;
				util::construct_move_n(memory, content + ibegin, head_size);
				util::construct_move_n(memory + head_size, content, iend);
				ccdk_assert(head_size + iend == original_size);
			}
			else {
				util::construct_move_n(memory, content + ibegin, original_size);
			}
			destroy();
			iend = original_size;
		}
		content = memory;
		cap = new_cap - 1;
	}

	// if big enough to hold n element, just destruct content than copy range to it
	// else reallocate and copy
	template<typename InputIt>
	CCDK_FORCEINLINE this_type& copy_or_reallocate_copy(InputIt beginIt, size_type n) {
		if (out_of_capcity(n)) reallocate(n);
		else destruct_content();
		util::construct_copy_n(content, beginIt, n);
		iend = n;
		return *this;
	}

	// if big enough to hold n element, just destruct content than fill it with T
	// else reallocate and fill
	CCDK_FORCEINLINE this_type& fill_or_reallocate_fill(T const& t, size_type n) {
		if (out_of_capcity(n)) reallocate(n); 
		else destruct_content();
		util::construct_fill_n(content, t, n);
		iend = n;
		return *this;
	}

public:

	CCDK_FORCEINLINE void debug_all(const char* title= "") const noexcept {
		DebugValueItBegin(title);
		foreach([](T const& t) {
			DebugValueIt(t);
		});
		DebugValueItEnd();
	}

};

ccdk_namespace_ct_end