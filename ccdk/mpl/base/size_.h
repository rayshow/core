#pragma once

#include<ccdk/mpl/base/integer_.h>
#include<ccdk/type.h>

namespace ccdk
{
	namespace mpl
	{

		template<ptr::size_t v > using size_ = integer_<ptr::size_t, v>;
		template<ptr::size_t v> constexpr size_<v> size_c{};
	}
}