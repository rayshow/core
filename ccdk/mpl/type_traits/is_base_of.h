#pragma once

#include<ccdk/mpl/base/bool_.h>
#include<ccdk/mpl/base/logic_.h>
#include<ccdk/mpl/type_traits/is_same.h>
#include<ccdk/mpl/type_traits/remove_cv.h>

namespace ccdk
{
	namespace mpl
	{
		//T1 may be same to T2
		template<typename T1, typename T2> struct is_base_of  :public bool_<__is_base_of(T1, T2)> {};

		template<typename T1, typename T2> constexpr bool is_base_of_v = is_base_of<T1, T2>::value;
		
		//T1 derived from T2
		template<typename T1, typename T2> struct is_child_of :
			public and_ < is_base_of<T2, T1>, not_< is_same< remove_cv_t<T1>, remove_cv_t<T2> >> >{};

		template<typename T1, typename T2>
			constexpr bool is_child_of_v = is_child_of<T1, T2>::value;
	}
}