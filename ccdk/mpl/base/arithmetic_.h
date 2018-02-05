#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/preprocessor/seq_foreach_tuple.h>
#include<ccdk/mpl/base/apply_.h>
#include<ccdk/mpl/base/fold_args.h>
#include<ccdk/mpl/base/type_.h>

ccdk_namespace_mpl_start

/* add */
template<typename T, typename U>
struct add2_ : T::template add<U>{};

template<typename T, typename U>
using add2_t = typename add2_<T, U>::type;

template<typename T, typename... Args>
struct add_ : reverse_fold_args< add2_, T, Args...> {};

template<typename T, typename ... Args>
using add_t = typename add_<T, Args...>::type;

template<typename T, typename ... Args>
static constexpr auto add_v = add_<T, Args...>::value;

template< typename T, T First, T... Args>
struct add_c : compile_t<T, First + add_c<T, Args...>::value> {};

template<typename T, T First>
struct add_c<T,First> : compile_t<T, First> {};

template<typename T, T First, T... Args>
using add_ct = typename add_c<T, First, Args...>::type;

template<typename T, T... Args>
static constexpr T add_cv = add_c<T, Args...>::value;


#define CCDK_MPL_BINARY_ARITHMATIC_OP( name, sign,...)                                                                      \
template<typename T, typename U> struct name ## 2_ : T::template name<U>{};                                                 \
template<typename T, typename U> using  name ## 2_t = typename name ## 2_<T, U>::type;                                      \
template<typename T, typename... Args> struct name ## _ : reverse_fold_args< name ## 2_, T,Args...> {};                             \
template<typename T, typename... Args> using  name ##_t = typename name ## _<T, Args...>::type;                             \
template<typename T, typename... Args> static constexpr auto name ## _v = name ## _<T, Args...>::value;                     \
template<typename T, T First, T... Args> struct name ## _c : compile_t<T, (First sign name ## _c<T, Args...>::value)> {};   \
template<typename T, T First> struct name ## _c<T,First> : compile_t<T, First> {};                                          \
template<typename T, T... Args> static constexpr T name ## _cv = name ## _c<T, Args...>::value;

/* other binary arithmatic operation */
CCDK_PP_FOREACH_TUPLE(CCDK_MPL_BINARY_ARITHMATIC_OP, ((sub, -), (mul, *), (div, / ), (mod, %), (band, &), (bor , | ), (shl, << ), (shr, >> )), );


ccdk_namespace_mpl_end