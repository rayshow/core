#pragma once

#include<core/mpl/base/size_.h>

namespace core
{
	namespace mpl
	{
		template<typename T> struct align_of :public size_<alignof(T)>{};
	}
}