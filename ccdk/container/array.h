#pragma once

#include<ccdk/mpl/fusion/local_obj.h>
#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/mcontainer/val_pack.h>
#include<ccdk/mpl/iterator/reverse_iterator.h>
#include<ccdk/mpl/util/move.h>
#include<ccdk/mpl/util/construct.h>
#include<ccdk/mpl/function/operator.h>
#include<ccdk/mpl/iterator/algorithm/distance.h>
#include<ccdk/container/filter/range_filter.h>
#include<ccdk/container/view/range_filter_view.h>
#include<ccdk/container/container_mudule.h>

ccdk_namespace_ct_start

using namespace ccdk::mpl;

template<typename T, uint32 N>
class array
{
public:
	using this_type   = array;
	using filter_type = range_filter<T>;
	using view_type   = range_filter_view<this_type>;

	using value_type      = T;
	using pointer         = T*;
	using const_pointer   = T const*;
	using reference       = T&;
	using const_reference = T const&;
	using size_type       = uint32;
	using difference_type = int32;

	using iterator               = T*;
	using const_iterator         = T const*;
	using reverse_iterator       = it::reverse_iterator< iterator>;
	using const_reverse_iterator = it::reverse_iterator< const_iterator>;

	template<typename T2, uint32 N2>
	friend class array;

private:
	T       content[N];
	uint32  len;

	// copy compile time c-array to content 
	template<uint32... indice>
	CCDK_FORCEINLINE array(T const* arr,
		mpl::val_pack<uint32, indice...>, uint32 inLength)
		: content{ arr[indice]... }, len{ inLength } {}

public:

	/* default */
	CCDK_FORCEINLINE constexpr array() : len(0) {}
	CCDK_FORCEINLINE constexpr array(ptr::nullptr_t) : len(0) {}

	/* compile-time arry constructor, note for c-string length include 0-terminal  */
	template<
		uint32 N2, 
		uint32 Min = min_val<uint32, N,N2>
	>
	CCDK_FORCEINLINE  array(const T(&arr)[N2])
		: array(arr, mpl::make_indice<Min>{}, Min) {}

	/* fill n */
	CCDK_FORCEINLINE array(uint32 n, const T& t = T()) : len{ n } {
		util::destruct_n(content, n);
		util::construct_fill_n(content, t, n); 
	}

	/* range-n */
	template<typename InputIt, typename = check_t< is_iterator<InputIt>> >
	CCDK_FORCEINLINE array(InputIt beginIt, size_type n) : len{ fn::min(n,N) } {
		util::destruct_n(content, n);
		util::construct_copy_n(content, beginIt, n);
	}

