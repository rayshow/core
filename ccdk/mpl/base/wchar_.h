#pragma once

#include<ccdk/mpl/base/integer_.h>

namespace ccdk
{
	namespace mpl
	{
		template<wchar_t c> using wchar_ = integer_<wchar_t, c>;
		template<wchar_t c> constexpr wchar_<c> wchar_c{};
	}
}