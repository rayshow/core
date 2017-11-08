#pragma once

#include<core/mpl/bool_.h>

namespace Aurora3D
{
	namespace mpl
	{
		template<typename T> struct IsUnsigned :public Bool_ < T(0) < T(-1) > {};
#define IsUnsignedV(T)   (IsUnsigned<T>::value)
#define NotUnSignedV(T)  (!IsUnsigned<T>::value)
	}
}