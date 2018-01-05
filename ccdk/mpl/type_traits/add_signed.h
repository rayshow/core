#pragma once

#include<ccdk/preprocessor/seq_compose.h>
#include<ccdk/preprocessor/seq_foreach_item.h>
#include<ccdk/preprocessor/sign.h>
#include<ccdk/mpl/type_traits/enum_base.h>

namespace ccdk
{
	namespace mpl
	{
		namespace tt_detail
		{
			template<typename T> struct add_signed_helper { typedef T type; };
			template<> struct add_signed_helper<char> { typedef signed char type; };
#define ADD_SIGNED(T,...)  template<> struct add_signed_helper<unsigned T>{ typedef signed T type;  };
			CCDK_PP_FOREACH_ITEM(ADD_SIGNED, CCDK_PP_COMPOSE((char, short, int, long, long long), (,const, volatile, const volatile)));
#undef  ADD_SIGNED
		}

		//unsigned integer to signed integer
		template<typename T> struct add_signed :public tt_detail::add_signed_helper< enum_base_t<T> > {};
		template<typename T> using add_signed_t = typename add_signed<T>::type;
	}
}