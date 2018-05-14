#pragma once

#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/base/bit_alg.h>
#include<ccdk/mpl/type_traits/has_constructor.h>
#include<ccdk/mpl/util/move.h>
#include<ccdk/mpl/util/fill.h>
#include<ccdk/mpl/util/copy.h>
#include<ccdk/mpl/util/construct.h>
#include<ccdk/mpl/util/swap.h>
#include<ccdk/mpl/util/forward.h>
#include<ccdk/mpl/fusion/local_arr.h>
#include<ccdk/mpl/function/operator.h>
#include<ccdk/mpl/iterator/round_index_iterator.h>
#include<ccdk/mpl/iterator/reverse_iterator.h>
#include<ccdk/mpl/iterator/algorithm/advance.h>
#include<ccdk/mpl/iterator/algorithm/distance.h>
#include<ccdk/mpl/units/ratio.h>
#include<ccdk/memory/allocator/simple_new_allocator.h>
#include<ccdk/memory/allocator_traits.h>
#include<ccdk/container/container_mudule.h>

ccdk_namespace_ct_start

using namespace ccdk::mpl;

template< typename T,uint32 N >
class fix_circular_buffer
{
public:
	using this_type = fix_circular_buffer;

    //common
	using value_type      = T;
	using pointer         = T*;
	using const_pointer   = T const*;
	using reference       = T&;
	using const_reference = T const&;
	using size_type       = uint32;
	using difference_type = ptr::diff_t;

	// iterator
	using iterator               = it::iterator<round_index_category, T, size_type>;
	using const_iterator         = it::iterator<round_index_category, const T, size_type>;
	using reverse_iterator       = it::reverse_iterator<iterator>;
	using const_reverse_iterator = it::reverse_iterator<const_iterator>;

	//actual size is ceil to 2^N
	constexpr static uint32 kCeilPow2 = bit_alg<uint32>::clp2(N);
	constexpr static uint32 kRoundVal = kCeilPow2 - 1;
private:

	fs::local_arr<T, kCeilPow2> content;
	size_type                   ibegin;
	size_type                   len;

	//help index
	CCDK_FORCEINLINE size_type iprev(size_type n) const noexcept { return (n + kRoundVal)&kRoundVal; }
	CCDK_FORCEINLINE size_type inext(size_type n) const noexcept { return (n + 1)&kRoundVal; }
	CCDK_FORCEINLINE size_type iround(size_type n) const noexcept { return (ibegin + n)&kRoundVal; }
	CCDK_FORCEINLINE size_type irround(size_type n) const noexcept { ccdk_assert(n < len); return iround(len - n-1); }
	CCDK_FORCEINLINE size_type iend() const noexcept { return iround(len); }
	CCDK_FORCEINLINE size_type iback() const noexcept { ccdk_assert(len > 0); return iround(len - 1); }

public:

////////////////////////////////////////////////////////////////////////////////
//// de-ctor / ctor

	// de-ctor
	CCDK_FORCEINLINE ~fix_circular_buffer() noexcept {
		if (len) {
			destruct_content();
		}
	}

	//default and nullptr ctor
	CCDK_FORCEINLINE constexpr fix_circular_buffer()
		: content{}, ibegin{ 0 }, len{ 0 } {}
	CCDK_FORCEINLINE constexpr fix_circular_buffer(ptr::nullptr_t) noexcept
		: content{}, ibegin{ 0 }, len{ 0 } {}

	//fill-n ctor
	template<
		typename... Args,
		typename = check_t< has_constructor<T,Args...> > >
	CCDK_FORCEINLINE fix_circular_buffer(size_type n, Args&&... args)
		noexcept(has_nothrow_constructor_v<T, Args...>)
		: ibegin{ 0 }, len{ fn::min(N,n) }, content{ } {
		ccdk_assert(len > 0 && len <= N);
		content.construct(0, len, util::forward<Args>(args)... );
	}

	// copy-n ctor
	template<
		typename InputIt,
		typename = check_t< is_iterator<InputIt>> >
	CCDK_FORCEINLINE fix_circular_buffer(InputIt beginIt, size_type n) 
		: ibegin{0}, len{fn::min(n,N)}, content{} {
		ccdk_assert(len > 0 && len <= N);
		content.construct(0, len, beginIt);
		
	}

	// copy range ctor
	template<
		typename InputIt,
		typename = check_t< is_iterator<InputIt>> >
	CCDK_FORCEINLINE fix_circular_buffer(InputIt beginIt, InputIt endIt)
		: fix_circular_buffer(beginIt, it::distance(beginIt, endIt)) {}

	template<uint32 S>
	CCDK_FORCEINLINE fix_circular_buffer(T const (&arr)[S])
		: fix_circular_buffer(arr, S) {}

	// copy ctor
	CCDK_FORCEINLINE fix_circular_buffer(fix_circular_buffer const& other) 
		: fix_circular_buffer(other.begin(), other.size()) {}

