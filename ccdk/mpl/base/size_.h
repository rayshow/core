#pragma once

#include<ccdk/mpl/base/integral_.h>
#include<ccdk/type.h>

namespace ccdk
{
	namespace mpl
	{

		template<size_t v> using size_ = integer_<size_t, v>;
		template<size_t v> constexpr size_<v> size_c{};
	}
}