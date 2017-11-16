#pragma once

#include<ccdk/mpl/base/or_.h>
#include<ccdk/mpl/type_traits/is_float.h>
#include<ccdk/mpl/type_traits/is_integer.h>

namespace ccdk
{
	namespace mpl
	{
		//lazy
		template<typename T> struct is_arithmetic :
			public or_< is_float<T>, is_integer<T> > {};

		template<typename T> constexpr bool is_arithmetic_v = is_arithmetic<T>::value;
	}
}