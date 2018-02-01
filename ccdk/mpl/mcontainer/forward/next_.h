#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/mcontainer/mcontainer_fwd.h>

ccdk_namespace_mpl_start

template<typename It> struct next_
{
	typedef typename It::next type;
};

template<typename It> using next_t_ = typename next_<It>::type;

ccdk_namespace_mpl_end