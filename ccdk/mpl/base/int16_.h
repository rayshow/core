#pragma once

#include<ccdk/mpl/base/integer_.h>

namespace ccdk
{
	namespace mpl
	{
		template<short v> using int16_ = integer_<short, v>;
		template<short v> constexpr int16_<v> int16_c{};
	}
}