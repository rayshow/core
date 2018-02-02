#pragma once

#include<ccdk/mpl/base/type_.h>

namespace ccdk
{
	namespace mpl
	{
		template<typename T> struct is_enum:public bool_<__is_enum(T)> {};
		template<typename T> constexpr bool is_enum_v = is_enum<T>::value;
	}
}