	// template copy
	template<uint32 N2>
	CCDK_FORCEINLINE fix_circular_buffer(fix_circular_buffer<T, N2> const& other)
		: fix_circular_buffer(other.begin(), other.size()) {}

	// move ctor
	CCDK_FORCEINLINE fix_circular_buffer(fix_circular_buffer && other) noexcept
		: ibegin{ other.ibegin },len {other.len } {
		util::construct_move_n(content.address(), other.begin(), other.size());
		other.rvalue_reset();
	}

	// template move ctor
	template<uint32 N2>
	CCDK_FORCEINLINE fix_circular_buffer(fix_circular_buffer<T, N2> && other) noexcept
		: ibegin{ other.ibegin }, len{ other.len } {
		util::construct_move_n(content.address(), other.begin(), other.size());
		other.rvalue_reset();
	}

	// swap
	CCDK_FORCEINLINE void swap(fix_circular_buffer& other) noexcept {
		//swap the actual occupied part
		content.swap_n<N>(other.content);
		util::swap(ibegin, other.ibegin);
		util::swap(len, other.len);
	}

	// copy assign
	CCDK_FORCEINLINE this_type& operator=(fix_circular_buffer const& other) {
		ccdk_if_not_this(other) { 
			destruct_content();
			util::construct_copy_n(content.address(),
				other.begin(), other.size()); 
			len = other.size();
		}
		return *this;
	}

	// template copy assign
	template<uint32 N2>
	CCDK_FORCEINLINE this_type& operator=(fix_circular_buffer<T, N2> const& other) {
		destruct_content();
		util::construct_copy_n(content.address(),
			 other.ibegin(), fn::min(N, other.size()) );
		len = other.size();
		return *this;
	}

	// move assign
	CCDK_FORCEINLINE this_type& operator=(fix_circular_buffer && other) {
		ccdk_if_not_this(other) {
			destruct_content();
			size_type n = fn::min(N, other.size());
			util::construct_move_n(content.address(),
				other.begin(), other.size());
			len = n;
			other.rvalue_reset();
		}
		return *this;
	}

	// template move assign
	template<uint32 N2>
	CCDK_FORCEINLINE this_type& operator=(fix_circular_buffer<T, N2> && other) {
		destruct_content();
		size_type n = fn::min(N, other.size());
		util::construct_move_n(content.address(),
			other.ibegin(), n);
		len = n;
		other.rvalue_reset();
		return *this;
	}

	// initialze list
	CCDK_FORCEINLINE this_type& operator=(std::initializer_list<T> const& lst) {
		return assign(lst.begin(), lst.size());
	}

	// fill-n assign
	template<typename... Args>
	CCDK_FORCEINLINE this_type& assign(size_type n, Args&&... args) {
		ccdk_assert(n <= N);
		destruct_content();
		n = fn::min(N, n);
		util::construct_n<T>(content.address(), n, util::forward<Args>(args)...);
		len = n;
		return *this;
	}

	// copy-n assign
	template<
		typename InputIt,
		typename = check_t< is_iterator<InputIt>> >
	CCDK_FORCEINLINE this_type& assign(InputIt beginIt, ptr::size_t n) {
		ccdk_assert(n <= N);
		destruct_content();
		n = fn::min(N, n);
		util::construct_copy_n(content.address(), beginIt, n);
		len = n;
		return *this;
	}

	// copy range assign
	template<
		typename InputIt,
		typename = check_t< is_iterator<InputIt>> >
	CCDK_FORCEINLINE this_type& assign(InputIt beginIt, InputIt endIt) {
		return assign(beginIt, it::distance(beginIt, endIt));
	}

	// element access
	CCDK_FORCEINLINE reference operator[](size_type index) noexcept { ccdk_assert( index < len); return content[iround(index)]; }
	CCDK_FORCEINLINE const_reference operator[](size_type index) const noexcept { ccdk_assert( index < len); return content[iround(index)]; } 
	CCDK_FORCEINLINE reference at(size_type index) noexcept { ccdk_assert(index < len); return content[iround(index)]; }
	CCDK_FORCEINLINE const_reference at(size_type index) const noexcept { ccdk_assert(index < len); return content[iround(index)]; }
	CCDK_FORCEINLINE reference front() noexcept { ccdk_assert(content.address()); return content[ibegin]; }
	CCDK_FORCEINLINE const_reference front() const noexcept { ccdk_assert(content.address()); return content[ibegin]; }
	CCDK_FORCEINLINE reference back() noexcept { ccdk_assert(content.address()); return content[iback()];}
	CCDK_FORCEINLINE const_reference back() const noexcept { ccdk_assert(len > 0); return content[iback()]; }
	CCDK_FORCEINLINE reference back(size_type n) noexcept { ccdk_assert(n>=0 && n<len); return content[irround(n)]; }
	CCDK_FORCEINLINE const_reference back(size_type n) const noexcept { ccdk_assert(n >= 0 && n<len); return content[irround(n)]; }

