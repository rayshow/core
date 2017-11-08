#pragma once

#include<core/mpl/bool_.h>

namespace Aurora3D
{
	namespace mpl
	{
		template<typename T> struct IsAbstract :public Bool_<__is_abstract(T)> {};
		
#define IsAbstractV(T)    (IsAbstract<T>::value)
#define NotAbstractV(T)   (!IsAbstract<T>::value)
	}
}