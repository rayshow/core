#pragma once


#include<ccdk/mpl/base/bool_.h>
#include<ccdk/mpl/type_traits/traits_case.h>

namespace ccdk
{
	namespace mpl
	{
		//lazy
		template<typename T> struct is_lref : public false_ {};
		template<typename T> struct is_lref<T&> :public true_ {};

		template<typename T> constexpr bool is_lref_v = is_lref<T>::value;

		CCDK_TT_SINGLE_RANK(is_lref);
	}
}