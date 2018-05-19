#pragma once
#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/base/derive_if.h>
#include<ccdk/mpl/type_traits/declval.h>
#include<ccdk/mpl/type_traits/is_same.h>

#define CCDK_TT_HAS_ATTRIBUTE_DECL_IMPL(TraitName, AttributeName )             \
namespace TraitName##_ns                                                  \
{   template<typename ClassType, typename ValueType>                      \
	struct TraitName##_impl {                                             \
		template<typename P,                                              \
			typename Ret = decltype(makeval<P>().AttributeName),          \
			typename = check_t< derive_if<                                \
				is_null<ValueType>, true_, is_same<Ret, ValueType>>> >    \
			static constexpr bool sfinae(int) { return true; }            \
		template<typename P>                                              \
		static constexpr bool sfinae(...) { return false; }               \
		constexpr static bool value = sfinae<ClassType>(0);               \
};}                                                                       \

// define a type trait to test weather ValueType T.AttributeName is well-formed
#define CCDK_TT_HAS_ATTRIBUTE_DECL(TraitName, AttributeName )             \
		CCDK_TT_HAS_ATTRIBUTE_DECL_IMPL(TraitName, AttributeName)         \
template<typename ClassType, typename ValueType = null_>                  \
struct TraitName :bool_<TraitName##_ns::TraitName##_impl<ClassType,ValueType>::value>{}; \
template<typename ClassType, typename ValueType = null_>                  \
static constexpr bool TraitName##_v =                                     \
	TraitName##_ns::TraitName##_impl<ClassType, ValueType>::value;        \
template<typename ClassType, typename ValueType = null_>                  \
constexpr typename TraitName<ClassType,ValueType>::type TraitName##_c{};


// define a type trait to test weather Type T.AttributeName is well-formed
#define CCDK_TT_HAS_ATTRIBUTE_WITH_TYPE_DECL(TraitName, AttributeName, Type )  \
		CCDK_TT_HAS_ATTRIBUTE_DECL_IMPL(TraitName, AttributeName)         \
template<typename T>                                                      \
struct TraitName :bool_<TraitName##_ns::TraitName##_impl<T, Type>::value> {};  \
template<typename T>                                                      \
static constexpr bool TraitName##_v =                                     \
	TraitName##_ns::TraitName##_impl<T, Type>::value;                     \
template<typename T> constexpr typename TraitName<T>::type TraitName##_c{};