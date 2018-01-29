#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/mcontainer/mcontainer_fwd.h>

ccdk_namespace_mpl_start

template<typename T, typename U>
struct div2_ : T::template div<U>{};

template<typename T, typename... Args>
struct div_;

template<typename T>
struct div_<T> { typedef T type; };

template<typename T, typename U, typename... Args>
struct div_<T,U,Args...> : div_< typename div2_<T, U>::type, Args...> {};

template<typename T, typename ... Args>
using div_t = typename div_<T, Args...>::type;

ccdk_namespace_mpl_end