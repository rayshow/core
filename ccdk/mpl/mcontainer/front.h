#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/mcontainer/mcontainer_fwd.h>

ccdk_namespace_mpl_start

template<typename Container> struct front  
{
	typedef Container::front type;
};

template<typename Container>
using front_t = typename front<Container>::type;

ccdk_namespace_mpl_end