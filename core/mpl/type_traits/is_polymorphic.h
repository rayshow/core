#pragma once

#include<core/mpl/base/bool_.h>

namespace core
{
	namespace mpl
	{
		template<typename T> struct is_polymorphic :public bool_<__is_polymorphic(T)> {};
		template<typename T> constexpr bool is_polymorphic_v = is_polymorphic<T>::value;
	}
}