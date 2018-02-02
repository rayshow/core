#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/mcontainer/mcontainer_fwd.h>

ccdk_namespace_mpl_start

template<typename Container> struct begin_
{
	typedef typename Container::begin type;
};

template<typename Container>
using begin_t = typename begin_<Container>::type;

ccdk_namespace_mpl_end