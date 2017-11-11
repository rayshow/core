#pragma once

#include<core/mpl/type_traits/decay.h>
#include<core/mpl/type_traits/declval.h>

namespace core
{
	namespace mpl
	{
		template<typename T, typename... Args>
		struct compatible_type :public compatible_type<T, typename compatible_type<Args...>::type> {};

		template<typename T>
		struct compatible_type<T> { typedef decay_t<T> type; };

		template<typename T1, typename T2>
		struct compatible_type<T1,T2>
		{
			typedef decay_t<decltype(false ? declval<T1>() : declval<T2>())> type;
		};

		template<typename T, typename... Args>
		using compatible_type_t = typename compatible_type<T, Args...>::type;
	}
}