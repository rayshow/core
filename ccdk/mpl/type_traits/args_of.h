#pragma once

#include<ccdk/compile.h>
#include<ccdk/preprocessor/seq_compose.h>
#include<ccdk/preprocessor/seq_foreach_item.h>
#include<ccdk/mpl/base/arg_pack.h>

namespace ccdk
{
	namespace mpl
	{
		namespace tt_impl
		{
			template<typename T> struct args_of_impl;

			//for function pointer
			template<typename T> struct args_of_impl<T*> : args_of_impl<T> {};

#ifdef CCDK_COMPILER_MSVC
#define FUNCTION_ENUM(Postfix,...) \
			template<typename Class, typename Ret, typename... Args> struct args_of_impl<Ret(Class::*)(Args...) Postfix> { typedef arg_pack<Args...> type; typedef Class clazz;};       \
			template<typename Class, typename Ret, typename... Args> struct args_of_impl<Ret(Class::*)(Args..., ...) Postfix> { typedef arg_pack<Args...> type; typedef Class clazz;};  \
			template<typename Ret, typename... Args> struct args_of_impl<Ret __cdecl(Args...) Postfix> { typedef arg_pack<Args...> type; typedef void clazz; };                                             \
			template<typename Ret, typename... Args> struct args_of_impl<Ret __cdecl(Args......) Postfix> { typedef arg_pack<Args...> type; typedef void clazz; };                                          \
			template<typename Ret, typename... Args> struct args_of_impl<Ret __stdcall(Args...) Postfix> { typedef arg_pack<Args...> type; typedef void clazz; };                                          \
			template<typename Ret, typename... Args> struct args_of_impl<Ret __fastcall(Args...) Postfix> { typedef arg_pack<Args...> type; typedef void clazz; };                                         \
			template<typename Ret, typename... Args> struct args_of_impl<Ret __vectorcall(Args...) Postfix> { typedef arg_pack<Args...> type; typedef void clazz; };
#else
#define FUNCTION_ENUM(Postfix,...) \
			template<typename Class, typename Ret, typename... Args> struct args_of_impl<Ret(Class::*)(Args...) Postfix> { typedef arg_pack<Class, Args...> type; typedef Class clazz;};       \
			template<typename Class, typename Ret, typename... Args> struct args_of_impl<Ret(Class::*)(Args..., ...) Postfix> {typedef arg_pack<Class, Args...> type; typedef Class clazz; };  \
			template<typename Ret, typename... Args> struct args_of_impl< Ret(Args...) Postfix> { typedef arg_pack<Args...> type; typedef void clazz; };                                                            \
			template<typename Ret, typename... Args> struct args_of_impl< Ret(Args...,...) Postfix> { typedef arg_pack<Args...> type; typedef void clazz; };
#endif
			CCDK_PP_FOREACH_ITEM(FUNCTION_ENUM, CCDK_PP_COMPOSE_EX((const, volatile, const volatile), (&, &&, ), , &, &&));
			
#undef FUNCTION_ENUM
		}

		template<typename T> struct args_of :tt_impl::args_of_impl<T> {};

		template<typename T> using  args_of_t = typename args_of<T>::type;

		template<typename T> using  mfn_class_t = typename  args_of<T>::clazz;

	}
}
