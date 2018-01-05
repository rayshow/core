#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/logic_.h>

ccdk_namespace_mpl_start
		
/* template syntax suger */
template<bool> struct when_impl {};

using when = when_impl<true>;
		
template<bool condi> using match_default = when_impl<condi>;

template<typename T> using match = when_impl<T::value>;
template<typename T, typename... Args> using match_both = when_impl< and_<T,Args...>::value >;

/* match one condition of arg */
template<typename T, typename... Args> using match_one = when_impl< or_<T, Args...>::value >;

ccdk_namespace_mpl_end