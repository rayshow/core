#pragma once

#include<ccdk/preprocessor/seq_compose.h>
#include<ccdk/preprocessor/seq_foreach_item.h>

#include<ccdk/mpl/base/type_.h>


namespace ccdk
{
	namespace mpl
	{
		//mfn ptr is not pointer but rather slot number
		template<typename T> struct is_mfn_ptr :public false_ {};

#define IS_MEMBER_FUNCTION_ENUM(Postfix,...)  \
		template<typename Class, typename Ret, typename... Args> struct is_mfn_ptr<Ret (Class::*)(Args...) Postfix> :public true_ {};    \
		template<typename Class, typename Ret, typename... Args> struct is_mfn_ptr<Ret (Class::*)(Args...,...) Postfix> :public true_ {}; 

		CCDK_PP_FOREACH_ITEM(IS_MEMBER_FUNCTION_ENUM, CCDK_PP_COMPOSE_EX((const, volatile, const volatile), (&, &&, ), , &, &&))
#undef IS_MEMBER_FUNCTION_ENUM

		template<typename T> constexpr bool is_mfn_ptr_v = is_mfn_ptr<T>::value;


		CCDK_TT_SINGLE_RANK(is_mfn_ptr);

	}
}