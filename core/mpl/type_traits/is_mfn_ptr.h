#pragma once

#include<core/preprocessor/seq_compose.h>
#include<core/preprocessor/seq_foreach_item.h>
#include<core/mpl/base/bool_.h>


namespace core
{
	namespace mpl
	{
		template<typename T> struct is_mfn_ptr :public false_ {};
#define IS_MEMBER_FUNCTION(Postfix,...)                                                                                           \
		template<typename C, typename Ret, typename... Args> struct is_mfn_ptr<Ret (C::*)(Args...) Postfix> :public true_ {};     \
		template<typename C, typename Ret, typename... Args> struct is_mfn_ptr<Ret (C::*)(Args...,...) Postfix> :public true_ {};
		A3D_PP_FOREACH_ITEM(IS_MEMBER_FUNCTION, A3D_PP_COMPOSE_EX((const, volatile, const volatile), (&, &&, ), , &, &&))
#undef IS_FUNCTION

		template<typename T> constexpr bool is_mfn_ptr_v = is_mfn_ptr<T>::value;
	}
}