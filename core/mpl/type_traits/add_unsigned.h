#pragma once

#include<core/preprocessor/seq_compose.h>
#include<core/preprocessor/seq_foreach_item.h>
#include<core/mpl/type_traits/enum_base.h>

namespace core
{
	namespace mpl
	{
		namespace detail
		{
			template<typename T> struct add_unsigned_helper { typedef T type; };
			template<> struct add_unsigned_helper<char> { typedef unsigned char type; };
#define ADD_UNSIGNED(T,...) template<> struct add_unsigned_helper<signed T>{ typedef unsigned T type;  };
			CCDK_PP_FOREACH_ITEM(ADD_UNSIGNED, CCDK_PP_COMPOSE((char, short, int, long, long long), (, const, volatile, const volatile)));
#undef  ADD_UNSIGNED
		}

		template<typename T> struct add_unsigned :public detail::add_unsigned_helper< enum_base_t<T> > {};
		template<typename T> using add_unsigned_t = typename add_unsigned<T>::type;
	}
}