#pragma once

#include<ccdk/mpl/base/bool_.h>
#include<ccdk/mpl/base/condi_derive.h>

namespace ccdk
{
	namespace mpl
	{
		template<typename... Args>
		struct arg_pack {};

		template< unsigned int index, unsigned int len, typename T, typename U = arg_pack<> >
		struct arg_pack_ingore_index;

		template< unsigned int count, unsigned int len, typename T, typename... TArgs, typename... UArgs>
		struct arg_pack_ingore_index< count, len, arg_pack<T, TArgs...>, arg_pack<UArgs...>>
			:arg_pack_ingore_index<count - 1, len, arg_pack<TArgs...>, arg_pack<UArgs..., T>> {};

		template< unsigned int len, typename T, typename... TArgs, typename... UArgs>
		struct arg_pack_ingore_index<0, len, arg_pack<T, TArgs...>, arg_pack<UArgs...>>
			:arg_pack_ingore_index<0, len-1, arg_pack<TArgs...>, arg_pack<UArgs...>>
		{
			
		};

		template<typename T, typename... TArgs, typename... UArgs>
		struct arg_pack_ingore_index<0, 1, arg_pack<T, TArgs...>, arg_pack<UArgs...>>
		{
			typedef arg_pack<UArgs...> head_pack;
			typedef arg_pack<TArgs...> tail_pack;
		};
	}
}