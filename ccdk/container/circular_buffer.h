#pragma once


#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/util/move.h>
#include<ccdk/mpl/util/fill.h>
#include<ccdk/mpl/util/copy.h>
#include<ccdk/mpl/util/construct.h>
#include<ccdk/mpl/util/swap.h>
#include<ccdk/mpl/function/operator.h>
#include<ccdk/mpl/iterator/iterator_fwd.h>
#include<ccdk/mpl/units/ratio.h>
#include<ccdk/memory/simple_new_allocator.h>
#include<ccdk/memory/allocator_traits.h>

#include<ccdk/algorithm/advance.h>
#include<ccdk/algorithm/distance.h>
#include<ccdk/container/circular_buffer_iterator.h>
#include<ccdk/container/container_mudule.h>

ccdk_namespace_ct_start

using namespace ccdk::mpl;

template<
	typename T,
	typename Size = uint32,
	typename InceaseRatio = units::ratio<2, 1>,     /* preallocate 1x  more times node space */
	typename Alloc = mem::simple_new_allocator< T >
>
class circular_buffer : protected Alloc
{
public:
	typedef circular_buffer                this_type;
	typedef circular_buffer                type;

	/* common */
	typedef T                    value_type;
	typedef T*                   pointer_type;
	typedef T const*             const_pointer_type;
	typedef T&                   reference_type;
	typedef T const&             const_reference_type;
	typedef Size                 size_type;
	typedef ptr::diff_t          difference_type;
	
	typedef mem::allocator_traits< Alloc > allocate_type;   

	/* iterator */
	typedef circular_buffer_iterator<T,Size>          iterator_type;
	typedef reverse_circular_buffer_iterator<T, Size> reverse_iterator_type;
	typedef const iterator_type                       const_iterator_type ;
	typedef const reverse_iterator_type               const_reverse_iterator_type;

	template<typename T2, typename Ratio2, typename Size2, typename Alloc2>
	friend class circular_buffer;

private:
	pointer_type content;
	size_type    begin;
	size_type    len;
	size_type    cap;

public:

	/* destroy */
	CCDK_FORCEINLINE ~circular_buffer() noexcept {
		destruct_content();
		allocate_type::deallocate(*this, content, cap);
		cap = begin = 0;
	}

	/* default and nullptr ctor */
	CCDK_FORCEINLINE constexpr circular_buffer() noexcept
		: content{ nullptr }, begin{ 0 }, len{ 0 }, cap{ 0 } {}
	CCDK_FORCEINLINE constexpr circular_buffer(ptr::nullptr_t) noexcept
		: content{ nullptr }, begin{ 0 }, len{ 0 }, cap{ 0 } {}

	/* fill n */
	CCDK_FORCEINLINE constexpr circular_buffer(size_type n, T const& t) noexcept{
		ccdk_assert(n > 0);
		allocate_fill(n, t);
	}
	
	/* copy range */
	template<
		typename InputIt,
		typename = check_t< is_iterator<InputIt>>
	>
	CCDK_FORCEINLINE circular_buffer(InputIt beginIt, size_type n) noexcept {
		ccdk_assert(n > 0);
		allocate_copy(n, beginIt);
	}

	/* copy range */
	template<
		typename InputIt,
		typename = check_t< is_iterator<InputIt>>
	>
	CCDK_FORCEINLINE circular_buffer(InputIt begin, InputIt end) {
		ptr::size_t n = alg::distance(begin, end);
		ccdk_assert(n > 0);
		allocate_copy(n, begin);
	}

	/* copy */
	CCDK_FORCEINLINE circular_buffer(circular_buffer const& other) {
		allocate_copy(other.len, other.content);
	}

	/* template copy */
	template<typename Size2, typename Alloc2, typename Ratio2 >
	CCDK_FORCEINLINE explicit circular_buffer(circular_buffer<T, Ratio2, Size2, Alloc2> const& other) {
		allocate_copy(other.len, other.content);
	}

	/* move */
	CCDK_FORCEINLINE circular_buffer(circular_buffer && other) noexcept
		:content{ other.content }, begin{ other.begin }, len{ other.len }, cap{ other.cap } {
		other.rvalue_reset();
	}

	/* template move */
	template<typename Size2, typename Alloc2, typename Ratio2>
	CCDK_FORCEINLINE circular_buffer(circular_buffer<T, Ratio2, Size2, Alloc2> && other) noexcept
		:content{ other.content }, begin{ other.begin }, len{ other.len }, cap{ other.cap } {
		other.rvalue_reset();
	}

	/* assign  nullptr, first destruct objects */
	CCDK_FORCEINLINE this_type& operator=(ptr::nullptr_t) noexcept {
		destruct_content();
		return *this;
	}

