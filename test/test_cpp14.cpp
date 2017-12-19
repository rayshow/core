#include<cstdio>
#include<utility>
#include<memory>
#include<string>
#include<typeinfo>
#include<iostream>
#include<type_traits>
#include<cassert>
#include<functional>
#include<ccdk/mpl/type_traits.h>
#include<ccdk/mpl/base/arg_pack.h>
#include<ccdk/mpl/base/val_pack.h>
#include<ccdk/mpl/base/integer_.h>
#include<ccdk/mpl/base/int_.h>
#include<ccdk/mpl/type_traits/select_case.h>
#include<ccdk/mpl/function/arg.h>
#include<ccdk/mpl/function/function.h>
#include<ccdk/mpl/function/bind_mfn.h>
#include<ccdk/mpl/function/partial.h>
#include<ccdk/mpl/function/capture.h>

using namespace ccdk;
using namespace ccdk::mpl;
using namespace ccdk::mpl::literals;

template<typename... Args>
auto test_tuple_storage(Args&& ... args)
{
	DebugFunctionName();
	return tuple_storage<sizeof...(Args), make_indice<sizeof...(Args)>, decay_t<Args>...>{ util::forward<Args>(args)... };
}

int test_n(test_copy_t& tt, int a)
{
	DebugValue(a);
	return 0;
}

int test_n(test_copy_t& tt, float a)
{
	DebugValue(a);
	return 0;
}

template<typename T>
void fn_impl(case_t< is_function >, T&& t)
{
	DebugValue("is function");
}

template<typename T>
void fn_impl(case_t< is_function_ptr>, T&& t)
{
	DebugValue("is function pointer");
}

template<typename T>
void fn_impl(case_t< is_function_obj>, T&& t)
{
	DebugValue("is function obj");
}

template<typename T>
void fn_impl(case_t< is_mfn_ptr>, T&&t)
{
	DebugValue("is member function");
}

template<typename T>
void fn_impl(default_t, T&& t)
{
	DebugValue("default function ");
}

template<typename T>
void fn_impl(case_or<is_float, is_integer >, T&& t)
{
	DebugValue("is integer/float ");
}

template<typename T>
void fn_impl(case_and<is_pointer, is_void >, T&& t)
{
	DebugValue("is void* ");
}

template<typename T, typename P = remove_ref_t<T>>
void fn_dispatch_test(T&& t)
{
	//DebugTypeName< case_val_t<
	//	and_<is_pointer<P>, is_integer<remove_pointer_t<P>> >
	//	, true> >();

	//DebugTypeName< case_and<is_pointer, is_integer> >();

	//static dispatch
	fn_impl(dispatch_v<
		is_function<P>,
		is_function_ptr<P>,
		is_function_obj<P>,
		is_mfn_ptr<P>,
		or_<is_float<P>, is_integer<P>>,
		and_< is_pointer<P>, is_void<remove_pointer_t<P>>>
	>,
		util::forward<T>(t));
}

void simple_nfn() {}

struct test_obj
{
	int operator()(test_copy_t& tt, float a)
	{
		DebugValue(a);
		return 0;
	}
};

template<template<typename...>typename T>
struct traits {

};

int main()
{
<<<<<<< HEAD
	std::shared_ptr<int> sp;
	std::unique_ptr<int> up;
=======
	
	std::shared_ptr<int> sh;
	std::unique_ptr<int> us;

	std::pair(1, 2);

>>>>>>> 63301a61ef1eeeed47dfa050dfd8939a843eede8
	fn_dispatch_test(0);
	fn_dispatch_test(simple_nfn);
	fn_dispatch_test(&simple_nfn);
	fn_dispatch_test(test_obj{});
	fn_dispatch_test(&test_obj::operator());
	fn_dispatch_test(1);
	fn_dispatch_test(1.2f);
	fn_dispatch_test((void*)0);

	getchar();
	return 0;
}