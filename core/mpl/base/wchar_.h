#pragma once

#include<core/mpl/base/integral_.h>

namespace core
{
	namespace mpl
	{
		template<wchar_t c> using wchar_ = integer_<wchar_t, c>;
		template<wchar_t c> constexpr wchar_<c> wchar_c{};
	}
}