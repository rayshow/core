#pragma once

#include<ccdk/preprocessor/seq_compose.h>
#include<ccdk/preprocessor/seq_foreach_item.h>
#include<ccdk/mpl/base/bool_.h>


namespace ccdk
{
	namespace mpl
	{
		template<typename T> struct is_mfn_ptr :public false_ {};
		template<typename T> struct mfn_class { typedef T type; };
#define IS_MEMBER_FUNCTION(Postfix,...)                                                                                           \
		template<typename C, typename Ret, typename... Args> struct is_mfn_ptr<Ret (C::*)(Args...) Postfix> :public true_ {};     \
		template<typename C, typename Ret, typename... Args> struct is_mfn_ptr<Ret (C::*)(Args...,...) Postfix> :public true_ {};
#define MFN_CLASS(Postfix,...)                                                                                                    \
		template<typename C, typename Ret, typename... Args> struct mfn_class<Ret(C::*)(Args...) Postfix>{ typedef C type; };     \
		template<typename C, typename Ret, typename... Args> struct mfn_class<Ret(C::*)(Args..., ...) Postfix>{ typedef C type; };
		CCDK_PP_FOREACH_ITEM(IS_MEMBER_FUNCTION, CCDK_PP_COMPOSE_EX((const, volatile, const volatile), (&, &&, ), , &, &&))
		CCDK_PP_FOREACH_ITEM(MFN_CLASS, CCDK_PP_COMPOSE_EX((const, volatile, const volatile), (&, &&, ), , &, &&))
#undef IS_MEMBER_FUNCTION
#undef MFN_CLASS


		template<typename T> constexpr bool is_mfn_ptr_v = is_mfn_ptr<T>::value;

		template<typename T> using mfn_class_t = typename  mfn_class<T>::type;
	}
}