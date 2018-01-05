#pragma once

#include<ccdk/type.h>
#include<ccdk/mpl/mcontainer/arg_pack.h>
#include<ccdk/mpl/type_traits/is_same.h>
#include<ccdk/mpl/base/bool_.h>
#include<ccdk/mpl/base/derive_if.h>
#include<ccdk/mpl/base/if_.h>

namespace ccdk
{
	namespace mpl
	{

		template<typename T>
		struct always_false
		{
			static constexpr bool value = true;
		};

		template<
			uint32 index,
			typename T,
			typename... Args
		>
			struct arg_pack_find_index;

		template<
			uint32 index,
			typename T, 
			typename P, 
			typename ... Args>
			struct arg_pack_find_index<index,T,P,Args...>
				: arg_pack_find_index<index + 1, T, Args...> {};


		template<uint32 index, typename T, typename... Args>
		struct arg_pack_find_index<index, T, T, Args...> {
			static constexpr uint32 value = index; 
		};

		template<uint32 index, typename T>
		struct arg_pack_find_index<index, T> {
			static_assert(always_false<T>::value, "type not found in args");
		};

		template<typename T, typename P,typename... Args>
		static constexpr uint32 arg_pack_find_index_v = arg_pack_find_index<0, T, P,Args...>::value;

	}
}