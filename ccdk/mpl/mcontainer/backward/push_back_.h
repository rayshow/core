#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/mcontainer/mcontainer_fwd.h>

ccdk_namespace_mpl_start

template<typename Container, typename P>
struct push_back_: Container::template push_back<P> {};

template<typename Container, typename P>
using push_back_t_ = typename push_back_<Container, T>::type;

ccdk_namespace_mpl_end