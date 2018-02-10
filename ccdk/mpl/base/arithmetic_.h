#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/preprocessor/seq_foreach_tuple.h>
#include<ccdk/mpl/base/apply_.h>
#include<ccdk/mpl/base/fold_args.h>
#include<ccdk/mpl/base/type_.h>

ccdk_namespace_mpl_start


/* min_ */
template<typename T, T First, T Second>
struct  min_c2 : compile_t<T, (First < Second ? First : Second)>{};

template< typename T, T First, T... Args>
struct min_c : min_c2<T, First, min_c<T, Args...>::value> {};

template<typename T, T First>
struct min_c<T, First> : compile_t<T, First> {};

template<typename T, T First, T... Args>
static constexpr T min_val = min_c<T, First, Args...>::value;

template<typename T, typename U>
struct min2_ : T::template min<U>{};

template<typename T, typename... Args>
struct min_ : reverse_fold_args< min2_, T, Args...> {};

template<typename T, typename ... Args>
using min_t = typename min_<T, Args...>::type;

/* max_ */
template<typename T, T First, T Second>
struct  max_c2 : compile_t<T, (First > Second ? First : Second)>{};

template< typename T, T First, T... Args>
struct max_c : max_c2<T, First, max_c<T, Args...>::value> {};

template<typename T, T First>
struct max_c<T, First> : compile_t<T, First> {};

template<typename T, T First, T... Args>
static constexpr T max_val = max_c<T, First, Args...>::value;

template<typename T, typename U>
struct max2_ : T::template max<U>{};

template<typename T, typename... Args>
struct max_ : reverse_fold_args< max2_, T, Args...> {};

template<typename T, typename ... Args>
using max_t = typename max_<T, Args...>::type;

#define CCDK_MPL_BINARY_ARITHMATIC_OP( name, sign,...)                                                                      \
template<typename T, typename U> struct name ## 2_ : T::template name<U>{};                                                 \
template<typename T, typename... Args> struct name ## _ : reverse_fold_args< name ## 2_, T,Args...> {};                     \
template<typename T, typename... Args> using  name ##_t = typename name ## _<T, Args...>::type;                             \
template<typename T, typename... Args> static constexpr auto name ## _v = name ## _<T, Args...>::value;                     \
template<typename T, T First, T... Args> struct name ## _c : compile_t<T, (First sign name ## _c<T, Args...>::value)> {};   \
template<typename T, T First> struct name ## _c<T,First> : compile_t<T, First> {};                                          \
template<typename T, T... Args> static constexpr T name ## _val = name ## _c<T, Args...>::value;

/* other binary arithmatic operation */
CCDK_PP_FOREACH_TUPLE(CCDK_MPL_BINARY_ARITHMATIC_OP, ((add,+), (sub, -), (mul, *), (div, / ), (mod, %), (band, &), (bor , | ), (shl, << ), (shr, >> )), );

ccdk_namespace_mpl_end