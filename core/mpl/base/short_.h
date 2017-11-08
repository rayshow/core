#pragma once

#include<core/mpl/base/integral_.h>

namespace core
{
	namespace mpl
	{
		template<short v> using short_ = integer_<short, v>;
		template<short v> constexpr short_<v> short_c{};
	}
}