#pragma once

#include<core/type.h>
#include<core/mpl/int_.h>

namespace Aurora3D
{
	namespace mpl
	{
		// ~T
		template<typename T> struct BitNot :public T::bitnot {};
	}
}