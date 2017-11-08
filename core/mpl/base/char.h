#pragma once

#include<core/mpl/base/integral_.h>

namespace core
{
	namespace mpl
	{
		template<char c> using char_ = integer_<char, v>;
		template<char c> constexpr char_<c> char_c{};
	}
}