#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/type_.h>

ccdk_namespace_mpl_start

/* abstract binary operation */
template<typename T, typename BinaryOp, T first, T... args>
struct exp_arithmatic_ : 
	BinaryOp::template apply< T, first, exp_arithmatic_<T, BinaryOp, args...>::value >
{};

template<typename T, typename BinaryOp, T first>
struct exp_arithmatic_<T, BinaryOp, first>
{
	static constexpr T value = first;
};



/* max args... */
struct exp_max2
{
	template<typename T, T a, T b>
	struct apply
	{
		static constexpr T value = a < b ? b : a;
	};
};
template< typename T, T... args>
struct exp_max_ : exp_arithmatic_<T, exp_max2, args...> {};

template< typename T, T... args>
static constexpr T exp_max_v = exp_max_<T, args...>::value;

/* min args... */
struct exp_min2
{
	template<typename T, T a, T b>
	struct apply
	{
		static constexpr T value = a < b ? a : b;
	};
};

template< typename T, T... args>
struct exp_min_ :exp_arithmatic_<T, exp_min2, args...> {};

template< typename T, T... args>
static constexpr T exp_min_v = exp_min_<T, args...>::value;

ccdk_namespace_mpl_end