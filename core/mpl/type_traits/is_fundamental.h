#pragma once

#include<core/mpl/base/or_.h>
#include<core/mpl/type_traits/is_float.h>
#include<core/mpl/type_traits/is_integer.h>
#include<core/mpl/type_traits/is_void.h>
#include<core/mpl/type_traits/is_nullptr_t.h>

namespace core
{
	namespace mpl
	{		
		template<typename T> struct is_fundamental :
			public or_< is_float<T>, is_integer<T>, is_void<T>, is_nullptr_t<T> > {};

		template<typename T> constexpr bool is_fundamental_v = is_fundamental<T>::value;
	}
}