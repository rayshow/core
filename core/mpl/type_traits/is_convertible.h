#pragma once

#include<core/mpl/base/bool_.h>
#include<core/mpl/type_traits/declval.h>

namespace core
{
	namespace mpl
	{
		template<typename T, typename P>
		struct is_convertible_helper
		{
			static constexpr bool sfinae(T) { return true; }

			static constexpr bool sfinae(...) { return false; }

			static constexpr bool value = sfinae(declval<P>());
		};


		//implicitly convertible, use no-explicit-construction
		//e.g.string a = 1.1; not works  no suitable construction
		template<typename From, typename To> struct is_convertible : bool_<is_convertible_helper<From,To>::value> {};

		template<typename From, typename To> constexpr bool is_convertible_v = is_convertible<From, To>::value;
	}
}