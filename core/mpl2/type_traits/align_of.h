#pragma once

#include<core/mpl/int_.h>

namespace Aurora3D
{
	namespace mpl
	{
		template<typename T> struct AlignOf :public Int_<alignof(T)>{};
	}
}