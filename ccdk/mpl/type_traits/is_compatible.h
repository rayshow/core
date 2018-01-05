#pragma once

#include<ccdk/mpl/type_traits/declval.h>
#include<ccdk/mpl/type_traits/is_fundamental.h>
#include<ccdk/mpl/base/logic_.h>
#include<ccdk/mpl/type_traits/is_void.h>

namespace ccdk
{
	namespace mpl
	{
		namespace impl
		{
			template<typename T, typename P, typename = decltype(false ? declval<T>() : declval<P>())>
			inline constexpr bool test_compatible(int) { return true; }

			template<typename T, typename P>
			inline constexpr bool test_compatible(...) { return false; }
		}

		template<typename T, typename P, typename... Args>
		struct is_compatible :public and_< is_compatible<T, P>, is_compatible<P, Args...> > {};

		template<typename T, typename P> struct is_compatible<T,P> :and_< not_<is_void<T>>, not_<is_void<P>>, bool_< impl::test_compatible<T,P>(0)> > {};
		
		template<typename T, typename P, typename... Args>
		static constexpr bool is_compatible_v = is_compatible<T, P, Args...>::value;
	}
}