	/* copy assign */
	CCDK_FORCEINLINE this_type& operator=(circular_buffer const& other) {
		ccdk_if_not_this(other) { copy_or_allocate_copy(other.content, other.len); }
		return *this;
	}

	/* template copy assign */
	template<typename Size2, typename Alloc2, typename Ratio2>
	CCDK_FORCEINLINE this_type& operator=(circular_buffer<T, Ratio2, Size2, Alloc2> const& other) {
		copy_or_allocate_copy(other.content, other.len);
		return *this;
	}

	/* move assign */
	CCDK_FORCEINLINE this_type& operator=(circular_buffer && other) {
		ccdk_if_not_this(other) { 
			this->rvalue_set(other.content, other.begin, other.len, other.cap);
			other.rvalue_reset();
		}
		return *this;
	}

	/* template move assign */
	template<typename Size2, typename Alloc2, typename Ratio2>
	CCDK_FORCEINLINE this_type& operator=(circular_buffer<T, Ratio2, Size2, Alloc2> && other) {
		this->rvalue_set(other.content, other.begin, other.len, other.cap);
		other.rvalue_reset();
		return *this;
	}
	
	/* swap */
	CCDK_FORCEINLINE void swap(circular_buffer& other) noexcept {
		util::swap(content, other.content);
		util::swap(begin, other.begin);
		util::swap(len, other.len);
		util::swap(cap, other.cap);
	}

	/* assign nullptr */
	CCDK_FORCEINLINE this_type& assign(ptr::nullptr_t) noexcept {
		return (*this = nullptr);
	}

	/* fill with nxt */
	CCDK_FORCEINLINE this_type& assign(size_type n, T const& t = T()) {
		fill_or_allocate_fill(t, n);
		return *this;
	}

	/* copy n*/
	template<typename InputIt,
		typename = check_t< is_iterator<InputIt>>
	>
	CCDK_FORCEINLINE this_type& assign(InputIt begin, ptr::size_t n) {
		copy_or_allocate_copy(begin, n);
		return *this;
	}

	/* copy range */
	template<
		typename InputIt,
		typename = check_t< is_iterator<InputIt>>
	>
	CCDK_FORCEINLINE this_type& assign(InputIt begin, InputIt end) {
		return assign(begin, alg::distance(begin, end));
	}

	/* initialize_list */
	CCDK_FORCEINLINE this_type& assign(std::initializer_list<T> const& lst) {
		return assign(lst.begin(), lst.size());
	}

	/* access */
	CCDK_FORCEINLINE T& operator[](size_type index) noexcept {
		ccdk_assert(cap > 0 && index < len);
		return content[(begin + index) % cap];
	}
	CCDK_FORCEINLINE T const& operator[](size_type index) const noexcept {
		ccdk_assert(cap > 0 && index < len);
		return content[(begin + index) % cap];
	}

	CCDK_FORCEINLINE T& at(size_type index) noexcept {
		ccdk_assert(cap > 0 && index < len);
		return content[(begin + index) % cap];
	}
	CCDK_FORCEINLINE T const& at(size_type index) const noexcept {
		ccdk_assert(cap > 0 && index < len);
		return content[(begin + index) % cap];
	}
	CCDK_FORCEINLINE T& front() noexcept {
		return content[begin];
	}

	CCDK_FORCEINLINE T const& front() const noexcept {
		return content[begin];
	}
	CCDK_FORCEINLINE T& back() noexcept {
		return content[back_index()];
	}

	CCDK_FORCEINLINE T const& back() const noexcept {
		return content[back_index()];
	}

	/* attribute */
	CCDK_FORCEINLINE size_type length() const noexcept { return len; }
	CCDK_FORCEINLINE size_type capcity() const noexcept { return cap; }
	CCDK_FORCEINLINE size_type max_size() const noexcept { allocate_type::max_allocate_size(); }
	CCDK_FORCEINLINE bool empty() const noexcept { return len == 0; }

	/* iterator */
	CCDK_FORCEINLINE iterator_type begin() noexcept {
		return {content, cap, begin };
	}

	CCDK_FORCEINLINE iterator_type begin() noexcept {
		return { content, cap, end_index() };
	}

	CCDK_FORCEINLINE const_iterator_type cbegin() const noexcept {
		return { content, cap, begin };
	}
	CCDK_FORCEINLINE const_iterator_type cend() const noexcept {
		return { content, cap, end_index() };
	}
	CCDK_FORCEINLINE reverse_iterator_type rbegin() noexcept {
		return { content, cap, (begin + len - 1) % cap };
	}

	CCDK_FORCEINLINE reverse_iterator_type rend() noexcept {
		return { content, cap,  (begin + cap - 1) % cap };
	}

