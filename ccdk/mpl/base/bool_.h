#pragma once

#include<ccdk/mpl/base/integer_.h>

namespace ccdk
{
	namespace mpl
	{
		template<bool v> struct bool_ :public integer_<bool, v> {};

		template<bool v> bool_<v> bool_c{};

		using false_ = bool_<false>;
		using true_ = bool_<true>;
		
		constexpr false_ false_c{};
		constexpr true_  true_c{};

		//template<typename T, typename = void> struct is_false_ :false_ {};
		//template<typename T> struct is_false_< T, decltype((void)static_cast<false_>(declval<T>()))> :true_ {};
	}
}