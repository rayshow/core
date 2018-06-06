#pragma once

#include<ccdk/container/container_mudule.h>
#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/mcontainer/val_pack.h>
#include<ccdk/mpl/function/function.h>
#include<ccdk/mpl/fusion/maybe.h>

ccdk_namespace_ct_start

using namespace mpl;

template<
	typename Container, 
	typename size_type = Container::size_type,
	typename difference_type = Container::difference_type
>
size_type absolute_index(Container const& ct, difference_type index) {
	ccdk_assert( ( index<0 && index + difference_type(ct.size()) >= 0) || 
		( index>=0 && index < difference_type(ct.size())) );
	if (index < 0) return size_type(index + difference_type(ct.size()));
	return size_type(index);
}

/* aggresive initialize */
template<typename T>
struct range_filter
{
	using this_type = range_filter;

	ptr::diff_t start;               /*can be negtive */
	ptr::diff_t end;
	ptr::diff_t step;
	fn::function<bool(const T&)> pred;

	/* 2-value */
	CCDK_FORCEINLINE constexpr range_filter(ptr::diff_t inStart, ptr::diff_t inEnd)
		: start{ inStart }, end{ inEnd }, step{ 1 }, pred{} {}

	/* 3-value */
	CCDK_FORCEINLINE constexpr range_filter(ptr::diff_t inStart, ptr::diff_t inEnd,
		ptr::diff_t inStep) : start{ inStart }, end{ -1 }, step{ inStep }, pred{} {}

	/* 4-value */
	template<typename Fn>
	CCDK_FORCEINLINE range_filter(ptr::diff_t inStart, ptr::diff_t inEnd, ptr::diff_t inStep, Fn&& fn)
		: start{ inStart }, end{ -1 }, step{ inStep }, pred{ util::forward<Fn>(fn) } {}

	/* move */
	CCDK_FORCEINLINE range_filter(this_type&& other) : start{ other.start }, end{ other.end },
		step{ other.step }, pred{ util::move(other.pred) } {}
};

ccdk_namespace_ct_end