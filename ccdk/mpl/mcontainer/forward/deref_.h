#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/mcontainer/mcontainer_fwd.h>

ccdk_namespace_mpl_start

template<typename It> struct deref_
{
	typedef typename It::deref type;
};

template<typename It> using deref_t_ = typename deref_<It>::type;

ccdk_namespace_mpl_end