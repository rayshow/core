#pragma once

#include<ccdk/mpl/type_traits/is_fundamental.h>
#include<ccdk/mpl/base/logic_.h>

namespace ccdk
{
	namespace mpl
	{
		template<typename T> struct is_compound :not_< is_fundamental<T>> {};
		template<typename T> constexpr bool is_compound_v = is_compound<T>::value;
	}
}