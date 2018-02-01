#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/preprocessor/seq_foreach_tuple.h>
#include<ccdk/mpl/base/apply_.h>
#include<ccdk/mpl/base/fold_args.h>
#include<ccdk/mpl/base/type_.h>

ccdk_namespace_mpl_start

/* add */
template<typename T, typename U>
struct add2_ : T::template add_<U>{};

template<typename T, typename U>
using add2_t = typename add2_<T, U>::type;

template<typename T, typename... Args>
struct add_ : fold_args< add2_, T,Args...> {};

template<typename T, typename ... Args>
using add_t = typename add_<T, Args...>::type;

template<typename T, typename ... Args>
static constexpr auto add_v = add_<T, Args...>::value;

template< typename T, T first, T... args>
struct add_c : compile_t<T, first + add_c<T, args...>::value> {};

template<typename T, T first>
struct add_c<T,first> : compile_t<T, first> {};

template<typename T, T first, T... args>
using add_ct = typename add_c<T, first, args...>::type;

template<typename T, T... args>
static constexpr T add_cv = add_c<T, args...>::value;


#define CCDK_M_BINARY_ARITHMATIC_OP( name, sign,...)                                                                        \
template<typename T, typename U> struct name ## 2_ : T::template name ## _<U>{};                                            \
template<typename T, typename U> using  name ## 2_t = typename name ## 2_<T, U>::type;                                      \
template<typename T, typename... Args> struct name ## _ : fold_args< name ## 2_, T,Args...> {};                            \
template<typename T, typename... Args> using  name ##_t = typename name ## _<T, Args...>::type;                             \
template<typename T, typename... Args> static constexpr auto name ## _v = name ## _<T, Args...>::value;                    \
template<typename T, T first, T... args> struct name ## _c : compile_t<T, (first sign name ## _c<T, args...>::value)> {};   \
template<typename T, T first> struct name ## _c<T,first> : compile_t<T, first> {};                                          \
template<typename T, T... args> static constexpr T name ## _cv = name ## _c<T, args...>::value;

/* other binary arithmatic operation */
CCDK_PP_FOREACH_TUPLE(CCDK_M_BINARY_ARITHMATIC_OP, ((sub, -), (mul, *), (div, / ), (mod, %), (bitand, &), (bitor , | ), (shl, << ), (shr, >> )), );

template<typename T> struct reverse_  : T::reverse_ {};
template<typename T> struct negative_ : T::negative_ {};
template<typename T> struct positive_ : T::positive_ {};
template<typename T> struct dec_ : T::dec_ {};
template<typename T> struct inc_ : T::inc_ {};


ccdk_namespace_mpl_end