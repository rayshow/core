#pragma once

#include<core/mpl/logic_or.h>
#include<core/mpl/type_traits/is_float.h>
#include<core/mpl/type_traits/is_integral.h>

namespace Aurora3D
{
	namespace mpl
	{
		//lazy
		template<typename T> struct IsArithmetic :public Or< IsFloat<T>, IsIntegral<T> > {};
		
#define IsArithmeticV(T)     (IsArithmetic<T>::value)
#define NotArithmeticV(T)    (!IsArithmetic<T>::value)
	}
}