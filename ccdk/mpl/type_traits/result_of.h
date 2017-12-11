#pragma once

#include<ccdk/compile.h>
#include<ccdk/preprocessor/seq_compose.h>
#include<ccdk/preprocessor/seq_foreach_item.h>

#include<ccdk/mpl/type_traits/is_function_obj.h>

namespace ccdk
{
	namespace mpl
	{
		namespace tt_impl
		{
			template<typename T> struct result_of_impl;

			//for function pointer
			template<typename T> struct result_of_impl<T*> : result_of_impl<T> {};

#ifdef CCDK_COMPILER_MSVC
	#define FUNCTION_ENUM(Postfix,...) \
			template<typename Class, typename Ret, typename... Args> struct result_of_impl<Ret(Class::*)(Args...) Postfix> { typedef Ret type; };        \
			template<typename Class, typename Ret, typename... Args> struct result_of_impl<Ret(Class::*)(Args..., ...) Postfix> { typedef Ret type; };   \
			template<typename Ret, typename... Args> struct result_of_impl<Ret __cdecl(Args...) Postfix> { typedef Ret type; };                          \
			template<typename Ret, typename... Args> struct result_of_impl<Ret __cdecl(Args......) Postfix> { typedef Ret type; };                       \
			template<typename Ret, typename... Args> struct result_of_impl<Ret __stdcall(Args...) Postfix> { typedef Ret type;  };                       \
			template<typename Ret, typename... Args> struct result_of_impl<Ret __fastcall(Args...) Postfix> { typedef Ret type;  };                      \
			template<typename Ret, typename... Args> struct result_of_impl<Ret __vectorcall(Args...) Postfix> { typedef Ret type; };
#else
	#define FUNCTION_ENUM(Postfix,...) \
			template<typename Class, typename Ret, typename... Args> struct result_of_impl<Ret(Class::*)(Args...) Postfix> { typedef Ret type; };       \
			template<typename Class, typename Ret, typename... Args> struct result_of_impl<Ret(Class::*)(Args..., ...) Postfix> { typedef Ret type; };  \
			template<typename Ret, typename... Args> struct result_of_impl< Ret(Args...) Postfix> { typedef Ret type; };                                \
			template<typename Ret, typename... Args> struct result_of_impl< Ret(Args......) Postfix> { typedef Ret type; };
#endif

			CCDK_PP_FOREACH_ITEM(FUNCTION_ENUM, CCDK_PP_COMPOSE_EX((const, volatile, const volatile), (&, &&, ), , &, &&))
#undef FUNCTION_ENUM
		}

		template<typename T, bool = is_function_obj_v<T> > struct result_of :tt_impl::result_of_impl<T> {};

		template<typename T> struct result_of<T,true>
		{
			typedef typename result_of< decltype(&T::operator()) >::type type;
		};

		template<typename T>  using result_of_t = typename result_of<T>::type;
	}
}
