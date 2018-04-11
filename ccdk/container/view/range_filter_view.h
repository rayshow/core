#pragma once

#include<ccdk/container/container_mudule.h>
#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/mcontainer/val_pack.h>
#include<ccdk/mpl/function/function.h>
#include<ccdk/mpl/fusion/maybe.h>

ccdk_namespace_ct_start

using namespace mpl;

template<typename Container>
struct range_filter_view
{
	using value_type  = typename Container::value_type;
	using filter_type = typename Container::filter_type;
	using this_type = range_filter_view;

	Container&  container;
	filter_type filter;

	this_type(Container& ct, filter_type&& ft)
		: container{ ct }, filter{ util::move(ft) }
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
	void operator=(value_type const& v) noexcept{
		if (filter.pred) {  
			for (uint32 i = filter.start; i < filter.end; i += filter.step) { 
				if (filter.pred(container.at(i))) container.at(i) = v;
			}
		} else {
			for (uint32 i = filter.start; i < filter.end; i += filter.step) { 
				container.at(i) = v; 
			} 
		}
	}

	/* container[{start, end, step, pred } ] = generator */
	void operator=(fn::function<value_type(value_type)> gen) {
		if (filter.pred) { 
			for (uint32 i = filter.start; i < filter.end; i += filter.step) { 
			if (filter.pred(container.at(i))) 
				container.at(i) = gen(container.at(i)); }
		}
		else { 
			for (uint32 i = filter.start; i < filter.end; i += filter.step) { 
				container.at(i) = gen(container.at(i));
			} 
		}
	}

	/* Container c = container[{start, end, step, pred } ] */
	void assign_to(Container& ct)
	{

	}
};

ccdk_namespace_ct_end