#pragma once

#include<ccdk/container/container_mudule.h>
#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/mcontainer/make_indice.h>
#include<ccdk/mpl/function/function.h>
#include<ccdk/mpl/fusion/maybe.h>

ccdk_namespace_ct_start

using namespace mpl;


template<typename T>
struct filter_t
{
	ptr::size_t start;
	ptr::size_t end;
	fs::maybe< ptr::size_t > step;
	fn::function<bool(const T&)> pred;
};

template<typename Container>
struct filter_view_t
{
	typedef typename Container::value_type  value_type;
	typedef typename Container::filter_type filter_type;
	Container& container;
	filter_type filter;

	/* container[{start, end, step, pred } ] = v */
	void operator=(value_type const& v)
	{
		/* is step initalized ? */
		uint32 step = filter.step ? filter.step.value() : 1;
		if (filter.pred) { for (uint32 i = filter.start; i < filter.end; i += step) { if (filter.pred(container[i])) container[i] = v; } }
		else { for (uint32 i = filter.start; i < filter.end; i += step) { container[i] = v; } }
	}

	/* Container c = container[{start, end, step, pred } ] */
	void assign_to(Container& ct)
	{

	}

};

ccdk_namespace_ct_end