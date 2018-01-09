#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/mcontainer/mcontainer_fwd.h>

ccdk_namespace_mpl_start

template<typename Container> struct clear_
{
	typedef typename Container::clear type;
};

ccdk_namespace_mpl_end