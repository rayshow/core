#pragma once
#include<ccdk/mpl/type_traits/is_same.h>
#include<ccdk/mpl/type_traits/declval.h>

#define CCDK_TT_HAS_MEMBER_DECL_IMPL(TraitName, MemberName)                        \
namespace TraitName##ns                                                            \
{                                                                                  \
	template<typename T, typename Ret, typename ... Args>                          \
	struct TraitName##_impl {                                                      \
		template< typename P,                                                      \
		 typename AR = decltype(makeval<P>().##MemberName##(declval<Args>()...)),  \
		 typename = check_t< derive_if<is_null<Ret>, true_, is_same<Ret, AR>>> >   \
		constexpr static bool sfinae(int) { return true; }                         \
		template<typename P>                                                       \
		constexpr static bool sfinae(...) { return false; }                        \
		constexpr static bool value = sfinae<T>(0);                                \
	};                                                                             \
}

// define a type trait to test weather anyRet T.MemberName(args...) is well-formed
#define CCDK_TT_HAS_MEMBER_DECL( TraitName, MemberName)                           \
		CCDK_TT_HAS_MEMBER_DECL_IMPL( TraitName, MemberName)                      \
template<typename T, typename Ret, typename... Args> struct TraitName             \
	:bool_< TraitName##ns::##TraitName##_impl<T, Ret, Args...>::value >{};        \
template<typename T, typename Ret, typename... Args>                              \
static constexpr bool TraitName##_v = TraitName##ns::##TraitName##_impl<T, Ret, Args...>::value; \
template<typename T, typename... Args> struct TraitName##_ingore_ret              \
	:bool_< TraitName##ns::##TraitName##_impl<T, null_, Args...>::value >{};      \
template<typename T, typename... Args>                                            \
static constexpr bool TraitName##_ignore_ret_v = TraitName##ns::##TraitName##_impl<T, null_, Args...>::value; 

// define a type trait to test weather Ret T.MemberName(args...) is well-formed
#define CCDK_TT_HAS_MEMBER_WITH_RET_DECL(TraitName, MemberName, Ret)                \
			CCDK_TT_HAS_MEMBER_DECL_IMPL(TraitName, MemberName)                     \
template<typename T, typename... Args> struct TraitName                             \
	:bool_< TraitName##ns::##TraitName##_impl<T, Ret, Args...>::value >{};          \
template<typename T, typename... Args>                                              \
static constexpr bool TraitName##_v = TraitName##ns::##TraitName##_impl<T, Ret, Args...>::value; 
