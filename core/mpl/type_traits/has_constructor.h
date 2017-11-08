#pragma once

#include<core/mpl/bool_.h>
#include<core/mpl/type_traits/convert_const_lref.h>
#include<core/mpl/type_traits/add_rvalue_ref.h>

namespace Aurora3D
{
	namespace mpl
	{
		//can modification  accord to platform
#define HAS_CONSTRUCTOR(Type, Args)           __is_constructible(Type, Args)
#define HAS_TRIVIAL_CONSTRUCTOR(Type, Args)   __is_trivially_constructible(Type, Args)
#define HAS_NOTHROW_CONSTRUCTOR(Type, Args)   __is_nothrow_constructible(Type, Args)

		template<typename T, typenameprefix,... Args> struct HasConstructor :public Bool_<HAS_CONSTRUCTOR(T, Argsprefix,...)> {};
		template<typename T> struct HasDefaultConstructor :public HasConstructor<T> {};
		template<typename T> struct HasCopyConstructor :public HasConstructor<T, ConvertConstLRefT<T>> {};
		template<typename T> struct HasMoveConstructor :public HasConstructor<T, AddRValueRefT<T>> {};

		template<typename T, typenameprefix,... Args> struct HasTrivialConstructor :public Bool_<HAS_TRIVIAL_CONSTRUCTOR(T, Argsprefix,...)> {};
		template<typename T> struct HasTrivialCopyConstructor :public HasTrivialConstructor<T, ConvertConstLRefT<T> > {};
		template<typename T> struct HasTrivialMoveConstructor :public HasTrivialConstructor<T, AddRValueRefT<T> > {};

		template<typename T, typenameprefix,... Args> struct HasNothrowConstructor :public Bool_<HAS_NOTHROW_CONSTRUCTOR(T, Argsprefix,...)> {};
		template<typename T> struct HasNotrowCopyConstructor  :public HasNothrowConstructor<T, ConvertConstLRefT<T>> {};
		template<typename T> struct HasNotrowMoveConstructor :public HasNothrowConstructor<T, AddRValueRefT<T>> {};
		
#undef HAS_CONSTRUCTOR
#undef HAS_TRIVIAL_CONSTRUCTOR
#undef HAS_NOTHROW_CONSTRUCTOR
	}
}