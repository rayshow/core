#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/type_.h>

ccdk_namespace_mpl_start

template<typename T> struct identity { typedef T type; };
template<typename T> using identity_t = typename identity<T>::type;

ccdk_namespace_mpl_end