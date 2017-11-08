#pragma once

#include<core/mpl/type_traits/is_enum.h>

namespace core
{
	namespace mpl
	{
		namespace detail
		{
			template<typename T, bool is_enum> struct enum_base_helper { typedef T type; };
			template<typename T> struct enum_base_helper<T, true> { typedef __underlying_type(T) type; };
		}

		//if   T is enum return enum-underlying type
		//else return T
		template<typename T> struct enum_base :public detail::enum_base_helper<T, is_enum_v<T>> {};
		template<typename T> using enum_base_t = typename enum_base<T>::type;
	}
}