#pragma once

#include<ccdk/mpl/base/integer_.h>

namespace ccdk
{
	namespace mpl
	{
		template<long long v> using int64_ = integer_<long long, v>;
		template<long long v> constexpr int64_<v> int64_{};
	}
}