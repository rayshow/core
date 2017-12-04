#pragma once

#include<ccdk/compile.h>
#include<ccdk/preprocessor/seq_compose.h>
#include<ccdk/preprocessor/seq_foreach_item.h>
#include<ccdk/mpl/base/bool_.h>
#include<ccdk/mpl/base/and_.h>
#include<ccdk/mpl/base/not_.h>
#include<ccdk/mpl/base/arg_pack.h>
#include<ccdk/mpl/type_traits/is_pointer.h>
#include<ccdk/mpl/type_traits/traits_case.h>

namespace ccdk
{
	namespace mpl
	{
		template<typename T> struct is_function :false_ {};
		template<typename T> struct is_function<T*> :is_function<T> {};
		
		template<typename T> struct function_traits {};
		template<typename T> struct function_traits<T*> :function_traits<T> {};
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
			template<typename Ret, typename... Args> struct function_traits<Ret __cdecl(Args...) Prefix>  { typedef Ret ret; typedef arg_pack<Args...> args; };      \
			template<typename Ret, typename... Args> struct function_traits<Ret __cdecl(Args......) Prefix> { typedef Ret ret; typedef arg_pack<Args...> args; };    \
			template<typename Ret, typename... Args> struct function_traits<Ret __stdcall(Args...) Prefix> { typedef Ret ret; typedef arg_pack<Args...> args; };     \
			template<typename Ret, typename... Args> struct function_traits<Ret __fastcall(Args...) Prefix> { typedef Ret ret; typedef arg_pack<Args...> args; };    \
			template<typename Ret, typename... Args> struct function_traits<Ret __vectorcall(Args...) Prefix> { typedef Ret ret; typedef arg_pack<Args...> args; };  \

#else  
		#define IS_FUNCTION(Prefix,...)                                                                                     \
			template<typename Ret, typename... Args> struct is_function< Ret(Args...) Prefix> :public true_ {};             \
			template<typename Ret, typename... Args> struct is_function< Ret(Args......) Prefix> :public true_ {};    
		#define FUNCTION_BODY(Prefix, ...)                                                                                                                \
			template<typename Ret, typename... Args> struct function_traits< Ret(Args...) Prefix> { typedef Ret ret; typedef arg_pack<Args...> args; };      \
			template<typename Ret, typename... Args> struct function_traits< Ret(Args......) Prefix> { typedef Ret ret; typedef arg_pack<Args...> args; };   
#endif
		CCDK_PP_FOREACH_ITEM(IS_FUNCTION, CCDK_PP_COMPOSE_EX((const, volatile, const volatile), (&, &&, ), , &, &&))
		CCDK_PP_FOREACH_ITEM(FUNCTION_BODY, CCDK_PP_COMPOSE_EX((const, volatile, const volatile), (&, &&, ), , &, &&))
#undef IS_FUNCTION
#undef FUNCTION_BODY

		template<typename T> constexpr bool is_function_v = is_function<T>::value;

		template<typename T> struct is_function_ptr: and_< is_function<T>,is_pointer<T> > {};

		template<typename T> constexpr bool is_function_ptr_v = is_function_ptr<T>::value;

		template<typename T> using function_ret_t = typename function_traits<T>::ret;

		template<typename T> using function_args_t = typename function_traits<T>::args;

		


		//for static dispatch
		CCDK_TT_SINGLE_RANK(is_function);

		CCDK_TT_COMPOSE_RANK( ccdk_tt_rank_and_val, is_function_ptr, is_function, is_pointer);

	} 
}