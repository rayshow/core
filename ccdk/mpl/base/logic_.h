#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/integer_.h>
#include<ccdk/mpl/base/bool_.h>
#include<ccdk/mpl/base/derive_if.h>

ccdk_namespace_mpl_start

/* default false */
template<typename ... Args> struct or_ :public false_ {};

/* have least one element is constexpr true */
template<typename T, typename... Args>
struct or_<T, Args...> : public derive_if<T, true_, or_<Args...>> {};

template<typename... Args> using or_t = typename or_<Args...>::type;
template<typename... Args> constexpr bool or_v = or_<Args...>::value;

/* default  false */
template<typename... Args> struct and_ :public true_ {};

/* only one element */
template<typename T> struct and_<T> :public T {};

/* at least 2-elements */
template<typename T1, typename T2, typename... Args>
struct and_<T1, T2, Args...> : public derive_if<T1, and_<T2, Args...>, false_ > {};

template<typename... Args> using and_t = typename and_<Args...>::type;
template<typename... Args> static constexpr bool and_v = and_<Args...>::value;


template<typename T> struct not_ : public bool_< !T::value > {};

ccdk_namespace_mpl_end