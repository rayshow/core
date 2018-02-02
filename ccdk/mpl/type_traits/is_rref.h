#pragma once


#include<ccdk/mpl/base/type_.h>
#include<ccdk/mpl/type_traits/traits_case.h>

namespace ccdk
{
	namespace mpl
	{
		//lazy
		template<typename T> struct is_rref : public false_ {};
		template<typename T> struct is_rref<T&&> :public true_ {};
		template<typename T> constexpr bool is_rref_v = is_rref<T>::value;

		CCDK_TT_SINGLE_RANK(is_rref);
	}
}