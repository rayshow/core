#pragma once

#include<ccdk/type.h>

namespace ccdk
{
	namespace mpl
	{
		struct range
		{
			ptr::size_t start = 0;
			ptr::size_t end = 1;

		};
	}
}