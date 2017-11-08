#pragma once

#include<core/mpl/base/bool_.h>
#include<core/mpl/type_traits/is_same.h>
#include<core/mpl/type_traits/remove_cv.h>
namespace core
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