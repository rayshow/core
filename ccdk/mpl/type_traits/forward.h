#pragma once

#include<ccdk/mpl/type_traits/remove_ref.h>

namespace ccdk
{
	namespace mpl
	{
		template<typename T> inline constexpr T&& forward(remove_ref_t<T>&& inT) { return inT; }
	}
}