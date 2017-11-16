#pragma once

#include<ccdk/mpl/base/size_.h>

namespace ccdk
{
	namespace mpl
	{
		template<typename T> struct align_of :public size_<alignof(T)>{};
	}
}