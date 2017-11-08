#pragma once
#include<core/mpl/bool_.h>
#include<core/mpl/bit_not.h>

namespace Aurora3D
{
	namespace mpl
	{
		//T1 == T2
		template<typename T1, typename T2> struct Equal:public T1::template equal<T2>{};

		//T1 != T2
		template<typename T1, typename T2> struct NotEqual :public Equal<T1,T2>::not {};

	}
}