#pragma once

#include<ccdk/container/container_mudule.h>
#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/mcontainer/val_pack.h>
#include<ccdk/mpl/iterator/ptr_iterator.h>
#include<ccdk/mpl/iterator/reverse_iterator.h>
#include<ccdk/mpl/util/move.h>
#include<ccdk/mpl/util/construct.h>
#include<ccdk/mpl/function/operator.h>

#include<ccdk/algorithm/distance.h>
#include<ccdk/container/filter/filter_view.h>

ccdk_namespace_ct_start

using namespace ccdk::mpl;

template<typename T, uint32 N>
class array
{
public:
	/* */
	typedef array                           this_type;

	/* container */
	typedef T                               value_type;
	typedef T*                              pointer_type;
	typedef T const*                        const_pointer_type;
	typedef T&						        reference_type;
	typedef T const&                        const_reference_type;
	typedef uint32                          size_type;
	typedef int32                           difference_type;

	typedef filter_t<T>                     filter_type;
	typedef filter_view_t<this_type>        view_type;

	/* iterator */
	typedef it::iterator<T*>                    iterator_type;
	typedef it::iterator<T*> const              const_iterator_type;
	typedef it::reverse_iterator<iterator_type> reverse_iterator_type;
	typedef it::reverse_iterator<iterator_type> const  const_reverse_iterator_type;

	template<typename T2, uint32 N2>
	friend class array;

private:
	T       content[N];
	uint32  len;

	/* copy compile time c-array to content */
	template<uint32... indice>
	CCDK_FORCEINLINE constexpr array(T const* arr,
		mpl::val_pack<uint32, indice...>, uint32 inLength)
		: content{ arr[indice]... }, len{ inLength } {}

public:

	/* default */
	CCDK_FORCEINLINE constexpr array() : len(0) {}


	/* compile-time arry constructor, note for c-string length include 0-terminal  */
	template<
		uint32 N2, 
		uint32 Min = min_val<uint32, N,N2>
	>
	CCDK_FORCEINLINE constexpr array(const T(&arr)[N2])
		: array(arr, mpl::make_indice<Min>{}, Min) {}

	/* fill n */
	CCDK_FORCEINLINE constexpr array(uint32 n , const T& t = T()) : len{ n } {
		util::construct_fill_n(content, t, n); 
	}

	/* range-n */
	template<typename InputIt, typename = check_t< is_iterator<InputIt>> >
	CCDK_FORCEINLINE constexpr array(InputIt beginIt, size_type n) : len{ fn::min(n,N) } {
		util::construct_copy_n(content, beginIt, n);
	}

	/* range */
	template<typename InputIt, typename = check_t< is_iterator<InputIt>>>
	CCDK_FORCEINLINE constexpr array(InputIt beginIt, InputIt endIt)
		: len{ fn::min(N, alg::distance(beginIt, endIt)) } {
		util::construct_copy_n(content, beginIt, len);
	}

	/* template copy */
	template<uint32 N2, uint32 Min = min_c2<uint32, N,N2>::value >
	CCDK_FORCEINLINE constexpr array(array<T,N2> const& other)
		: array{other.content, mpl::make_indice<Min>{}, fn::min( Min, other.len)} {
	}

	/* copy */
	CCDK_FORCEINLINE constexpr array(array const& other)
		: array{other.content, mpl::make_indice<N>{}, other.len} {}

	/* template move */
	template<uint32 N2>
	CCDK_FORCEINLINE array(array<T, N2> && other) : len{ other.len }{
		static_assert(N2 < N, "N2 need less then N");
		util::move_n(content, other.content, len);
		other.len = 0;
	}

	/* move */
	CCDK_FORCEINLINE array(array && other) : len{ other.len } {
		util::move_n(content, other.content, len);
		other.len = 0;
	}


	/* array assign */
	template< uint32 N2, uint32 Min = min_val<uint32, N, N2> >
	CCDK_FORCEINLINE this_type& operator=(const T(&arr)[N2]){
		util::destruct_n(content, len);
		util::construct_copy_n(content, arr, Min);
		len = Min;
	}

	/* template copy assign */
	template<uint32 N2>
	CCDK_FORCEINLINE this_type& operator=(array<T, N2> const& other) {
		util::destruct_n(content, len);
		len = fn::min(N, other.len);
		util::construct_copy_n(content, other.content, len);
	}

	/* copy assign */
	CCDK_FORCEINLINE this_type& operator=(array const& other) {
		util::destruct_n(content, len);
		util::construct_copy_n(content, other.content, other.len);
		len = other.len;
	}

