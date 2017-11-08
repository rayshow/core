#pragma once

#include<core/mpl/base/bool_.h>

namespace core
{
	namespace mpl
	{
		template<typename T> struct is_array : public false_ {};
		template<typename T> struct is_array<T[]> :public true_ {};
		template<typename T, unsigned int N> struct is_array<T[N]> :public true_ {};
		template<typename T> constexpr bool is_array_v = is_array<T>::value;

	}
}