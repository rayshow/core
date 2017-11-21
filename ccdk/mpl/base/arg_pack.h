#pragma once

#include<ccdk/mpl/base/bool_.h>
#include<ccdk/mpl/base/condi_derive.h>

namespace ccdk
{
	namespace mpl
	{
		//store Args...
		template<typename... Args>
		struct arg_pack {};

		//split Args... by index and ingore length element
		template< unsigned int index, unsigned int length, typename T, typename U = arg_pack<> >
		struct arg_pack_split
		{
			static_assert(index >= 0 && length >= 0, "index and length need to be greater equal then 0");
		};

		template< unsigned int index, unsigned int length, typename T, typename... TArgs, typename... UArgs>
		struct arg_pack_split< index, length, arg_pack<T, TArgs...>, arg_pack<UArgs...>>
			:arg_pack_split<index - 1, length, arg_pack<TArgs...>, arg_pack<UArgs..., T>> {};

		template< unsigned int length, typename T, typename... TArgs, typename... UArgs>
		struct arg_pack_split<0, length, arg_pack<T, TArgs...>, arg_pack<UArgs...>>
			:arg_pack_split<0, length -1, arg_pack<TArgs...>, arg_pack<UArgs...>>{};

		template<typename T, typename... TArgs, typename... UArgs>
		struct arg_pack_split<0, 1, arg_pack<T, TArgs...>, arg_pack<UArgs...>>
		{
			typedef arg_pack<UArgs...> head;
			typedef arg_pack<TArgs...> tail;
		};

		template<typename T, typename... TArgs, typename... UArgs>
		struct arg_pack_split<0, 0, arg_pack<T, TArgs...>, arg_pack<UArgs...>>
		{
			typedef arg_pack<UArgs...> head;
			typedef arg_pack<T, TArgs...> tail;
		};
	}
}