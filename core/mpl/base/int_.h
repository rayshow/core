#pragma once

#include<core/mpl/base/integral_.h>

namespace core
{
	namespace mpl
	{
		template<int v> using int_ = integer_<int, v>;
		template<int v> constexpr int_<v> int_c{};
	}
}