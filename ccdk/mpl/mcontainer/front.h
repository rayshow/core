#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/mcontainer/mcontainer_fwd.h>

ccdk_namespace_mpl_start

template<typename Container> struct front_
{
	typedef typename Container::front type;
};

template<typename Container>
using front_t = typename front_<Container>::type;

template<typename... Args>
struct first_{};

template<typename T, typename... Args>
struct first_<T, Args...> { typedef T type; };

template<typename... Args>
using first_t = typename first_<Args...>::type;

ccdk_namespace_mpl_end