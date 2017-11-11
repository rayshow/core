#pragma once

#include<core/mpl/base/bool_.h>
#include<core/mpl/type_traits/add_const_lref.h>
#include<core/mpl/type_traits/add_rref.h>

namespace core
{
	namespace mpl
	{
		//can modification  accord to platform
#define HAS_CONSTRUCTOR(Type, Args)           __is_constructible(Type, Args)
#define HAS_TRIVIAL_CONSTRUCTOR(Type, Args)   __is_trivially_constructible(Type, Args)
#define HAS_NOTHROW_CONSTRUCTOR(Type, Args)   __is_nothrow_constructible(Type, Args)

		template<typename T, typename... Args> struct has_constructor :public bool_<HAS_CONSTRUCTOR(T, Args...)> {};
		template<typename T> struct has_default_constructor :public has_constructor<T> {};
		template<typename T> struct has_copy_constructor :public has_constructor<T, add_const_lref_t<T>> {};
		template<typename T> struct has_move_constructor :public has_constructor<T, add_rref_t<T>> {};

		template<typename T, typename... Args> struct has_trivial_constructor :public bool_<HAS_TRIVIAL_CONSTRUCTOR(T, Args...)> {};
		template<typename T> struct has_trivial_copy_constructor :public has_trivial_constructor<T, add_const_lref_t<T> > {};
		template<typename T> struct has_trivial_move_constructor :public has_trivial_constructor<T, add_rref_t<T> > {};

		template<typename T, typename... Args> struct has_nothrow_constructor :public bool_<HAS_NOTHROW_CONSTRUCTOR(T, Args...)> {};
		template<typename T> struct has_nothrow_copy_constructor :public has_nothrow_constructor<T, add_const_lref_t<T>> {};
		template<typename T> struct has_nothrow_move_constructor :public has_nothrow_constructor<T, add_rref_t<T>> {};
		
#undef HAS_CONSTRUCTOR
#undef HAS_TRIVIAL_CONSTRUCTOR
#undef HAS_NOTHROW_CONSTRUCTOR
	}
}