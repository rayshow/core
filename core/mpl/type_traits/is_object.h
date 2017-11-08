#pragma once

#include<core/mpl/base/or_.h>
#include<core/mpl/type_traits/is_scalar.h>
#include<core/mpl/type_traits/is_union.h>
#include<core/mpl/type_traits/is_class.h>
#include<core/mpl/type_traits/is_array.h>

namespace core
{
	namespace mpl
	{
		template<typename T> struct is_object :
			public or_< is_scaler<T>, is_union<T>, is_class<T>, is_array<T>> {};

		template<typename T>  constexpr bool is_object_v = is_object<T>::value;
	}
}