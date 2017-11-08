#pragma once

#include<core/mpl/type_traits/is_enum.h>

namespace Aurora3D
{
	namespace mpl
	{
		namespace detail
		{
			template<typename T, bool IsEnum> struct UnderlyingTypeHelper { typedef T type; };
			template<typename T> struct UnderlyingTypeHelper<T, true> { typedef __underlying_type(T) type; };
		}

		//not change
		template<typename T> struct UnderlyingType :public detail::UnderlyingTypeHelper<T, IsEnum<T>::value> {};
		template<typename T> using UnderlyingTypeT = typename UnderlyingType<T>::type;
	}
}