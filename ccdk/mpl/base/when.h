#pragma once

#include<ccdk/mpl/base/and_.h>
#include<ccdk/mpl/base/or_.h>

namespace ccdk
{
	namespace mpl
	{


		//template syntax suger
		template<bool> struct when_impl;
		using when = when_impl<true>;
		template<bool condi> using match_default = when_impl<condi>;

		template<typename T> using match = when_impl<T::value>;
		template<typename T, typename... Args> using match_both = when_impl< and_<T,Args...>::value >;
		template<typename T, typename... Args> using match_one = when_impl< or_<T, Args...>::value >;
	}
}