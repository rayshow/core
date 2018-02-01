#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/mcontainer/mcontainer_fwd.h>

ccdk_namespace_mpl_start

template<typename Container>
struct pop_front_
{
	typedef typename Container::pop_front type;
};

template<typename Container, uint32 N>
struct pop_front_n_ :pop_front_n_< typename pop_front_<Container>::type, N-1 >
{};

template<typename Container>
struct pop_front_n_< Container, 0>
{
	typedef Container type;
};



ccdk_namespace_mpl_end