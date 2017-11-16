#pragma once

#include<core/preprocessor/seq_compose.h>
#include<core/preprocessor/seq_foreach_item.h>
#include<core/preprocessor/sign.h>
#include<core/mpl/type_traits/enum_base.h>

namespace core
{
	namespace mpl
	{
		namespace detail
		{
			template<typename T> struct add_signed_helper { typedef T type; };
			template<> struct add_signed_helper<char> { typedef signed char type; };
#define ADD_SIGNED(T,...)  template<> struct add_signed_helper<unsigned T>{ typedef signed T type;  };
			CCDK_PP_FOREACH_ITEM(ADD_SIGNED, CCDK_PP_COMPOSE((char, short, int, long, long long), (,const, volatile, const volatile)));
#undef  ADD_SIGNED
		}

		//unsigned integer to signed integer
		template<typename T> struct add_signed :public detail::add_signed_helper< enum_base_t<T> > {};
		template<typename T> using add_signed_t = typename add_signed<T>::type;
	}
}