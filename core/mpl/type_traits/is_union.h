#pragma once

#include<core/mpl/base/bool_.h>

namespace core
{
	namespace mpl
	{
		template<typename T> struct is_union : bool_<__is_union(T)> {};
		template<typename T> constexpr bool is_union_v = is_union<T>::value;

	}
}