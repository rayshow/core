#pragma once


#include<core/mpl/base/bool_.h>

namespace core
{
	namespace mpl
	{
		//lazy
		template<typename T> struct is_rref : public false_ {};
		template<typename T> struct is_rref<T&&> :public true_ {};
		template<typename T> constexpr bool is_rref_v = is_rref<T>::value;

	}
}