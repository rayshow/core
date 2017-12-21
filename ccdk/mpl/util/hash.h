#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/util/move.h>

ccdk_namespace_mpl_util_start

//general implements
template<typename T>
struct hash_t
{
	ptr::size_t operator()(const T& t) const
	{
		return 0;
	}
};

template<typename T> constexpr hash_t<T> hash{};

ccdk_namespace_mpl_util_end