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
#include<ccdk/memory/list_allocate_adapter.h>

#include<ccdk/algorithm/advance.h>
#include<ccdk/algorithm/distance.h>
#include<ccdk/container/slist_node.h>
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
	
	typedef mem::allocator_traits< Alloc > allocate_type;   /* allocate 1 node */

	/* iterator */
	typedef T*                         iterator_type;
	typedef T const*                   const_iterator_type;
	typedef reverse_iterator<T*>       reverse_iterator_type;
	typedef reverse_iterator<const T*> const_reverse_iterator_type;

	template<typename T2, typename Ratio2, typename Size2, typename Alloc2>
	friend class circular_buffer;

private:
	pointer_type content;
	size_type    begin;
	size_type    end;
	size_type    cap;

public:
	/* default and nullptr ctor */
	CCDK_FORCEINLINE constexpr circular_buffer() noexcept
		: content{ nullptr }, begin{ 0 }, end{ 0 }, len{ 0 }, cap{ 0 } {}
	CCDK_FORCEINLINE constexpr circular_buffer(ptr::nullptr_t) noexcept
		: content{ nullptr }, begin{ 0 }, end{ 0 }, len{ 0 }, cap{ 0 } {}

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
		allocate_copy(other.length(), other.content);
	}

	/* template copy */
	template<typename Size2, typename Alloc2, typename Ratio2 >
	CCDK_FORCEINLINE explicit circular_buffer(vector<T, Ratio2, Size2, Alloc2> const& other) {
		allocate_copy(other.length(), other.content);
	}

	/* move */
	CCDK_FORCEINLINE circular_buffer(circular_buffer && other) noexcept
		:content{ other.content }, begin{ other.begin }, end{ other.end }, cap{ other.cap } {
		other.rvalue_reset();
	}

	/* template move */
	template<typename Size2, typename Alloc2, typename Ratio2>
	CCDK_FORCEINLINE circular_buffer(vector<T, Ratio2, Size2, Alloc2> && other) noexcept
		:content{ other.content }, begin{ other.begin }, end{ other.end }, cap{ other.cap } {
		other.rvalue_reset();
	}

	/* swap */
	CCDK_FORCEINLINE void swap(vector& other) noexcept {
		util::swap(content, other.content);
		util::swap(begin, other.begin);
		util::swap(end, other.end);
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
	template<typename InputIt,
		typename = check_t< is_iterator<InputIt>>
	>
	CCDK_FORCEINLINE this_type& assign(InputIt begin, InputIt end) {
		return assign(begin, alg::distance(begin, end));
	}

	/* initialize_list */
	CCDK_FORCEINLINE this_type& assign(std::initializer_list<T> const& lst) {
		return assign(lst.begin(), lst.size());
	}


	CCDK_FORCEINLINE size_type length() const noexcept {
		return (end - begin + cap) % cap;
	}

private:

	CCDK_FORCEINLINE void rvalue_reset() noexcept {
		content = nullptr;
		begin = end = cap = 0;
	}

	/*
		allocate n*increase size memory to content,
		then fill [begin, begin+n) with v
	*/
	CCDK_FORCEINLINE void allocate_fill(size_type n, T const& v) {
		ccdk_increase_allocate2(n, content, cap);
		begin = (size_type((float)(cap-n)*0.25f);
		end = begin + n;
		util::construct_fill_n(content+begin, v, n);
	}

	/*
		allocate n*increase_ratio size memory to content,
		then copy [begin, begin+n) to [content+begin, content+end)
	*/
	template<typename InputIt>
	CCDK_FORCEINLINE void allocate_copy(size_type n, InputIt beginIt) {
		if (n == 0) return;
		ccdk_increase_allocate2(n, content, cap);
		begin = (size_type((float)(cap - n)*0.25f);
		end = begin + n;
		util::construct_copy_n(content + begin, beginIt, n);
	}

	/* if len > 0 , destruct objects */
	CCDK_FORCEINLINE void destruct_content() noexcept {
		if (len > 0) { util::destruct_n(content, len); len = 0; }
	}


	/* copy from [begin, begin+n) to content[0, n), destruct old objects  */
	CCDK_FORCEINLINE void range_fill(T const& t, size_type n) {
		destruct_content();
		util::construct_fill_n(content, t, n);
		len = n;
	}

	/* copy from [begin, begin+n) to content[0, n), destruct old objects  */
	template<typename InputIt>
	CCDK_FORCEINLINE void range_copy(InputIt begin, size_type n) {
		destruct_content();
		util::construct_copy_n(content, begin, n);
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
};

ccdk_namespace_ct_end