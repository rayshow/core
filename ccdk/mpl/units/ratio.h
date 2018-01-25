#pragma once

#include<ccdk/mpl/mpl_module.h>

ccdk_namespace_mpl_units_start

namespace detail
{
	constexpr int64 gcd(int64 v1, int64 v2)
	{
		int64 left = 0;
		while (v2)
		{
			left = v1 % v2;
			v1 = v2;
			v2 = left;
		}
		return v1;
	}

	constexpr int64 lcm(int64 v1, int64 v2)
	{
		return v1 * v2 / gcd(v1, v2);
	}

	constexpr int64 abs(int64 v)
	{

	}
}





template< int64 N, int64 D>
struct ratio
{

};

ccdk_namespace_mpl_units_end