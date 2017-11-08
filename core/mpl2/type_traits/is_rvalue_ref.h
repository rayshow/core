#pragma once


#include<core/mpl/bool_.h>

namespace Aurora3D
{
	namespace mpl
	{
		//lazy
		template<typename T> struct IsRValueRef : public False_ {};
		template<typename T> struct IsRValueRef<T&&> :public True_ {};
#define IsRValueRefV(T)   (IsRValueRef<T>::value)
#define NotRValueRefV(T)  (!IsRValueRef<T>::value)
	}
}