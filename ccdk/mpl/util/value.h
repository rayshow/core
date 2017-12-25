#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/util/move.h>

ccdk_namespace_mpl_util_start

//general implements
template<typename T>
struct get_value_t
{
	decltype(auto) operator()(const T& t) const
	{
		return t.value;
	}
};

template<typename T> constexpr get_value_t<T> value{};

ccdk_namespace_mpl_util_end