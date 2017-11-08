#pragma once

#include<core/mpl/bool_.h>

namespace Aurora3D
{
	namespace mpl
	{
		template<typename T1, typename T2> struct LessEqual : T1::template less_eq<T2>{};
	}
}