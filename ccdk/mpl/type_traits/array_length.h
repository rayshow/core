#pragma once

#include<ccdk/mpl/base/int_.h>

namespace ccdk
{
	namespace mpl
	{
		template<typename T> struct array_len :public int_<0> { };
		template<typename T, int len> struct array_len<T[len]>: public int_<len>{};
		template<typename T> using  array_len_t = typename array_len<T>::type;
		template<typename T> constexpr int array_len_v = array_len<T>::value;
	}
}