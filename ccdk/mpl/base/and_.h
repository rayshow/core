#pragma once

#include<ccdk/mpl/base/bool_.h>
#include<ccdk/mpl/base/derive_if.h>

namespace ccdk
{
	namespace mpl
	{
		//default 
		template<typename... Args> struct and_ :public false_ {};

		//only one element
		template<typename T> struct and_<T> :public T {};

		//at least 2-elements
		template<typename T1, typename T2, typename... Args> struct and_<T1, T2, Args...>
			: public derive_if<T1, and_<T2, Args...>, false_ >{};

		template<typename... Args> using and_t = typename and_<Args...>::type;
		template<typename... Args> static constexpr bool and_v = and_<Args...>::value;

	}
}