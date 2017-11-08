#pragma once

#include<core/mpl/base/integral_.h>

namespace core
{
	namespace mpl
	{
		template<long long v> using llong_ = integer_<long long, v>;
		template<long long v> constexpr llong_<v> llong_c{};
	}
}