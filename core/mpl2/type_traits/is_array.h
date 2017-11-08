#pragma once

#include<core/mpl/bool_.h>

namespace Aurora3D
{
	namespace mpl
	{
		template<typename T> struct IsArray : public False_ {};
		template<typename T> struct IsArray<T[]> :public True_ {};
		template<typename T, unsigned int N> struct IsArray<T[N]> :public True_ {};

#define IsArrayV(T)     (IsArray<T>::value)
#define NotArrayV(T)    (!IsArray<T>::value)
	}
}