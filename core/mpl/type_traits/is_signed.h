#pragma once

#include<core/mpl/base/bool_.h>
#include<core/mpl/base/and_.h>
#include<core/mpl/type_traits/is_integral.h>

namespace core
{
	namespace mpl
	{
		namespace detail
		{
			template<typename T>
			struct is_signed_helper :public bool_< (T(0) > T(-1)) >{};
		}

		template<typename T> struct is_signed :
			public and_< is_integer<T>, detail::is_signed_helper<T> >{};
		template<typename T> constexpr bool is_signed_v = is_signed<T>::value;
	}
}