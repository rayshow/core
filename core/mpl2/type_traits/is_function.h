#pragma once

#include<core/preprocessor/seq_compose.h>
#include<core/preprocessor/seq_foreach_item.h>
#include<core/preprocessor/sign.h>
#include<core/mpl/bool_.h>
#include<core/type.h>


namespace Aurora3D
{
	namespace mpl
	{
		template<typename T> struct IsFunction :public False_ {};

#ifdef A3D_COMPILER_MSVC
		#define IS_FUNCTION(Prefix, prefix,...)                                                                                   \
			template<typename Ret, typenameprefix,... Args> struct IsFunction<Ret __cdecl(Argsprefix,...) Prefix> :public True_ {};      \
			template<typename Ret, typenameprefix,... Args> struct IsFunction<Ret __cdecl(Argsprefix,..., prefix,...) Prefix> :public True_ {}; \
			template<typename Ret, typenameprefix,... Args> struct IsFunction<Ret __stdcall(Argsprefix,...) Prefix> :public True_ {};    \
			template<typename Ret, typenameprefix,... Args> struct IsFunction<Ret __fastcall(Argsprefix,...) Prefix> :public True_ {};   \
			template<typename Ret, typenameprefix,... Args> struct IsFunction<Ret __vectorcall(Argsprefix,...) Prefix> :public True_ {};      
#else  
		#define IS_FUNCTION(Prefix,prefix,...)                                                                                     \
			template<typename Ret, typenameprefix,... Args> struct IsFunction< Ret(Argsprefix,...) Prefix> :public True_ {};          \
			template<typename Ret, typenameprefix,... Args> struct IsFunction< Ret(Argsprefix,..., prefix,...) Prefix> :public True_ {};    
#endif
		A3D_PP_FOREACH_ITEM(IS_FUNCTION, A3D_PP_COMPOSE_EX((const,volatile,const volatile), (&,&&,  A3D_PP_NULL), A3D_PP_NULL, &, &&))
#undef IS_FUNCTION

#define IsFunctionV(T)   (IsFunction<T>::value)
#define NotFunctionV(T)  (!IsFunction<T>::value)
	} 
}