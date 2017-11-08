#pragma once

#include<core/mpl/bool_.h>

namespace Aurora3D
{
	namespace mpl
	{
		template<typename T> struct IsEnum:public Bool_<__is_enum(T)> {};
		
#define IsEnumV(T)   (IsEnum<T>::value)
#define NotEnumV(T)  (!IsEnum<T>::value)
	}
}