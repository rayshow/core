#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/integer_.h>

ccdk_namespace_mpl_start

template<typename... Args> struct add_;

template<typename T> struct add_<T> :public T {};

template<typename T, typename... Args> struct add_<T, Args...> : public int32_<T::value + add_<Args...>::value> {};

template<typename T>
constexpr T max2(const T& t1, const T& t2)
{
	return t1 < t2 ? t2 : t1;
}

template<typename T>
constexpr T min2(const T& t1, const T& t2)
{
	return t1 < t2 ? t1 : t2;
}


ccdk_namespace_mpl_end