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
		};
	}
}