	/* range */
	template<typename InputIt, typename = check_t< is_iterator<InputIt>>>
	CCDK_FORCEINLINE array(InputIt beginIt, InputIt endIt)
		: array {beginIt, it::distance(beginIt, endIt)}{}

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
		return *this;
	}

	/* template copy assign */
	template<uint32 N2>
	CCDK_FORCEINLINE this_type& operator=(array<T, N2> const& other) {
		util::destruct_n(content, len);
		len = fn::min(N, other.len);
		util::construct_copy_n(content, other.content, len);
		return *this;
	}

	/* copy assign */
	CCDK_FORCEINLINE this_type& operator=(array const& other) {
		util::destruct_n(content, len);
		util::construct_copy_n(content, other.content, other.len);
		len = other.len;
		return *this;
	}

	/* template move assign */
	template<uint32 N2>
	CCDK_FORCEINLINE this_type& operator=(array<T, N2> && other) {
		static_assert(N2 <= N, "N2 need less N");
		util::destruct_n(content, len);
		len = other.len;
		util::construct_move_n(content, other.content, other.len);
		other.len = 0;
		return *this;
	}

	/* template move assign */
	CCDK_FORCEINLINE this_type& operator=(array&& other) {
		util::destruct_n(content, len);
		len = other.len;
		util::construct_move_n(content, other.content, other.len);
		other.len = 0;
		return *this;
	}

	/* assign fill n */
	CCDK_FORCEINLINE this_type& assign(size_type n, T const& t = T()) {
		util::destruct_n(content, len);
		len = fn::min(n, N);
		util::construct_fill_n(content, t, len);
		return *this;
	}

	/* assign range-n */
	template<typename InputIt, typename = check_t< is_iterator<InputIt>>>
	CCDK_FORCEINLINE this_type& assign(InputIt beginIt, size_type n) {
		util::destruct_n(content, len);
		len = fn::min(n, N);
		util::construct_copy_n(content, beginIt, len);
		return *this;
	}

	/* assign range */
	template<typename InputIt, typename = check_t< is_iterator<InputIt>>>
	CCDK_FORCEINLINE this_type& assign(InputIt beginIt, InputIt endIt) {
		util::destruct_n(content, len);
		len = fn::min(it::distance(beginIt, endIt), N);
		util::construct_copy_n(content, beginIt, len);
		return *this;
	}

	/* filte index */
	CCDK_FORCEINLINE view_type operator[](filter_type&& filter) { 
		return { *this, util::move(filter) }; 
	}

	/* index */
	CCDK_FORCEINLINE constexpr reference operator[](difference_type index) {
		//ccdk_check_index(index,len); 
		return content[(index + len) % len];
	}

	/* const index */
	CCDK_FORCEINLINE constexpr const_reference operator[](difference_type index) const {
		ccdk_check_index(index, len);
		return content[(index + len) % len];
	}

	/* quick index */
	CCDK_FORCEINLINE constexpr reference at(size_type index) noexcept {
		ccdk_assert(index < len); 
		return content[index]; 
	}

	/* quick const index */
	CCDK_FORCEINLINE constexpr const_reference at(uint32 index) const noexcept {
		ccdk_assert(index < len);
		return content[index]; 
	}

	/* access data */
	CCDK_FORCEINLINE constexpr const_pointer data() const noexcept { return content; }
	CCDK_FORCEINLINE constexpr pointer data() noexcept { return content; }

	/* access front */
	CCDK_FORCEINLINE constexpr reference front() noexcept { return content[0]; }
	CCDK_FORCEINLINE constexpr const_reference front() const noexcept { return content[0]; }

	/* access back */
	CCDK_FORCEINLINE constexpr reference back() noexcept { 
		ccdk_assert(len > 0);
		return content[len - 1]; 
	}
	CCDK_FORCEINLINE constexpr const_reference back() const noexcept { 
		ccdk_assert(len > 0);
		return content[len-1]; 
	}

	/* attribute */
	CCDK_FORCEINLINE constexpr size_type size() const noexcept { return len; }
	CCDK_FORCEINLINE constexpr size_type capcity() const noexcept { return N; }
	CCDK_FORCEINLINE constexpr size_type max_size() const noexcept {
		return size_type(-1) / sizeof(T);
	}

	/* iterator */
	CCDK_FORCEINLINE constexpr iterator begin()  noexcept { return content; }
	CCDK_FORCEINLINE constexpr iterator end()  noexcept  { return content + len; }
	CCDK_FORCEINLINE constexpr const_iterator cbegin() noexcept  { return content; }
	CCDK_FORCEINLINE constexpr const_iterator cend()  noexcept { return content + len; }
	CCDK_FORCEINLINE constexpr reverse_iterator rbegin() noexcept { return { content + len - 1 }; }
	CCDK_FORCEINLINE constexpr reverse_iterator rend() noexcept { return { content - 1 }; }
	CCDK_FORCEINLINE constexpr const_reverse_iterator crbegin() const noexcept { return { content + len - 1 }; }
	CCDK_FORCEINLINE constexpr const_reverse_iterator crend() const noexcept { return { content - 1 }; }

//////////////////////////////////////////////////////////////////////////////////////////////////////
//// debug

public:
	void debug(const char* title) const noexcept {
		std::cout << title;
		for (uint32 i = 0; i < len; ++i) {
			std::cout << content[i] <<" ";
		}
		std::cout << std::endl;
	}
	void rdebug(const char* title) noexcept {
		std::cout << title;
		for (auto it = rbegin(); it != rend();++it) {
			std::cout << *it << " ";
		}
		std::cout << std::endl;
	}
	void crdebug(const char* title) const noexcept {
		std::cout << title;
		for (auto it = crbegin(); it != crend(); ++it) {
			std::cout << *it << " ";
		}
		std::cout << std::endl;
	}
};

ccdk_namespace_ct_end