#pragma once

#include<core/mpl/bool_.h>
namespace Aurora3D
{
	namespace mpl
	{
		template<typename T> struct IsSigned :public Bool_< (T(0)>T(-1)) >{};
#define IsSignedV(T)   (IsSigned<T>::value)
#define NotSignedV(T)  (!IsSigned<T>::value)
	}
}