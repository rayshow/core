#pragma once
#include<ccdk/mpl/base/bool_.h>
#include<ccdk/mpl/base/derive_if.h>

namespace ccdk
{
	namespace mpl
	{
		//default false
		template<typename ... Args> struct or_:public false_ {};

		//have least one element
		template<typename T, typename... Args> 
		struct or_<T, Args...> 
			: public derive_if<T, true_, or_<Args...>>{};

		template<typename... Args> using or_t = typename or_<Args...>::type;
		template<typename... Args> constexpr bool or_v = or_<Args...>::value;
	}
}