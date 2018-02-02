#pragma once

#include<ccdk/mpl/base/type_.h>
#include<ccdk/compile.h>
#include<ccdk/mpl/type_traits/add_const_lref.h>
#include<ccdk/mpl/type_traits/add_rref.h>

#if defined(CCDK_COMPILER_GCC)
#include<ccdk/mpl/type_traits/is_array.h>
#include<ccdk/mpl/type_traits/remove_all_dim.h>
#include<ccdk/mpl/type_traits/declval.h>
#endif
#include<ccdk/mpl/type_traits/remove_ref.h>

namespace ccdk
{
	namespace mpl
	{
#if defined(CCDK_COMPILER_MSVC) || defined(CCDK_COMPILER_CLANG) || defined(CCDK_COMPILER_INTEL)
		//normal constructor family
		template<typename T, typename... Args> struct has_constructor :public bool_<__is_constructible(T, Args...)> {};
		template<typename T> struct has_copy_constructor :public has_constructor<T, add_const_lref_t<T>> {};
		template<typename T> struct has_move_constructor :public has_constructor<T, add_rref_t<T>> {};		

		template<typename T, typename... Args> struct has_nothrow_constructor :public bool_<__is_nothrow_constructible(T, Args...)> {};
		template<typename T> struct has_nothrow_copy_constructor :public has_nothrow_constructor<T, add_const_lref_t<T>> {};
		template<typename T> struct has_nothrow_move_constructor :public has_nothrow_constructor<T, add_rref_t<T>> {};

#else
		namespace detail
		{
			template<typename T, typename... Args>
			struct has_constructor_helper
			{
				template<typename U, typename = decltype(::new U(declval<Args>()...)) >
				static constexpr bool sfinae(int) { return true; }

				template<typename U> static constexpr bool sfinae(...) { return false; }

				static constexpr bool value = sfinae<T>(0);
			};
			template<typename T, typename... Args> struct has_nothrow_constructor_helper :bool_< noexcept(T(declval<Args>()...))> {};
		}
		

		//normal constructor family
		template<typename T, typename...Args> struct has_constructor :bool_<detail::has_constructor_helper<remove_rref_t<T>, Args...>::value> {};
		template<typename T> struct has_copy_constructor :has_constructor<T, add_lref_t<T>> {};
		template<typename T> struct has_move_constructor :has_constructor<T, add_rref_t<T>> {};

		//nothrow constructor family
		template<typename T, typename... Args> struct has_nothrow_constructor :and_<has_constructor<remove_rref_t<T>, Args...>, detail::has_nothrow_constructor_helper<remove_rref_t<T>, Args...>> {};
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