#pragma once

#include<ccdk/mpl/base/bool_.h>
#include<ccdk/mpl/base/condi_derive.h>
#include<ccdk/mpl/base/if_.h>

namespace ccdk
{
	namespace mpl
	{
		//store Args...
		template<typename... Args>
		struct arg_pack {};

		template<typename... Args>
		struct arg_pack_first;

		template<typename T, typename... Args>
		struct arg_pack_first<T, Args...> { typedef T type; };

		template<typename... Args>
		using arg_pack_first_t = typename arg_pack_first<Args...>::type;

		//split Args... by index and ingore length element
		template< unsigned int index, unsigned int length, typename T, typename U = arg_pack<> >
		struct arg_pack_split
		{
			static_assert(index >= 0 && length >= 0, "index and length need to be greater equal then 0");
		};

		template< unsigned int index, unsigned int length, typename T, typename... TArgs, typename... UArgs>
		struct arg_pack_split< index, length, arg_pack<T, TArgs...>, arg_pack<UArgs...>>
			: arg_pack_split<index - 1, length, arg_pack<TArgs...>, arg_pack<UArgs..., T>> {};

		template< unsigned int length, typename T, typename... TArgs, typename... UArgs>
		struct arg_pack_split<0, length, arg_pack<T, TArgs...>, arg_pack<UArgs...>>
			:arg_pack_split<0, length -1, arg_pack<TArgs...>, arg_pack<UArgs...>>{};

		template<typename T, typename... TArgs, typename... UArgs>
		struct arg_pack_split<0, 1, arg_pack<T, TArgs...>, arg_pack<UArgs...>>
		{
			typedef arg_pack<UArgs...> head;
			typedef arg_pack<TArgs...> tail;
			typedef arg_pack<UArgs..., TArgs...> type;
		};

		template<typename T, typename... TArgs, typename... UArgs>
		struct arg_pack_split<0, 0, arg_pack<T, TArgs...>, arg_pack<UArgs...>>
		{
			typedef arg_pack<UArgs...> head;
			typedef arg_pack<T, TArgs...> tail;
			typedef arg_pack<UArgs...,T, TArgs...> type;
		};

		template<typename T, uint32 i>
		struct find_identity
		{
			typedef T type;
			static constexpr uint32 value = i;
		};


		template<typename T, typename Default, uint32 i, typename... Args>
		struct arg_pack_find_if;

		//loop
		template<typename T, typename Default, uint32 i, typename P, typename... Args>
		struct arg_pack_find_if< T, Default, i,  P, Args...>
			:condi_derive<
				typename T::template apply<P>,
				find_identity<P,i>,
				arg_pack_find_if<T, Default, i+1, Args...>>{};

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