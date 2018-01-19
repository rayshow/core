#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/integer_.h>

ccdk_namespace_mpl_start

/* abstract binary operation */
template<typename T, typename BinaryOp, T first, T... args>
struct arithmatic : BinaryOp::template apply< T, first, arithmatic<T, BinaryOp, args...>::value >
{};

template<typename T, typename BinaryOp, T first>
struct arithmatic<T, BinaryOp, first>
{
	static constexpr T value = first;
};

/* add args... */
struct add2
{
	template<typename T, T a, T b>
	struct apply
	{
		static constexpr T value = a + b;
	};
};

template< typename T, T... args>
struct add_ :arithmatic<T, add2, args...> {};

/* max args... */
struct max2
{
	template<typename T, T a, T b>
	struct apply
	{
		static constexpr T value = a < b ? b : a;
	};
};
template< typename T, T... args>
struct max_ :arithmatic<T, max2, args...> {};

template< typename T, T... args>
static constexpr T max_v = max_<T, args...>::value;

/* min args... */
struct min2
{
	template<typename T, T a, T b>
	struct apply
	{
		static constexpr T value = a < b ? a : b;
	};
};
template< typename T, T... args>
struct min_ :arithmatic<T, min2, args...> {};

template< typename T, T... args>
static constexpr T min_v = min_<T, args...>::value;

ccdk_namespace_mpl_end