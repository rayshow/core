#pragma once

#include<core/preprocessor/seq_compose.h>
#include<core/preprocessor/seq_foreach_item.h>
#include<core/preprocessor/sign.h>
#include<core/mpl/type_traits/underlying_type.h>

namespace Aurora3D
{
	namespace mpl
	{
		namespace detail
		{
			template<typename T> struct AddUnsignedHelper { typedef T type; };
			template<> struct AddUnsignedHelper<char> { typedef unsigned char type; };
#define ADD_UNSIGNED(NonsignType, prefix,...)  \
			template<> struct AddUnsignedHelper<signed NonsignType>{ typedef unsigned NonsignType type;  };
			A3D_PP_FOREACH_ITEM(ADD_UNSIGNED, A3D_PP_COMPOSE((char, short, int, long, long long), (A3D_PP_NULL, const, volatile, const volatile)));
#undef  ADD_UNSIGNED
		}

		template<typename T> struct AddUnsigned :public detail::AddUnsignedHelper<typename UnderlyingType<T>::type> {};
		template<typename T> using AddUnsignedT = typename AddUnsigned<T>::type;
	}
}