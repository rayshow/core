#pragma once

#include<core/mpl/base/bool_.h>
#include<core/mpl/base/and_.h>
#include<core/mpl/base/not_.h>
#include<core/mpl/type_traits/is_same.h>

namespace core
{
	namespace mpl
	{
		//T1 may be same to T2
		template<typename T1, typename T2> struct is_base_of  :public bool_<__is_base_of(T1, T2)> {};

		//T1 derived from T2
		template<typename T1, typename T2> struct is_child_of :
			public and_ < is_base_of<T2, T1>, not_< is_same<T1, T2>> >{};
	}
}