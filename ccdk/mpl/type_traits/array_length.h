#pragma once

#include<ccdk/mpl/base/type_.h>

namespace ccdk
{
	namespace mpl
	{
		template<typename T> struct array_len :public int32_<0> { };
		template<typename T, int len> struct array_len<T[len]>: public int32_<len>{};
		template<typename T> struct array_len<T&> :array_len<T> {};
		template<typename T> struct array_len<T&&> :array_len<T> {};

		template<typename T> using  array_len_t = typename array_len<T>::type;
		template<typename T> constexpr int array_len_v = array_len<T>::value;
		
	}
}