#pragma once

#include<core/mpl/base/integral_.h>

namespace core
{
	namespace mpl
	{
		template<unsigned int v> using uint_ = integer_<unsigned int, v>;
		template<unsigned int v> constexpr uint_<v> uint_c{};
	}
}