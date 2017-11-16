#pragma once

#include<ccdk/mpl/base/integral_.h>

namespace ccdk
{
	namespace mpl
	{
		template<unsigned long long v> using ullong_ = integer_<unsigned long long, v>;
		template<unsigned long long v> constexpr ullong_<v> ullong_c{};
	}
}