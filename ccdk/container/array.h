#pragma once

#include<ccdk/container/container_mudule.h>
#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/mcontainer/val_pack.h>
#include<ccdk/mpl/iterator/ptr_iterator.h>
#include<ccdk/mpl/util/move.h>
#include<ccdk/mpl/util/fill.h>
#include<ccdk/container/filter/filter_view.h>

ccdk_namespace_ct_start

using namespace ccdk::mpl;

template<typename T, uint32 N>
class array
{
public:
	typedef T                               value_type;
	typedef T*                              pointer_type;
	typedef T const*                        const_pointer_type;
	typedef T&						        reference_type;
	typedef T const&                        const_reference_type;
	typedef array                           this_type;
	typedef filter_t<T>                     filter_type;
	typedef filter_view_t<this_type>        view_type;
	typedef it::iterator<T*>                iterator_type;
	typedef it::reverse_iterator<T*>        reverse_iterator_type;
private:
	T       content[N];
	uint32  length;

	/* copy compile time c-array to content */
	template<uint32... indice>
	CCDK_FORCEINLINE constexpr array(T const* arr, mpl::val_pack<uint32, indice...>, uint32 inLength)
		: content{ arr[indice]... }, length{ inLength } {}

public:
	CCDK_FORCEINLINE constexpr array() : length(0) {}

	/* compile-time copy, to delegate constructor, note for c-string length include 0-terminal  */
	template<uint32 M, typename = mpl::check_lequal<M, N>>
	CCDK_FORCEINLINE constexpr array(const T(&arr)[M]) : array(arr, mpl::make_indice<M>{}, M) { }
	/* fill n */
	CCDK_FORCEINLINE constexpr array(const T& t, uint32 n = N): length{ N } { using util::fill; fill(content, t, n); }

	/* index and filter-index */
	CCDK_FORCEINLINE view_type operator[](filter_type&& filter) { return view_type{ *this, util::move(filter) }; }
	CCDK_FORCEINLINE constexpr T& operator[](int32 index) {	ccdk_check_index(index,length); return content[(index + length) % length]; }
	CCDK_FORCEINLINE constexpr T const& operator[](int32 index) const { ccdk_check_index(index, length); return content[(index + length) % length]; }
	CCDK_FORCEINLINE constexpr T& at(uint32 index) noexcept { ccdk_assert(index < length); return content[index]; }
	CCDK_FORCEINLINE constexpr T const& at(uint32 index) const noexcept { ccdk_assert(index < length); return content[index]; }

	/* access */
	CCDK_FORCEINLINE constexpr T const* data() const noexcept { return content; }
	CCDK_FORCEINLINE constexpr T& front() noexcept { return content[0]; }
	CCDK_FORCEINLINE constexpr T const& front() const noexcept { return content[0]; }
	CCDK_FORCEINLINE constexpr T& back() noexcept { return content[length-1]; }
	CCDK_FORCEINLINE constexpr T const& back() const noexcept { return content[length-1]; }

	/* attribute */
	CCDK_FORCEINLINE constexpr uint32 size() const noexcept { return length; }
	CCDK_FORCEINLINE constexpr uint32 capcity() const noexcept { return N; }
	CCDK_FORCEINLINE constexpr uint32 max_size() const noexcept { return N; }

	/* expand */
	CCDK_FORCEINLINE constexpr this_type& fit_capcity() noexcept { length = N; return *this; }

	/* iterator */
	CCDK_FORCEINLINE constexpr iterator_type begin() const noexcept { return iterator_type{ content }; }
	CCDK_FORCEINLINE constexpr iterator_type end() const noexcept  { return iterator_type{ content + length }; }
	CCDK_FORCEINLINE constexpr const_iterator_type cbegin() const noexcept  { return const_iterator_type{ content}; }
	CCDK_FORCEINLINE constexpr const_iterator_type cend() const noexcept { return const_iterator_type{ content + length }; }
	CCDK_FORCEINLINE constexpr reverse_iterator_type rbegin() const noexcept { return reverse_iterator_type{ content + length -1 }; }
	CCDK_FORCEINLINE constexpr reverse_iterator_type rend() const noexcept { return reverse_iterator_type{ content -1 }; }
	CCDK_FORCEINLINE constexpr const_reverse_iterator_type crbegin() const noexcept { return const_reverse_iterator_type{ content + length -1 }; }
	CCDK_FORCEINLINE constexpr const_reverse_iterator_type crend() const noexcept { return const_reverse_iterator_type{ content - 1 }; }
};

ccdk_namespace_ct_end