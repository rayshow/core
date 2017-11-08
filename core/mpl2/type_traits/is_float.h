#pragma once

#include<core/mpl/bool_.h>
#include<core/type.h>

namespace Aurora3D
{
	namespace mpl
	{
		template<typename T> struct IsFloat :public False_{};
		template<> struct IsFloat<float> :public  True_ {};
		template<> struct IsFloat<double> :public True_ {};
		template<> struct IsFloat<hfloat> :public True_ {};
		template<typename T> struct IsFloat<const T> :public IsFloat<T> {};
		template<typename T> struct IsFloat<volatile T> :public IsFloat<T> {};
		template<typename T> struct IsFloat<const volatile T> :public IsFloat<T> {};
		
#define IsFloatV(T)   (IsFloat<T>::value)
#define NotFloatV(T)  (!IsFloat<T>::value)
	}
}
