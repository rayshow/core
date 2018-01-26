#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/mcontainer/mcontainer_fwd.h>

ccdk_namespace_mpl_start

template<typename T, typename U>
struct add2_ : T::template add<U>{};

template<typename T, typename... Args>
struct add_;

template<typename T>
struct add_<T> { typedef T type; };

template<typename T, typename U, typename... Args>
struct add_<T,U,Args...> : add_< typename add2_<T, U>::type, Args...> {};

template<typename T, typename ... Args>
using add_t = typename add_<T, Args...>::type;

ccdk_namespace_mpl_end