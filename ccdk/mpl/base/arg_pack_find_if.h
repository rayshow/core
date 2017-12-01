#pragma once

#include<ccdk/type.h>
#include<ccdk/mpl/base/arg_pack.h>
#include<ccdk/mpl/base/bool_.h>
#include<ccdk/mpl/base/condi_derive.h>
#include<ccdk/mpl/base/if_.h>

namespace ccdk
{
	namespace mpl
	{
		template<typename T, uint32 i>
		struct found_at
		{
			typedef T type;
			static constexpr uint32 value = i;
		};


		template<typename T, typename Default, uint32 i, typename... Args> struct arg_pack_find_if;

		//loop
		template<typename T, typename Default, uint32 i, typename P, typename... Args>
		struct arg_pack_find_if< T, Default, i, P, Args...>
			:condi_derive<
				typename T::template apply<P>,
				found_at<P, i>,
				arg_pack_find_if<T, Default, i + 1, Args...>>
		{};

		//not found
		template<typename T, typename Default, uint32 i>
		struct arg_pack_find_if<T, Default, i>
		{
			typedef Default type;
			static constexpr uint32 value = i;
		};

		template<typename T, typename Default, typename... Args>
		using arg_pack_find_if_t = typename arg_pack_find_if<T, Default, 0, Args...>::type;

		template<typename T, typename Default, typename... Args>
		static constexpr uint32 arg_pack_find_if_v = arg_pack_find_if<T, Default, 0, Args...>::value;


	}
}