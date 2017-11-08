#pragma once

#include<core/mpl/bool_.h>

namespace Aurora3D
{
	namespace mpl
	{
		template<typename T> struct IsClass:public Bool_<__is_class(T)>{};
		
#define IsClassV(T)         (IsClass<T>::value)
#define NotClassOfV(T)      (!IsClass<T>::value)
	}
}