#pragma once

#include<ccdk/mpl/base/integer_.h>

namespace ccdk
{
	namespace mpl
	{
		template<char c> using char_ = integer_<char, v>;
		template<char c> constexpr char_<c> char_c{};
	}
}