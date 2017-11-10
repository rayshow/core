#pragma once

#include<core/mpl/base/int_.h>

namespace core
{
	namespace mpl
	{
		template<typename... Args> struct add_;

		template<typename T> struct add_<T> :public T {};

		template<typename T, typename... Args> struct add_<T, Args...> :
			public int_<T::value + add_<Args...>::value> {};
	}
}