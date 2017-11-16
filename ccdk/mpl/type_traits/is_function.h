#pragma once

#include<ccdk/preprocessor/seq_compose.h>
#include<ccdk/preprocessor/seq_foreach_item.h>
#include<ccdk/preprocessor/sign.h>
#include<ccdk/mpl/base/bool_.h>
#include<ccdk/type.h>
#include<ccdk/compile.h>


namespace ccdk
{
	namespace mpl
	{
		template<typename T> struct is_function :public false_ {};
#ifdef CCDK_COMPILER_MSVC
		//varg function always prefix with __cdecl, no matter explicitly with which prefix
		//normal function default prefix width __cdecl
		#define IS_FUNCTION(Prefix, ...)                                                                                    \
			template<typename Ret, typename... Args> struct is_function<Ret __cdecl(Args...) Prefix> :public true_ {};      \
			template<typename Ret, typename... Args> struct is_function<Ret __cdecl(Args......) Prefix> :public true_ {};   \
			template<typename Ret, typename... Args> struct is_function<Ret __stdcall(Args...) Prefix> :public true_ {};    \
			template<typename Ret, typename... Args> struct is_function<Ret __fastcall(Args...) Prefix> :public true_ {};   \
			template<typename Ret, typename... Args> struct is_function<Ret __vectorcall(Args...) Prefix> :public true_ {};      
#else  
		#define IS_FUNCTION(Prefix,...)                                                                                     \
			template<typename Ret, typename... Args> struct is_function< Ret(Args...) Prefix> :public true_ {};             \
			template<typename Ret, typename... Args> struct is_function< Ret(Args......) Prefix> :public true_ {};    
#endif
		CCDK_PP_FOREACH_ITEM(IS_FUNCTION, CCDK_PP_COMPOSE_EX((const, volatile, const volatile), (&, &&, ), , &, &&))
#undef IS_FUNCTION

		template<typename T> constexpr bool is_function_v = is_function<T>::value;

	} 
}