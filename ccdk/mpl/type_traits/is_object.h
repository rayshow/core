#pragma once

#include<ccdk/mpl/base/or_.h>
#include<ccdk/mpl/type_traits/is_scalar.h>
#include<ccdk/mpl/type_traits/is_union.h>
#include<ccdk/mpl/type_traits/is_class.h>
#include<ccdk/mpl/type_traits/is_array.h>

namespace ccdk
{
	namespace mpl
	{
		template<typename T> struct is_object :
			public or_< is_scaler<T>, is_union<T>, is_class<T>, is_array<T>> {};

		template<typename T>  constexpr bool is_object_v = is_object<T>::value;

		
	}
}