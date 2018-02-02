#pragma once

#include<ccdk/mpl/base/type_.h>

namespace ccdk
{
	namespace mpl
	{
		//non-union class with at least one virtual function
		template<typename T> struct is_polymorphic :public bool_<__is_polymorphic(T)> {};
		template<typename T> constexpr bool is_polymorphic_v = is_polymorphic<T>::value;
	}
}