#pragma once

#include<core/mpl/base/bool_.h>

namespace core
{
	namespace mpl
	{
		template<typename T> struct is_enum:public bool_<__is_enum(T)> {};
		template<typename T> constexpr bool is_enum_v = is_enum<T>::value;
	}
}