	CCDK_FORCEINLINE const_reverse_iterator_type crbegin() const noexcept {
		return { content, cap, (begin + len - 1) % cap };
	}
	CCDK_FORCEINLINE const_reverse_iterator_type crend() const noexcept {
		return { content, cap, (begin + cap -1) % cap };
	}

	/* pop back */
	CCDK_FORCEINLINE this_type& pop_back()  {
		if (len > 0) {
			util::destruct<T>(content + back_index());
			--len;
		}
	}
	
	/* pop front */
	CCDK_FORCEINLINE this_type& pop_front() {
		if (len > 0) {
			util::destruct<T>(content + begin );
			begin = (begin + 1) % cap;
			--len;
		}
	}

	/* emplace back and push back */
	template<typename T, typename ... Args>
	CCDK_FORCEINLINE this_type& emplace_back(T&& t, Args&&... args){
		if (len >= cap) { reallocate_move(); }
		util::construct<T>(content + back_index(), util::forward<T>(t),
			util::forward<Args>(args)...);
		++len;
		return *this;
	}

	CCDK_FORCEINLINE this_type& push_back(T const& t) {
		return emplace_back(t);
	}

	CCDK_FORCEINLINE this_type& push_back(T&& t) {
		return emplace_back(util::move(t));
	}

	/* emplace front and push front */
	template<typename T, typename ... Args>
	CCDK_FORCEINLINE this_type& emplace_front(T&& t, Args&&... args) {
		if (len >= cap) { reallocate_move(); }
		util::construct<T>(content + --begin, util::forward<T>(t),
			util::forward<Args>(args)...);
		++len;
		return *this;
	}

	CCDK_FORCEINLINE this_type& push_front(T const& t) {
		return emplace_front(t);
	}

	CCDK_FORCEINLINE this_type& push_front(T&& t) {
		return emplace_front(util::move(t));
	}

	/* inplace-construct at pos( [0,len] ) */
	template<
		typename P, typename... Args,
		typename = check_t< has_constructor<T, P, Args...>>
	>
		CCDK_FORCEINLINE this_type& emplace(size_type pos, P&& p, Args&&... args) {
		ccdk_assert(pos <= len);
		if (pos <= len) { emplace_impl(pos, pos + 1, util::forward<P>(p), util::forward<Args>(args)...); }
		return *this;
	}

	/* inplace-construct at pos( [0,len] ) */
	template<
		typename Iterator,
		typename P, typename... Args,
		typename = check_t<is_pointer_iterator<T, Iterator> >, // exclude comflict with size_type
		typename = check_t< has_constructor<T, P, Args...>>
	>
		CCDK_FORCEINLINE this_type& emplace(Iterator it, P&& p, Args&&... args) {
		return emplace(it - content, util::forward<P>(p), util::forward<Args>(args)...);
	}

private:

	CCDK_FORCEINLINE void rvalue_reset() noexcept {
		content = nullptr;
		begin = len = cap = 0;
	}

	CCDK_FORCEINLINE void rvalue_set(pointer_type inContent, size_type inBegin, 
		size_type inLen, size_type inCap) noexcept {
		content = inContent;
		begin = inBegin;
		len = inLen;
		cap = inCap;
	}


	/* index of end  */
	CCDK_FORCEINLINE void end_index() noexcept {
		return (begin + len) % cap;
	}

	CCDK_FORCEINLINE void back_index() noexcept {
		return (begin + len - 1) % cap;
	}


