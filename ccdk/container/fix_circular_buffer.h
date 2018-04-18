#pragma once


#include<ccdk/mpl/base/compile_check.h>
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
#include<ccdk/memory/simple_new_allocator.h>
#include<ccdk/memory/allocator_traits.h>
#include<ccdk/container/container_mudule.h>

ccdk_namespace_ct_start

using namespace ccdk::mpl;

template<
	typename T,
	uint32 N
>
class fix_circular_buffer
{
public:
	using this_type = fix_circular_buffer;

	/* common */
	using value_type      = T;
	using pointer         = T*;
	using const_pointer   = T const*;
	using reference       = T&;
	using const_reference = T const&;
	using size_type       = uint32;
	using difference_type = ptr::diff_t;

	/* iterator */
	using iterator               = it::iterator<round_index_category, T>;
	using const_iterator         = it::iterator<round_index_category, const T>;
	using reverse_iterator       = it::reverse_iterator<iterator>;
	using const_reverse_iterator = it::reverse_iterator<const_iterator>;

private:
	fs::local_arr<T, N> content;
	size_type           begin;
	size_type           len;
public:

	/* destroy */
	CCDK_FORCEINLINE ~fix_circular_buffer() noexcept {
		len = begin = 0;
	}

	/* default and nullptr ctor */
	CCDK_FORCEINLINE fix_circular_buffer() = default;
	CCDK_FORCEINLINE constexpr fix_circular_buffer(ptr::nullptr_t) noexcept
		: content{}, begin{ 0 }, len{ 0 } {}

	/* fill-n */
	template<
		typename... Args,
		typename = check_t< has_constructor<T,Args...> >
	>
	CCDK_FORCEINLINE fix_circular_buffer(size_type n, Args&&... args)
		noexcept(has_nothrow_constructor_v<T, Args...>)
		: begin{ 0 }, len{ n }, content{ util::forward<Args>(args)... } {
		ccdk_assert(n > 0 && n < N);
	}

	/* copy range */
	template<
		typename InputIt,
		typename = check_t< is_iterator<InputIt>>
	>
	CCDK_FORCEINLINE fix_circular_buffer(InputIt beginIt, size_type n) 
		: begin{0}, len{n}, content{ beginIt, n } {
		ccdk_assert(n > 0 && n < N);
	}

	/* copy range */
	template<
		typename InputIt,
		typename = check_t< is_iterator<InputIt>>
	>
	CCDK_FORCEINLINE fix_circular_buffer(InputIt beginIt, InputIt endIt)
		: fix_circular_buffer(beginIt, it::distance(beginIt, endIt)) {}

	/* copy */
	CCDK_FORCEINLINE fix_circular_buffer(fix_circular_buffer const& other) {
		if (other.size() > 0) {
			util::construct_copy_n(content.address(),
				other.begin(), other.size());
		}
	}

	/* template copy */
	template<uint32 N2>
	CCDK_FORCEINLINE fix_circular_buffer(fix_circular_buffer<T, N2> const& other) {
		if (other.size() > 0) {
			util::construct_copy_n(content.address(),
				other.begin(), fn::min(N, other.size()));
		}
	}

	/* move */
	CCDK_FORCEINLINE fix_circular_buffer(fix_circular_buffer && other) noexcept
		: len{ other.len } {
		util::construct_move_n(content.address(), other.begin(), other.size());
	}

	/* template move */
	template<uint32 N2>
	CCDK_FORCEINLINE fix_circular_buffer(fix_circular_buffer<T, N2> && other) noexcept
		:len{ other.len } {
		util::construct_move_n(content.address(),
			other.begin(), fn::min( other.size(), N) );
	}

	/* assign  nullptr, first destruct objects */
	CCDK_FORCEINLINE this_type& operator=(ptr::nullptr_t) noexcept {
		destruct_content();
		return *this;
	}

	/* copy assign */
	CCDK_FORCEINLINE this_type& operator=(fix_circular_buffer const& other) {
		ccdk_if_not_this(other) { 
			destruct_content();
			util::construct_copy_n(content.address(),
				other.begin(), other.size());

		}
		return *this;
	}

	/* template copy assign */
	template<typename Size2, typename Alloc2, typename Ratio2>
	CCDK_FORCEINLINE this_type& operator=(fix_circular_buffer<T, N2> const& other) {
		destruct_content();
		util::construct_copy_n(content.address(),
			 other.begin(), fn::min(N, other.size()) );
		return *this;
	}

	/* move assign */
	CCDK_FORCEINLINE this_type& operator=(fix_circular_buffer && other) {
		ccdk_if_not_this(other) {
			destruct_content();
			util::construct_move_n(content.address(),
				other.begin(), fn::min(N, other.size()));
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
		return { content, cap, begin };
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
		return { content, cap, (begin + cap - 1) % cap };
	}

	/* pop back */
	CCDK_FORCEINLINE this_type& pop_back() {
		if (len > 0) {
			util::destruct<T>(content + back_index());
			--len;
		}
	}

	/* pop front */
	CCDK_FORCEINLINE this_type& pop_front() {
		if (len > 0) {
			util::destruct<T>(content + begin);
			begin = (begin + 1) % cap;
			--len;
		}
	}

	/* emplace back and push back */
	template<typename T, typename ... Args>
	CCDK_FORCEINLINE this_type& emplace_back(T&& t, Args&&... args) {
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
		begin = (size_type((float)(cap - n)*0.5f);
		util::construct_fill_n(content + begin, v, n);
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
			util::destruct_n(content + begin, len);
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
				util::destruct_n(content + begin, cap - begin);  /* destruct tail objects */
				util::destruct_n(content, end);              /* destruct head objects */
			}
			else {
				util::destruct_n(content + begin, len);
			}
			len = 0;                                         /* reset len */
			begin = 0;
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