#pragma once

#include<core/mpl/base/bool_.h>

namespace core
{
	namespace mpl
	{
		template<typename T> struct is_abstract :public bool_<__is_abstract(T)> {};
		template<typename T> constexpr bool is_abstract_v = is_abstract<T>::value;
	}
}