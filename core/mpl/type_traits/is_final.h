#pragma once

#include<core/mpl/base/bool_.h>

namespace core
{
	namespace mpl
	{
		template<typename T> struct is_final :public bool_<__is_final(T)> {};
		template<typename T> constexpr bool is_final_v = is_final<T>::value;
	}
}