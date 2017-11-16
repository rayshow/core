#pragma once

#include<ccdk/mpl/base/integral_.h>

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

	}
}