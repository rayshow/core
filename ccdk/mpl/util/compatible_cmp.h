#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/type_traits/remove_ref.h>
#include<ccdk/mpl/type_traits/compatible_type.h>

ccdk_namespace_mpl_util_start

//T&& forward string literial will become char array ref
//  int literial will become int&
template<typename T1, typename T2>
struct compatible_less
{
	typedef compatible_type_t<T1, T2> comp_t;

	bool operator()(const comp_t& v1, const comp_t& v2)
	{
		return v1 < v2;
	}
};


ccdk_namespace_mpl_util_end