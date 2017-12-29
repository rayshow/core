#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/enable_if.h>
#include<ccdk/mpl/type_traits/decay.h>
#include<ccdk/mpl/type_traits/is_function.h>
#include<ccdk/mpl/util/addressof.h>

ccdk_namespace_mpl_fn_start


template<typename Fn>
struct select_overload_t;


template<typename Ret, typename... Args>
struct select_overload_t<Ret(Args...)>
{
	using value_type = Ret(*)(Args...);

	value_type fn;

	CCDK_FORCEINLINE select_overload_t(value_type inFn) :fn(inFn) {}

	CCDK_FORCEINLINE constexpr decltype(auto) operator()(Args... args) const { return fn(args...); }
};


template<typename Ret, typename Class, typename... Args>
struct select_overload_t<Ret(Class::*)(Args...)>
{
	using value_type = Ret(Class::*)(Args...);

	value_type fn;
	Class*     clazz;

	CCDK_FORCEINLINE select_overload_t(Class* inClass, value_type inFn) :clazz{ inClass }, fn{ inFn } {}

	CCDK_FORCEINLINE constexpr decltype(auto) operator()(Args... args) const { return (clazz->*fn)(args...); }
};

template<typename T>
struct make_select_overload_t;

template<typename Ret, typename... Args>
struct make_select_overload_t<Ret(Args...)>
{
	using Fn = Ret(Args...);

	/* select normal function overload */
	CCDK_FORCEINLINE constexpr auto operator()(Fn* fn) const { return select_overload_t<Fn>{ fn }; }

	/* select member function overload */
	template< typename Class, typename Class1 = decay_t<Class>, typename MFn = Ret(Class1::*)(Args...) >
	CCDK_FORCEINLINE constexpr auto operator()(Class* clazz, MFn fn) const { return select_overload_t<MFn>{ clazz, fn }; }
};


template<typename Fn>
constexpr make_select_overload_t<Fn> select_overload{};


ccdk_namespace_mpl_fn_end