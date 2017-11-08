#pragma once


#include<core/mpl/base/bool_.h>

namespace core
{
	namespace mpl
	{
		//lazy
		template<typename T> struct is_lref : public false_ {};
		template<typename T> struct is_lref<T&> :public true_ {};

		template<typename T> constexpr bool is_lref_v = is_lref<T>::value;
	}
}