	// readobly attribute
	CCDK_FORCEINLINE size_type size() const noexcept { return len; }
	CCDK_FORCEINLINE size_type capcity() const noexcept { return N; }
	CCDK_FORCEINLINE constexpr size_type max_size() const noexcept { return N;  }
	CCDK_FORCEINLINE bool empty() const noexcept { return len == 0; }

	// iterator
	CCDK_FORCEINLINE iterator begin() noexcept { return { content.address(), kRoundVal, ibegin }; }
	CCDK_FORCEINLINE iterator end() noexcept { return { content.address(), kRoundVal, iend() }; }
	CCDK_FORCEINLINE const_iterator begin() const noexcept { return { content.address(), kRoundVal, ibegin }; }
	CCDK_FORCEINLINE const_iterator end() const noexcept { return { content.address(), kRoundVal, iend() }; }
	CCDK_FORCEINLINE const_iterator cbegin() const noexcept { return { content.address(), kRoundVal, ibegin }; }
	CCDK_FORCEINLINE const_iterator cend() const noexcept { return { content.address(), kRoundVal, iend() }; }
	CCDK_FORCEINLINE reverse_iterator rbegin() noexcept { return { { content.address(), kRoundVal, iend() } }; }
	CCDK_FORCEINLINE reverse_iterator rend() noexcept { return { { content.address(), kRoundVal, iprev(ibegin) } }; }
	CCDK_FORCEINLINE const_reverse_iterator rbegin() const noexcept { return { { content.address(), kRoundVal, iend() } }; }
	CCDK_FORCEINLINE const_reverse_iterator rend() const noexcept { return { { content.address(), kRoundVal,  iprev(ibegin) } }; }
	CCDK_FORCEINLINE const_reverse_iterator crbegin() const noexcept { return { { content.address(), kRoundVal, iend() } }; }
	CCDK_FORCEINLINE const_reverse_iterator crend() const noexcept { return { { content.address(), kRoundVal,  iprev(ibegin) } }; }

	// pop back
	CCDK_FORCEINLINE this_type& pop_back(size_type n = 1) {
		if (len > 0) {
			--len;
			util::destruct<T>(content.address() + iend());
		}
		return *this;
	}


	// pop front
	CCDK_FORCEINLINE this_type& pop_front() {
		if (len > 0) {
			util::destruct<T>(content.address() + ibegin);
			ibegin = inext(ibegin);
			--len;
		}
		return *this;
	}

	// emplace at back
	template<
		typename ... Args,
		typename = check_t< has_constructor<T,Args...>> >
	CCDK_FORCEINLINE this_type& emplace_back(Args&&... args) {
		if (len < N) {
			util::construct<T>(content.address() + iend(), util::forward<Args>(args)...);
			++len;
		}
		return *this;
	}

	CCDK_FORCEINLINE this_type& push_back(T const& t) { return emplace_back(t); }
	CCDK_FORCEINLINE this_type& push_back(T&& t) {return emplace_back(util::move(t)); }

	// emplace at front
	template<
		typename ... Args,
		typename = check_t< has_constructor<T,Args...>> >
	CCDK_FORCEINLINE this_type& emplace_front(Args&&... args) {
		if (len < N) {
			ibegin = iprev(ibegin);
			util::construct<T>(content.address() + ibegin, util::forward<Args>(args)...);
			++len;
		}
		return *this;
	}

	CCDK_FORCEINLINE this_type& push_front(T const& t) { return emplace_front(t); }
	CCDK_FORCEINLINE this_type& push_front(T&& t) { return emplace_front(util::move(t)); }

///////////////////////////////////////////////////////////////////////////////////
////  transform / loop over

	template<typename FN>
	CCDK_FORCEINLINE void foreach(FN Fn) const noexcept {
		if (len) {
			size_type end_idx = iend();
			if (end_idx <= ibegin) {
				for (size_type i = ibegin; i < kCeilPow2; ++i) {
					Fn(content[i]);
				}
				for (size_type i = 0; i < end_idx; ++i) {
					Fn(content[i]);
				}
			}
			else {
				for (size_type i = ibegin; i < end_idx; ++i) {
					Fn(content[i]);
				}
			}
		}
	}


private:

	CCDK_FORCEINLINE void rvalue_reset() noexcept {
		destruct_content();
		ibegin = len = 0;
	}

	// destruct content
	CCDK_FORCEINLINE void destruct_content() noexcept {
		if (len > 0) {
			size_type end_index = iend();
			if (end_index <= ibegin) {                                  /* tail is round to head */
				util::destruct_n(content.address() + ibegin, kCeilPow2 - ibegin); /* destruct tail objects */
				util::destruct_n(content.address(), iend());                      /* destruct head objects */
			}
			else {
				util::destruct_n(content.address() + ibegin, len);
			}
			len = 0;     /* reset len */
			ibegin = 0;
		}
	}

public:

	CCDK_FORCEINLINE void debug_all(const char* title="") const noexcept {
		DebugValueItBegin(title);
		foreach([](T const& t) {
			DebugValueIt(t);
		});
		DebugValueItEnd();
	}

};

ccdk_namespace_ct_end