#pragma once

#include<core/mpl/logic_or.h>
#include<core/mpl/type_traits/is_float.h>
#include<core/mpl/type_traits/is_integral.h>
#include<core/mpl/type_traits/is_void.h>
#include<core/mpl/type_traits/is_nullptr_t.h>

namespace Aurora3D
{
	namespace mpl
	{		
		//lazy
		template<typename T> struct IsFundamental :public Or < IsFloat<T>, IsIntegral<T>, IsVoid<T>, IsNullptrType<T> > {};
		
#define IsFundamentalV(T)   (IsFundamental<T>::value)
#define NotFundamentalV(T)  (!IsFundamental<T>::value)
	}
}