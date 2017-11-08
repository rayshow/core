#pragma once

#include<core/type.h>
#include<core/mpl/int_.h>

namespace Aurora3D
{
	namespace mpl
	{
		//T1 << T2
		template<typename T1, typename T2> struct Shl :public T1::template shl<T2> {};
	}
}