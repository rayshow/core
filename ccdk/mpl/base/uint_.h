#pragma once

#include<ccdk/mpl/base/integer_.h>

namespace ccdk
{
	namespace mpl
	{
		template<unsigned int v> using uint_ = integer_<unsigned int, v>;
		template<unsigned int v> constexpr uint_<v> uint_c{};
	}
}