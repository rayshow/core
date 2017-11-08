#pragma once

#include<core/mpl/bool_.h>

namespace Aurora3D
{
	namespace mpl
	{
		template<typename T> struct IsPolymorphic :public Bool_<__is_polymorphic(T)> {};
#define IsPolymorphicV(T)   (IsPolymorphic<T>::value)
#define NotPolymorphicV(T)  (!IsPolymorphic<T>::value)
	}
}