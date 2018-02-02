#pragma once

#include<ccdk/mpl/base/type_.h>

namespace ccdk
{
	namespace mpl
	{
		template<typename T> struct is_const:  false_{};
		template<typename T> struct is_const<T const>: true_{};

		template<typename T> constexpr bool is_const_v = is_const<T>::value;

		template<typename T> struct is_top_const : false_ {};
		template<typename T> struct is_top_const<const T&> :true_ {};
		template<typename T> struct is_top_const<const T*> :true_ {};

		template<typename T> constexpr bool is_top_const_v = is_top_const<T>::value;
	}
}