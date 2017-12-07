#pragma once

#include<ccdk/type.h>

namespace ccdk
{
	namespace mpl
	{
		template<typename T>
		struct range
		{
			T start;
			T end;

			constexpr range()
				: start{}, end{}
			{}

			constexpr range(const T& s, const T& e)
				: start{ s },
				end{ e }
			{}



		};
	}
}