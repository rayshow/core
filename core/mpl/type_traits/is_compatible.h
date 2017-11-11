#pragma once

#include<core/mpl/type_traits/declval.h>
#include<core/mpl/type_traits/is_fundamental.h>
#include<core/mpl/base/not_.h>
#include<core/mpl/base/and_.h>
#include<core/mpl/type_traits/is_void.h>

namespace core
{
	namespace mpl
	{
		template<typename T, typename P>
		static constexpr bool test_compatible(T*, P*, decltype(false ? declval<T>() : declval<P>()) *ptr = 0) { return true; }

		template<typename T, typename P>
		static constexpr bool test_compatible(...) { return false;  }

		template<typename T, typename P, typename... Args>
		struct is_compatible :public and_< is_compatible<T, P>, is_compatible<P, Args...> > {};

		template<typename T, typename P> struct is_compatible<T,P> :and_< not_<is_void<T>>, not_<is_void<P>>, bool_< test_compatible<T,P>(0,0)> > {};
		
		template<typename T, typename P, typename... Args>
		static constexpr bool is_compatible_v = is_compatible<T, P, Args...>::value;
	}
}