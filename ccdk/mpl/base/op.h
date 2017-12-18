#pragma once

#include<ccdk/type.h>

namespace ccdk
{
	namespace mpl
	{
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

	}
}