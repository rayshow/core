#pragma once

#include<core/mpl/bool_.h>

namespace Aurora3D
{
	namespace mpl
	{
		template<typename T> struct IsFinal :public Bool_<__is_final(T)> {};
		
#define IsFinalV(T)   (IsFinal<T>::value)
#define NotFinalV(T)  (!IsFinal<T>::value)
	}
}