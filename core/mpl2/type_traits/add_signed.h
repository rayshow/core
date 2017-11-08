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
			template<typename T> struct AddSignedHelper { typedef T type; };
#define ADD_SIGNED(NonsignType, prefix,...)  \
			template<> struct AddSignedHelper<unsigned NonsignType>{ typedef signed NonsignType type;  };
			A3D_PP_FOREACH_ITEM(ADD_SIGNED, A3D_PP_COMPOSE((char, short, int, long, long long), (A3D_PP_NULL, const, volatile, const volatile)));
#undef  ADD_SIGNED
		}
		template<typename T> struct AddSigned :public detail::AddSignedHelper<typename UnderlyingType<T>::type> {};
		template<typename T> using AddSignedT = typename AddSigned<T>::type;
	}
}