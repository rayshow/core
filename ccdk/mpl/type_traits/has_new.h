#pragma once

#include<ccdk/type.h>
#include<ccdk/mpl/base/bool_.h>
#include<ccdk/mpl/base/and_.h>
#include<ccdk/mpl/type_traits/impl/has_operator_decl.h>

namespace ccdk
{
	namespace mpl
	{
		/*
		* new T results in a call of operator new(sizeof(T)),
		* new(2, f) T results in a call of operator new(sizeof(T), 2, f),
		* new T[5] results in a call of operator new[](sizeof(T) * 5 + x), and
		* new(2, f) T[5] results in a call of operator new[](sizeof(T) * 5 + y, 2, f).
		* x, y is overhead of array size
		*/
		HAS_MEMBER_FN_DECL(operator new, new);
		HAS_MEMBER_FN_DECL(operator new[], array_new);

		template<typename T> struct has_default_new:has_new<T, void*, std::size_t>{};

		template<typename T> struct has_nothrow_default_new :has_new<T, void*, size_t, const std::nothrow_t&> {};
		template<typename T, typename... Args> struct has_arg_new:has_new<T, void*, ccdk::size_t, Args...> {};

		template<typename T> static constexpr bool has_default_new_v = has_default_new<T>::value;
		template<typename T> static constexpr bool has_nothrow_default_new_v = has_nothrow_default_new<T>::value;
		template<typename T, typename... Args> static constexpr bool has_arg_new_v = has_arg_new<T,Args...>::value;
	}
}