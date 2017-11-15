#pragma once

#include<core/mpl/base/integral_.h>
#include<core/type.h>

namespace core
{
	namespace mpl
	{

		template<size_t v> using size_ = integer_<size_t, v>;
		template<size_t v> constexpr size_<v> size_c{};
	}
}