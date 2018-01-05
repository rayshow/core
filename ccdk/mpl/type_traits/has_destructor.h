#pragma once

#include<ccdk/compile.h>
#include<ccdk/mpl/base/bool_.h>
#include<ccdk/mpl/base/logic_.h>
#include<ccdk/mpl/type_traits/declval.h>

namespace ccdk
{
	namespace mpl
	{
#if defined(CCDK_COMPILER_MSVC) || defined(CCDK_COMPILER_CLANG)
		template<typename T> struct has_destructor:public bool_<__is_destructible(T)> {};
		template<typename T> struct has_nothrow_destructor :public bool_<__is_nothrow_destructible(T)> {};
#else
		template<typename T>
		struct has_destructor_helper
		{
			template<typename P, typename = decltype(makeval<P>().~P())>
			static constexpr bool sfinae(int) { return true; }

			template<typename P>
			static constexpr bool sfinae(...) { return false; }

			static constexpr bool value = sfinae<T>(0);
		};

		template<typename T>
		struct has_nothrow_destructor_helper
		{
			static constexpr bool value = noexcept(makeval<T>().~T());
		};
		template<typename T> struct has_destructor :has_destructor_helper<T> {};
		template<typename T> struct has_nothrow_destructor:and_<has_destructor_helper<T>, has_nothrow_destructor_helper<T>>{};

#endif
		template<typename T> struct has_trivial_destructor : bool_<__has_trivial_destructor(T)> {};
		template<typename T> struct has_virtual_destructor : bool_<__has_virtual_destructor(T)>{ };
		template<typename T> static constexpr bool has_destructor_v = has_destructor<T>::value;
		template<typename T> static constexpr bool has_trivial_destructor_v = has_trivial_destructor<T>::value;
		template<typename T> static constexpr bool has_nothrow_destructor_v = has_nothrow_destructor<T>::value;
	}
}