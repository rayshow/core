#pragma once

#include<core/mpl/int_.h>

namespace Aurora3D
{
	namespace mpl
	{
		template<typename T> struct ArrayLen :public Int_<0> { };
		template<typename T, int64 len> struct ArrayLen<T[len]>: public Int_<len>{};
		template<typename T> using  ArrayLen_t = typename ArrayLen<T>::type;
#define ArrayLenV(T)     (ArrayLen<T>::value)
	}
}