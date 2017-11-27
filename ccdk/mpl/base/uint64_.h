#pragma once

#include<ccdk/mpl/base/integer_.h>

namespace ccdk
{
	namespace mpl
	{
		template<unsigned long long v> using uint64_ = integer_<unsigned long long, v>;
		template<unsigned long long v> constexpr uint64_<v> uint64_c{};
	}
}