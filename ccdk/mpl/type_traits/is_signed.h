#pragma once

#include<ccdk/mpl/base/type_.h>
#include<ccdk/mpl/base/logic_.h>
#include<ccdk/mpl/type_traits/is_integer.h>

namespace ccdk
{
	namespace mpl
	{
		namespace tt_detail
		{
			template<typename T>
			struct is_signed_helper :public bool_< (T(0) > T(-1)) >{};
		}

		template<typename T> struct is_signed :
			public and_< is_integer<T>, tt_detail::is_signed_helper<T> >{};
		template<typename T> constexpr bool is_signed_v = is_signed<T>::value;
	}
}