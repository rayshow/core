#pragma once

#include<core/mpl/base/bool_.h>

namespace core
{
	namespace mpl
	{
		//implicitly convertible, use no-explicit-construction
		//e.g.string a = 1.1; not works  no suitable construction
		template<typename From, typename To> struct is_convertible :
			public bool_<__is_convertible_to(From,To)> {};

		template<typename From, typename To>
		constexpr bool is_convertible_v = is_convertible<From, To>::value;

	}
}