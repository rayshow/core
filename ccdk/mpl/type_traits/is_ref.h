#pragma once

#include<ccdk/mpl/base/or_.h>
#include<ccdk/mpl/type_traits/is_lref.h>
#include<ccdk/mpl/type_traits/is_rref.h>

namespace ccdk
{
	namespace mpl
	{
		//lazy 
		template<typename T> struct is_ref :public or_< is_lref<T>, is_rref<T>> {};
		template<typename T> constexpr bool is_ref_v = is_ref<T>::value;
	}
}