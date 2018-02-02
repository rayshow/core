#pragma once

#include<ccdk/mpl/base/type_.h>
#include<ccdk/mpl/type_traits/traits_case.h>

namespace ccdk
{
	namespace mpl
	{
		template<typename T> struct is_class:public bool_<__is_class(T)>{};

		template<typename T> constexpr bool is_class_v = is_class<T>::value;
		
		//for static dispatch
		CCDK_TT_SINGLE_RANK(is_class);

	}
}