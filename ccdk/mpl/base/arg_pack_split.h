#pragma once

#include<ccdk/type.h>
#include<ccdk/mpl/base/bool_.h>
#include<ccdk/mpl/base/derive_if.h>
#include<ccdk/mpl/base/if_.h>
#include<ccdk/mpl/base/arg_pack.h>

namespace ccdk
{
	namespace mpl
	{
		//split Args... by index and ingore length element
		template< uint32 index, uint32 length, typename T, typename U = arg_pack<> >
		struct arg_pack_split
		{
			static_assert(index >= 0 && length >= 0, "index and length need to be greater equal then 0");
		};


		template< uint32 index, uint32 length, typename T, typename... TArgs, typename... UArgs>
		struct arg_pack_split< index, length, arg_pack<T, TArgs...>, arg_pack<UArgs...>>
			: arg_pack_split<index - 1, length, arg_pack<TArgs...>, arg_pack<UArgs..., T>> {};

		// index == 0
		template< uint32 length, typename T, typename... TArgs, typename... UArgs>
		struct arg_pack_split<0, length, arg_pack<T, TArgs...>, arg_pack<UArgs...>>
			:arg_pack_split<0, length - 1, arg_pack<TArgs...>, arg_pack<UArgs...>> {};

		// index == 0 && length == 1
		template<typename T, typename... TArgs, typename... UArgs>
		struct arg_pack_split<0, 1, arg_pack<T, TArgs...>, arg_pack<UArgs...>>
		{
			typedef arg_pack<UArgs...> head;
			typedef arg_pack<TArgs...> tail;
			typedef arg_pack<UArgs..., TArgs...> type;
		};

		// index == 0 && length == 0
		template<typename T, typename... TArgs, typename... UArgs>
		struct arg_pack_split<0, 0, arg_pack<T, TArgs...>, arg_pack<UArgs...>>
		{
			typedef arg_pack<UArgs...> head;
			typedef arg_pack<T, TArgs...> tail;
			typedef arg_pack<UArgs..., T, TArgs...> type;
		};
	}
}