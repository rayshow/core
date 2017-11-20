#pragma once

#include<ccdk/mpl/type_traits/remove_ref.h>

namespace ccdk
{
	namespace mpl
	{
		namespace util
		{
			template<typename T> inline constexpr T&& forward(remove_ref_t<T>& inT) noexcept { return static_cast<T&&>(inT); }
			template<typename T> inline constexpr T&& forward(remove_ref_t<T>&& inT) noexcept { return static_cast<T&&>(inT); }

		}
	}
}