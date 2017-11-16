#pragma once

#include<ccdk/mpl/base/bool_.h>

namespace ccdk
{
	namespace mpl
	{
		template<typename T> struct is_const: public false_{};
		template<typename T> struct is_const<T const>: public true_{};

		template<typename T> constexpr bool is_const_v = is_const<T>::value;
	}
}