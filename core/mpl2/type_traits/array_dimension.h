#pragma once

#include<core/mpl/int_.h>

namespace Aurora3D
{
	namespace mpl
	{
		template<typename T, int64 dim = 0> struct ArrayDim:public Int_<dim> {};
		template<typename T, int64 len, int64 dim> struct ArrayDim<T[len], dim> : public ArrayDim<T, dim + 1> {};
		template<typename T, int64 dim> struct ArrayDim<T[], dim> : public ArrayDim<T, dim+1> {};
		template<typename T> using  ArrayDim_t = typename ArrayDim<T>::type;
#define ArrayDimV(T)     ArrayDim<T>::value
	}

}