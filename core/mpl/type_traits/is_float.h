#pragma once

#include<core/mpl/base/bool_.h>

namespace core
{
	namespace mpl
	{
		template<typename T> struct is_float :public false_{};
		template<> struct is_float<float> :public  true_ {};
		template<> struct is_float<double> :public true_ {};
		template<> struct is_float<long double> :public true_ {};
		template<typename T> struct is_float<T const> :public is_float<T> {};
		template<typename T> struct is_float<T volatile> :public is_float<T> {};
		template<typename T> struct is_float<T const volatile> :public is_float<T> {};


		template<typename T> constexpr bool is_float_v = is_float<T>::value;
	}
}
