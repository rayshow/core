#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/mcontainer/mcontainer_fwd.h>

ccdk_namespace_mpl_start

template<typename T, typename U>
struct mul2_ : T::template mul<U>{};

template<typename T, typename... Args>
struct mul_;

template<typename T>
struct mul_<T> { typedef T type; };

template<typename T, typename U, typename... Args>
struct mul_<T,U,Args...> : mul_< typename mul2_<T, U>::type, Args...> {};

template<typename T, typename ... Args>
using mul_t_ = typename mul_<T, Args...>::type;

ccdk_namespace_mpl_end