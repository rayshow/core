#pragma once

#include<core/mpl/base/bool_.h>
#include<core/compile.h>
#include<core/mpl/type_traits/add_const_lref.h>
#include<core/mpl/type_traits/add_rref.h>

#if defined(AURORA3D_COMPILER_GCC)
#include<core/mpl/type_traits/is_array.h>
#include<core/mpl/type_traits/remove_all_dim.h>
#include<core/mpl/type_traits/declval.h>
#endif
#include<core/mpl/type_traits/remove_ref.h>

namespace core
{
	namespace mpl
	{
#if defined(AURORA3D_COMPILER_MSVC) || defined(AURORA3D_COMPILER_CLANG) || defined(AURORA3D_COMPILER_INTEL)
		//normal constructor family
		template<typename T, typename... Args> struct has_constructor :public bool_<__is_constructible(T, Args...)> {};
		template<typename T> struct has_copy_constructor :public has_constructor<T, add_const_lref_t<T>> {};
		template<typename T> struct has_move_constructor :public has_constructor<T, add_rref_t<T>> {};		

		template<typename T, typename... Args> struct has_nothrow_constructor :public bool_<__is_nothrow_constructible(T, Args...)> {};
		template<typename T> struct has_nothrow_copy_constructor :public has_nothrow_constructor<T, add_const_lref_t<T>> {};
		template<typename T> struct has_nothrow_move_constructor :public has_nothrow_constructor<T, add_rref_t<T>> {};

#else
		template<typename T, typename... Args>
		struct has_constructor_helper
		{
			template<typename U, typename = decltype(::new U(declval<Args>()...)) >
			static constexpr bool sfinae(int) { return true; }

			template<typename U> static constexpr bool sfinae(...) { return false; }

			static constexpr bool value = sfinae<T>(0);
		};
		template<typename T, typename... Args> struct has_nothrow_constructor_helper :bool_< noexcept(T(declval<Args>()...))> {};

		//normal constructor family
		template<typename T, typename...Args> struct has_constructor :bool_<has_constructor_helper<remove_rref_t<T>, Args...>::value> {};
		template<typename T> struct has_copy_constructor :has_constructor<T, add_lref_t<T>> {};
		template<typename T> struct has_move_constructor :has_constructor<T, add_rref_t<T>> {};

		//nothrow constructor family
		template<typename T, typename... Args> struct has_nothrow_constructor :and_< has_constructor<remove_rref_t<T>, Args...>, has_nothrow_constructor_helper<remove_rref_t<T>, Args...>> {};
		template<typename T> struct has_nothrow_copy_constructor :has_nothrow_constructor<T, add_lref_t<T>> {};
		template<typename T> struct has_nothrow_move_constructor :has_nothrow_constructor<T, add_rref_t<T>> {};
#endif
		// c++14 of gcc6 msvc17 clang4 support this extension
		//trivial constructor family
		template<typename T, typename... Args> struct has_trivial_constructor :public bool_<__is_trivially_constructible(T, Args...)> {};
		template<typename T> struct has_trivial_copy_constructor :public has_trivial_constructor<T, add_const_lref_t<T> > {};
		template<typename T> struct has_trivial_move_constructor :public has_trivial_constructor<T, add_rref_t<T> > {};

		//short define for has_xx_contructor<T,...>::value
		template<typename T, typename... Args> static constexpr bool has_constructor_v = has_constructor<T, Args...>::value;
		template<typename T> static constexpr bool has_copy_constructor_v = has_copy_constructor<T>::value;
		template<typename T> static constexpr bool has_move_constructor_v = has_move_constructor<T>::value;
		template<typename T, typename... Args> static constexpr bool  has_nothrow_constructor_v = has_nothrow_constructor<T, Args...>::value;
		template<typename T> static constexpr bool has_nothrow_copy_constructor_v = has_nothrow_copy_constructor<T>::value;
		template<typename T> static constexpr bool has_nothrow_move_constructor_v = has_nothrow_move_constructor<T>::value;
		template<typename T, typename... Args> static constexpr bool has_trivial_constructor_v = has_trivial_constructor<T, Args...>::value;
		template<typename T> static constexpr bool has_trivial_copy_constructor_v = has_trivial_copy_constructor<T>::value;
		template<typename T> static constexpr bool has_trivial_move_constructor_v = has_trivial_move_constructor<T>::value;


	}
}