#pragma once

#include<ccdk/preprocessor/seq_compose.h>
#include<ccdk/preprocessor/seq_foreach_item.h>

#include<ccdk/mpl/base/bool_.h>
#include<ccdk/mpl/base/arg_pack.h>


namespace ccdk
{
	namespace mpl
	{
		//mfn ptr is not pointer but rather slot number
		template<typename T> struct is_mfn_ptr :public false_ {};
		template<typename T> struct mfn_traits {  };

#define IS_MEMBER_FUNCTION(Postfix,...)                                                                                           \
		template<typename C, typename Ret, typename... Args> struct is_mfn_ptr<Ret (C::*)(Args...) Postfix> :public true_ {};     \
		template<typename C, typename Ret, typename... Args> struct is_mfn_ptr<Ret (C::*)(Args...,...) Postfix> :public true_ {};

#define MFN_CLASS(Postfix,...)                                                                                                    \
		template<typename C, typename Ret, typename... Args> struct mfn_traits<Ret(C::*)(Args...) Postfix>       { typedef C clazz;  typedef Ret ret;  typedef arg_pack<Args...> args;  };                                            \
		template<typename C, typename Ret, typename... Args> struct mfn_traits<Ret(C::*)(Args..., ...) Postfix>  { typedef C clazz;  typedef Ret ret;  typedef arg_pack<Args...> args; };
		
		CCDK_PP_FOREACH_ITEM(IS_MEMBER_FUNCTION, CCDK_PP_COMPOSE_EX((const, volatile, const volatile), (&, &&, ), , &, &&))
		CCDK_PP_FOREACH_ITEM(MFN_CLASS, CCDK_PP_COMPOSE_EX((const, volatile, const volatile), (&, &&, ), , &, &&))
#undef IS_MEMBER_FUNCTION
#undef MFN_CLASS

		template<typename T> constexpr bool is_mfn_ptr_v = is_mfn_ptr<T>::value;

		template<typename T> using mfn_class_t = typename  mfn_traits<T>::clazz;

		template<typename T> using mfn_args_t = typename mfn_traits<T>::args;

		template<typename T> using mfn_ret_t = typename mfn_traits<T>::ret;

		
		CCDK_TT_SINGLE_RANK(is_mfn_ptr);

	}
}