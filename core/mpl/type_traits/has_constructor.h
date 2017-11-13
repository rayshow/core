#pragma once

#include<core/mpl/base/bool_.h>
#include<core/mpl/type_traits/add_const_lref.h>
#include<core/mpl/type_traits/add_rref.h>
#include<core/compile.h>

#include<core/mpl/type_traits/remove_all_dim.h>

namespace core
{
	namespace mpl
	{

		template<typename T, bool = is_void_v<T> || is_unlimit_array<T> >
		struct has_empty_constructor_helper
		{
			template<typename P, typename = decltype(P()) >
			static constexpr bool sfinae(int) { return true; }

			template<typename P>
			static constexpr bool sfinae(...) { return false; }

			static constexpr bool value = sfinae<remove_all_dim_t<T>>(0);
		};

		template<typename T>
		struct has_empty_constructor_helper<T, true> :false_ {};

		template<typename T>
		struct has_empty_constructor :has_empty_constructor_helper<T> {};

		//can modification  accord to platform
#if !defined(AURORA3D_COMPILER_MSVC) || defined(AURORA3D_COMPILER_CLANG) || defined(AURORA3D_COMPILER_INTEL)
#define HAS_CONSTRUCTOR(Type, ...)           __is_constructible(Type, __VA_ARGS__)
#define HAS_TRIVIAL_CONSTRUCTOR(Type, Args)   __is_trivially_constructible(Type, Args)
#define HAS_NOTHROW_CONSTRUCTOR(Type, Args)   __is_nothrow_constructible(Type, Args)
#else 
		//narg-constructor n>1
		template<typename T, typename...Args>
		struct has_constructor_helper
		{

		};

		//1 arg-constructor
		//check no to be ref and 
		template<typename T, typename P>
		struct has_constructor_helper<T, P> {};

		//no arg constructor
		//test T() method
		template<typename T>
		struct has_constructor_helper<T>:has_empty_constructor_helper<T>{};

		


#define HAS_CONSTRUCTOR(Type, ...)          false
#define HAS_TRIVIAL_CONSTRUCTOR(Type, Args)  false
#define HAS_NOTHROW_CONSTRUCTOR(Type, Args)   false
#endif
		//T(Args...) constructor
		template<typename T, typename... Args> struct has_constructor :public bool_<HAS_CONSTRUCTOR(T, Args...)> {};
		template<typename T, typename... Args> static constexpr bool has_constructor_v = has_constructor<T, Args...>::value;

		//T() constructor
		template<typename T> struct has_empty_constructor :public has_constructor<T> {};
		template<typename T> static constexpr bool has_empty_constructor_v = has_empty_constructor<T>::value;

		//T(const T&) constructor
		template<typename T> struct has_copy_constructor :public has_constructor<T, add_const_lref_t<T>> {};
		template<typename T> static constexpr bool has_copy_constructor_v = has_copy_constructor<T>::value;

		//T(T&&) constructor
		template<typename T> struct has_move_constructor :public has_constructor<T, add_rref_t<T>> {};
		template<typename T> static constexpr bool has_move_constructor_v = has_move_constructor<T>::value;
		
		template<typename T, typename... Args> struct has_trivial_constructor :public bool_<HAS_TRIVIAL_CONSTRUCTOR(T, Args...)> {};
		template<typename T> struct has_trivial_copy_constructor :public has_trivial_constructor<T, add_const_lref_t<T> > {};
		template<typename T> struct has_trivial_move_constructor :public has_trivial_constructor<T, add_rref_t<T> > {};

		template<typename T, typename... Args> struct has_nothrow_constructor :public bool_<HAS_NOTHROW_CONSTRUCTOR(T, Args...)> {};
		template<typename T, typename... Args> static constexpr bool has_nothrow_constructor_v = has_nothrow_constructor<T, Args...>::value;

		template<typename T> struct has_nothrow_copy_constructor :public has_nothrow_constructor<T, add_const_lref_t<T>> {};
		template<typename T> struct has_nothrow_move_constructor :public has_nothrow_constructor<T, add_rref_t<T>> {};



		
#undef HAS_CONSTRUCTOR
#undef HAS_TRIVIAL_CONSTRUCTOR
#undef HAS_NOTHROW_CONSTRUCTOR
	}
}