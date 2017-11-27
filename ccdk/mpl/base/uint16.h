#pragma once

#include<ccdk/mpl/base/integer_.h>

namespace ccdk
{
	namespace mpl
	{
		template<unsigned short v> using uint16_ = integer_<unsigned short, v>;
		template<unsigned short v> constexpr uint16_<v> uint16_c{};
	}
}