	/*
		allocate n*increase size memory to content,
		then fill [begin, begin+n) with v
	*/
	CCDK_FORCEINLINE void allocate_fill(size_type n, T const& v) {
		ccdk_increase_allocate3(n, content, cap, len);
		begin = (size_type((float)(cap-n)*0.5f);
		util::construct_fill_n(content+begin, v, n);
	}

	/*
		allocate n*increase_ratio size memory to content,
		then copy [begin, begin+n) to [content+begin, content+end)
	*/
	template<typename InputIt>
	CCDK_FORCEINLINE void allocate_copy(size_type n, InputIt beginIt) {
		if (n == 0) return;
		ccdk_increase_allocate3(n, content, cap, len);
		begin = (size_type((float)(cap - n)*0.5f);
		util::construct_copy_n(content + begin, beginIt, n);
	}

	/*
		memory is full , allocate bigger memory to hold
	*/
	void reallocate_move() {
		ccdk_increase_allocate2(len, T* memory, size_type new_cap);
		size_type end = end_index();
		size_type new_begin = (size_type)((float)(new_cap - len)*0.5f);
		if (end <= begin) {
			util::construct_move_n(memory, content, end);
			util::construct_move_n(memory + new_cap - (cap - begin),
				content + begin, cap - begin);
			util::destruct_n(content, end);
			util::destruct_n(content + begin, cap - begin);
		}
		else {
			util::construct_move_n(memory + new_begin, content + begin, end);
			util::destruct_n(content+begin, len);
		}
		
		allocate_type::deallocate(*this, content, cap);
		content = memory;
		cap = new_cap;
		begin = new_begin;
	}


	/* if len > 0 , destruct objects */
	CCDK_FORCEINLINE void destruct_content() noexcept {
		if (len > 0) {
			if (end_index() <= begin) {                      /* tail is round to head */
				util::destruct_n(content+begin, cap-begin);  /* destruct tail objects */
				util::destruct_n(content, end);              /* destruct head objects */
			} else { 
				util::destruct_n(content + begin, len);      
			}
			len = 0;                                         /* reset len */
		}
	}


	/* copy from [begin, begin+n) to content[0, n), destruct old objects  */
	CCDK_FORCEINLINE void range_fill(T const& t, size_type n) {
		if (n == 0) return;
		destruct_content();
		begin = (size_type)((float)(cap - n)*0.5f);
		util::construct_fill_n(content + begin, t, n);
		len = n;
	}

	/* copy from [begin, begin+n) to content[0, n), destruct old objects  */
	template<typename InputIt>
	CCDK_FORCEINLINE void range_copy(InputIt beginIt, size_type n) {
		if (n == 0) return;
		destruct_content();
		begin = (size_type)((float)(cap - n)*0.5f);
		util::construct_copy_n(content + begin, beginIt, n);
		len = n;
	}

	/*
		if content big enough to hold n elements, just copy [begin, begin+n) to it
		else allocate new big enough memory and swap with it
	*/
	template<typename InputIt>
	CCDK_FORCEINLINE void copy_or_allocate_copy(InputIt beginIt, size_type n) {
		if (n <= cap) range_copy(beginIt, n);
		else circular_buffer{ beginIt, n }.swap(*this);
	}

	/*
		if content big enough to hold n elements, just fill it
		else allocate new big enough memory and swap with it
	*/
	CCDK_FORCEINLINE void fill_or_allocate_fill(T const& t, size_type n) {
		if (n <= cap) range_fill(t, n);
		else circular_buffer{ n,t }.swap(*this);
	}


	/*
		if capacity is not big enough, realloc big enough memory to
		hold old data and insert data, [memory+start, memory+end) stay
		empty for insert data.
		else move [content+start, content+len) backward.
	*/
	template<typename InputIt>
	this_type& insert_impl(size_type start, size_type end, InputIt begin) {
		ccdk_assert(end > start && start <= len);
		if (end <= start || start > len) return *this;
		size_type n = end - start;
		size_type new_len = len + n;
		if (new_len > cap) {
			ccdk_increase_allocate2(new_len, T* memory, size_type new_cap);
			ccdk_safe_cleanup_if_exception(
				util::construct_copy_n(memory + start, begin, n), /* may throw */
				allocate_type::deallocate(*this, memory, new_cap)
			);
			split_copy(memory, start, n);
			allocate_type::deallocate(*this, content, cap);
			content = memory;
			cap = new_cap;
		}
		else {
			size_type max_end = fn::max(end, len);
			util::construct_move_n(content + max_end, content + max_end - n, n);
			if (end<len) util::move_n(content + end, content + start, len - end);
			util::destruct_n(content + start, fn::min(end, len) - start);
			util::construct_copy_n(content + start, begin, n);
		}
		len = new_len;
		return *this;
	}

	/*
		if capacity is not enough, realloc big enough memory to hold emplace construct data,
		memory[start, end) stay empty for external data to copy in.
		else move content[start, len) backward
	*/
	template<typename... Args>
	void emplace_impl(size_type start, size_type end, Args&&... args) {
		if (end <= start || start > len) return;
		size_type n = end - start;
		size_type new_len = len + n;
		if (new_len > cap) {
			ccdk_increase_allocate2(new_len, T* memory, size_type new_cap);
			ccdk_safe_cleanup_if_exception(
				util::construct_n<T>(memory + start, n, util::forward<Args>(args)...),
				allocate_type::deallocate(*this, memory, new_cap)
			);
			split_copy(memory, start, n);
			allocate_type::deallocate(*this, content, cap);
			content = memory;
			cap = new_cap;
		}
		else {
			size_type max_end = fn::max(end, len);
			util::construct_move_n(content + max_end, content + max_end - n, n);
			if (end<len) util::move_n(content + end, content + start, len - end);
			util::destruct_n(content + start, fn::min(end, len) - start);
			util::construct_n<T>(content + start, n, util::forward<Args>(args)...);
		}
		len = new_len;
	}

};

ccdk_namespace_ct_end