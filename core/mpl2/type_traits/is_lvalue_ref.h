#pragma once


#include<core/mpl/bool_.h>

namespace Aurora3D
{
	namespace mpl
	{
		//lazy
		template<typename T> struct IsLValueRef : public False_ {};
		template<typename T> struct IsLValueRef<T&> :public True_ {};

#define IsLValueRefV(T)   (IsLValueRef<T>::value)
#define NotLValueRefV(T)  (!IsLValueRef<T>::value)
	}
}