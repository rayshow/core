#pragma once

#include<ccdk/mpl/base/type_.h>

namespace ccdk
{
	namespace mpl
	{
		template<typename T> struct is_volatile:  false_{};
		template<typename T> struct is_volatile<T volatile>: true_{};

		template<typename T> constexpr bool is_volatile_v = is_volatile<T>::value;

		template<typename T> struct is_top_volatile : false_ {};
		template<typename T> struct is_top_volatile<volatile T&> :true_ {};
		template<typename T> struct is_top_volatile<volatile T*> :true_ {};

		template<typename T> constexpr bool is_top_volatile_v
			= is_top_volatile<T>::value;
	}
}