	/* template move assign */
	template<uint32 N2>
	CCDK_FORCEINLINE this_type& operator=(array<T, N2> && other) {
		static_assert(N2 <= N, "N2 need less N");
		util::destruct_n(content, len);
		len = other.len;
		util::construct_move_n(content, other.content, other.len);
		other.len = 0;
	}

	/* template move assign */
	CCDK_FORCEINLINE this_type& operator=(array&& other) {
		util::destruct_n(content, len);
		len = other.len;
		util::construct_move_n(content, other.content, other.len);
		other.len = 0;
	}

	/* assign fill n */
	CCDK_FORCEINLINE this_type& assign(size_type n, T const& t = T()) {
		util::destruct_n(content, len);
		len = fn::min(n, N);
		util::construct_fill_n(content, t, len);
	}

	/* assign range-n */
	template<typename InputIt, typename = check_t< is_iterator<InputIt>>>
	CCDK_FORCEINLINE this_type& assign(InputIt beginIt, size_type n) {
		util::destruct_n(content, len);
		len = fn::min(n, N);
		util::construct_copy_n(content, beginIt, len);
	}

	/* assign range */
	template<typename InputIt, typename = check_t< is_iterator<InputIt>>>
	CCDK_FORCEINLINE this_type& assign(InputIt beginIt, InputIt endIt) {
		util::destruct_n(content, len);
		len = fn::min(alg::distance(beginIt, endIt), N);
		util::construct_copy_n(content, beginIt, len);
	}

	/* filte index */
	CCDK_FORCEINLINE view_type operator[](filter_type&& filter) { 
		return view_type{ *this, util::move(filter) }; 
	}

	/* index */
	CCDK_FORCEINLINE constexpr reference_type operator[](difference_type index) {
		ccdk_check_index(index,len); 
		return content[(index + len) % len];
	}

	/* const index */
	CCDK_FORCEINLINE constexpr const_reference_type operator[](difference_type index) const {
		ccdk_check_index(index, len);
		return content[(index + len) % len];
	}

	/* quick index */
	CCDK_FORCEINLINE constexpr reference_type at(size_type index) noexcept {
		ccdk_assert(index < len); 
		return content[index]; 
	}

	/* quick const index */
	CCDK_FORCEINLINE constexpr const_reference_type at(uint32 index) const noexcept {
		ccdk_assert(index < len);
		return content[index]; 
	}

	/* access data */
	CCDK_FORCEINLINE constexpr const_pointer_type data() const noexcept { return content; }
	CCDK_FORCEINLINE constexpr pointer_type data() noexcept { return content; }

	/* access front */
	CCDK_FORCEINLINE constexpr reference_type front() noexcept { return content[0]; }
	CCDK_FORCEINLINE constexpr const_reference_type front() const noexcept { return content[0]; }

	/* access back */
	CCDK_FORCEINLINE constexpr reference_type back() noexcept { 
		ccdk_assert(len > 0);
		return content[len - 1]; 
	}
	CCDK_FORCEINLINE constexpr const_reference_type back() const noexcept { 
		ccdk_assert(len > 0);
		return content[len-1]; 
	}

	/* attribute */
	CCDK_FORCEINLINE constexpr size_type size() const noexcept { return len; }
	CCDK_FORCEINLINE constexpr size_type capcity() const noexcept { return N; }
	CCDK_FORCEINLINE constexpr size_type max_size() const noexcept { return size_type(-1) / sizeof(T); }

	/* iterator */
	CCDK_FORCEINLINE constexpr iterator_type begin()  noexcept { return { content }; }
	CCDK_FORCEINLINE constexpr iterator_type end()  noexcept  { return { content + len }; }
	CCDK_FORCEINLINE constexpr const_iterator_type cbegin() noexcept  { return { content}; }
	CCDK_FORCEINLINE constexpr const_iterator_type cend()  noexcept { return { content + len }; }
	CCDK_FORCEINLINE constexpr reverse_iterator_type rbegin() noexcept { return { { content + len - 1 } }; }
	CCDK_FORCEINLINE constexpr reverse_iterator_type rend() noexcept { return { { content - 1 } }; }
	CCDK_FORCEINLINE constexpr const_reverse_iterator_type crbegin() const noexcept { return { { content + len - 1 } }; }
	CCDK_FORCEINLINE constexpr const_reverse_iterator_type crend() const noexcept { return { { content - 1 } }; }
};

ccdk_namespace_ct_end