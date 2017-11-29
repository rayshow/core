#pragma once

#include<ccdk/preprocessor/seq_compose.h>
#include<ccdk/preprocessor/seq_foreach_item.h>
#include<ccdk/mpl/type_traits/remove_pointer.h>
#include<ccdk/mpl/base/bool_.h>
#include<ccdk/compile.h>


namespace ccdk
{
	namespace mpl
	{
		template<typename... Args> struct fn_args{};

		template<typename T> struct is_function :public false_ {};
		template<typename T> struct function_body {};
#ifdef CCDK_COMPILER_MSVC
		//varg function always prefix with __cdecl, no matter explicitly with which prefix
		//normal function default prefix width __cdecl
		#define IS_FUNCTION(Prefix, ...)                                                                                    \
			template<typename Ret, typename... Args> struct is_function<Ret __cdecl(Args...) Prefix> :public true_ {};      \
			template<typename Ret, typename... Args> struct is_function<Ret __cdecl(Args......) Prefix> :public true_ {};   \
			template<typename Ret, typename... Args> struct is_function<Ret __stdcall(Args...) Prefix> :public true_ {};    \
			template<typename Ret, typename... Args> struct is_function<Ret __fastcall(Args...) Prefix> :public true_ {};   \
			template<typename Ret, typename... Args> struct is_function<Ret __vectorcall(Args...) Prefix> :public true_ {};      

		#define FUNCTION_BODY(Prefix, ...)                                                                                  \
			template<typename Ret, typename... Args> struct function_body<Ret __cdecl(Args...) Prefix>  { typedef Ret ret; typedef fn_args<Args...> args; };      \
			template<typename Ret, typename... Args> struct function_body<Ret __cdecl(Args......) Prefix> { typedef Ret ret; typedef fn_args<Args...> args; };    \
			template<typename Ret, typename... Args> struct function_body<Ret __stdcall(Args...) Prefix> { typedef Ret ret; typedef fn_args<Args...> args; };     \
			template<typename Ret, typename... Args> struct function_body<Ret __fastcall(Args...) Prefix> { typedef Ret ret; typedef fn_args<Args...> args; };    \
			template<typename Ret, typename... Args> struct function_body<Ret __vectorcall(Args...) Prefix> { typedef Ret ret; typedef fn_args<Args...> args; };  \

#else  
		#define IS_FUNCTION(Prefix,...)                                                                                     \
			template<typename Ret, typename... Args> struct is_function< Ret(Args...) Prefix> :public true_ {};             \
			template<typename Ret, typename... Args> struct is_function< Ret(Args......) Prefix> :public true_ {};    
		#define FUNCTION_BODY(Prefix, ...)                                                                                                                \
			template<typename Ret, typename... Args> struct function_body< Ret(Args...) Prefix> { typedef Ret ret; typedef fn_args<Args...> args; };      \
			template<typename Ret, typename... Args> struct function_body< Ret(Args......) Prefix> { typedef Ret ret; typedef fn_args<Args...> args; };   
#endif
		CCDK_PP_FOREACH_ITEM(IS_FUNCTION, CCDK_PP_COMPOSE_EX((const, volatile, const volatile), (&, &&, ), , &, &&))
		CCDK_PP_FOREACH_ITEM(FUNCTION_BODY, CCDK_PP_COMPOSE_EX((const, volatile, const volatile), (&, &&, ), , &, &&))
#undef IS_FUNCTION
#undef FUNCTION_BODY

		template<typename T> constexpr bool is_function_v = is_function<T>::value;

		template<typename T> struct is_function_ptr : is_function< remove_pointer_t<T> > {};

		template<typename T> constexpr bool is_function_ptr_v = is_function_ptr<T>::value;
	} 
}