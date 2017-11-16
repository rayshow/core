#pragma once

#include<ccdk/mpl/base/integral_.h>

namespace ccdk
{
	namespace mpl
	{
		template<long long v> using llong_ = integer_<long long, v>;
		template<long long v> constexpr llong_<v> llong_c{};
	}
}