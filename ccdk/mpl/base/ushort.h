#pragma once

#include<ccdk/mpl/base/integral_.h>

namespace ccdk
{
	namespace mpl
	{
		template<unsigned short v> using ushort_ = integer_<unsigned short, v>;
		template<unsigned short v> constexpr ushort_<v> ushort_c{};
	}
}