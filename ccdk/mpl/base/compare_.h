#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/type_.h>

ccdk_namespace_mpl_start

/*  equal */
template<typename T, typename U> struct equal_ : T::template equal<U>{};
template<typename T, typename U> static constexpr bool equal_v = equal_<T, U>::value;

/* less */
template<typename T, typename U> struct less_ : T::template less<U>{};
template<typename T, typename U> static constexpr bool less_v = less_<T, U>::value;

/* not equal */
template<typename T, typename U> struct nequal_ : bool_< !equal_v<T, U>> {};
template<typename T, typename U> static constexpr bool nequal_v = !equal_v<T, U>;

/* greater */
template<typename T, typename U> struct greater_ : less_<U,T>{};
template<typename T, typename U> static constexpr bool greater_v = greater_<T, U>::value;

/* lequal */
template<typename T, typename U> struct lequal_ : bool_< !less_v<U, T> > {};
template<typename T, typename U> static constexpr bool lequal_v = lequal_<T, U>::value;

/* gequal */
template<typename T, typename U> struct gequal_ : bool_< !less_v<T, U> > {};
template<typename T, typename U> static constexpr bool gequal_v = lequal_<T, U>

ccdk_namespace_mpl_end