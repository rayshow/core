#pragma once

#include<ccdk/mpl/base/type_.h>

namespace ccdk
{
	namespace mpl
	{
		template<typename T> struct is_unbound_array :public false_ {};
		template<typename T> struct is_unbound_array<T[]> :public true_ {};
		template<typename T> static constexpr bool is_unbound_array_v = is_unbound_array<T>::value;

		template<typename T> struct is_array : public false_ {};
		template<typename T> struct is_array<T[]> :public true_ {};
		template<typename T, unsigned int N> struct is_array<T[N]> :public true_ {};
		template<typename T> constexpr bool is_array_v = is_array<T>::value;

	}
}