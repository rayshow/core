#pragma once

#include<ccdk/mpl/base/type_.h>
#include<ccdk/mpl/type_traits/is_same.h>
#include<ccdk/mpl/type_traits/remove_cv.h>
namespace ccdk
{
	namespace mpl
	{
		//ingore const volatile and &,&&
		template<typename A, typename B> struct is_base_same :
			public is_same< remove_cv_t<A> &, remove_cv_t<B> &> {};

		template<typename A, typename B>
		constexpr bool is_base_same_v = is_base_same<A, B>::value;

	}
}