#pragma once

#include<ccdk/container/container_mudule.h>
#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/mcontainer/make_indice.h>
#include<ccdk/mpl/function/function.h>
#include<ccdk/mpl/fusion/maybe.h>

ccdk_namespace_ct_start

using namespace mpl;

/* aggresive initialize */
template<typename T>
struct filter_t
{
	ptr::diff_t start;               /*can be negtive */
	ptr::diff_t end; 
	ptr::diff_t step;
	fn::function<bool(const T&)> pred;

	/* 2-value */
	CCDK_FORCEINLINE constexpr filter_t(ptr::diff_t inStart, ptr::diff_t inEnd) : start{ inStart }, end{ inEnd }, step{ 1 }, pred{} {}

	/* 3-value */
	CCDK_FORCEINLINE constexpr filter_t(ptr::diff_t inStart, ptr::diff_t inEnd, ptr::diff_t inStep) : start{ inStart }, end{ -1 }, step{ inStep }, pred{} {}

	/* 4-value */
	template<typename Fn>
	CCDK_FORCEINLINE filter_t(ptr::diff_t inStart, ptr::diff_t inEnd, ptr::diff_t inStep, Fn&& fn) : start{ inStart }, end{ -1 }, step{ inStep }, pred{util::forward<Fn>(fn)} {}

	CCDK_FORCEINLINE filter_t(filter_t&& other) : start{ other.start }, end{ other.end }, step{ other.step }, pred{ util::move(other.pred) } {}
};

template<typename Container>
struct filter_view_t
{
	typedef typename Container::value_type  value_type;
	typedef typename Container::filter_type filter_type;
	Container&  container;
	filter_type filter;

	filter_view_t(Container& ct, filter_type&& ft)  : container{ ct }, filter{ util::move(ft) }
	{
		if (filter.start < 0) { filter.start = container.size() + filter.start; }     /* allow negtive start*/
		if (filter.end < 0) { filter.end = container.size() + 1 + filter.end; }       /* allow negtive end */
		/* check step direction need same with start to end */
		ccdk_check_expr(
			((filter.step<0 && filter.start> filter.end && filter.start < container.size())
			|| (filter.step > 0 && filter.end > filter.start && filter.end <= container.size())),
			std::out_of_range{"invalid filter index at filter_view_t::filter_view_t"}
		);
	}
	

	/* container[{start, end, step, pred } ] = v */
	void operator=(value_type const& v)
	{
		if (filter.pred) { for (uint32 i = filter.start; i < filter.end; i += filter.step) { if (filter.pred(container.at(i))) container.at(i) = v; } }
		else { for (uint32 i = filter.start; i < filter.end; i += filter.step) { container[i] = v; } }
	}

	/* Container c = container[{start, end, step, pred } ] */
	void assign_to(Container& ct)
	{

	}
};

ccdk_namespace_ct_end