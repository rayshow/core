#pragma once

#include<core/mpl/base/bool_.h>

namespace core
{
	namespace mpl
	{
		template<typename T> struct is_empty :public bool_<__is_empty(T)> {};
		template<typename T> constexpr bool is_empty_v = is_empty<T>::value;
	}
}