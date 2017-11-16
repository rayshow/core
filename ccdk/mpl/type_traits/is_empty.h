#pragma once

#include<ccdk/mpl/base/bool_.h>

namespace ccdk
{
	namespace mpl
	{
		//only for class and struct ??
		template<typename T> struct is_empty :public bool_<__is_empty(T)> {};
		template<typename T> constexpr bool is_empty_v = is_empty<T>::value;
	}
}