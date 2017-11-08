#pragma once

#include<core/mpl/type_traits/is_fundamental.h>
#include<core/mpl/base/not_.h>

namespace core
{
	namespace mpl
	{
		template<typename T> struct is_compound :not_< is_fundamental<T>> {};
		template<typename T> constexpr bool is_compound_v = is_compound<T>::value;
	}
}