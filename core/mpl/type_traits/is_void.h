#pragma once

#include<core/mpl/base/bool_.h>

namespace core
{
	namespace mpl
	{
		template<typename T> struct is_void: public false_{};
		template<> struct is_void<void>:public true_{};
		template<> struct is_void<void const> :public true_ {};
		template<> struct is_void<void volatile> :public true_ {};
		template<> struct is_void<void const volatile> :public true_ {};

		template<typename T>  constexpr bool  is_void_v = is_void<T>::value;
	}
}