#pragma once

#include<ccdk/type.h>
#include<ccdk/mpl/base/val_pack.h>


namespace ccdk
{
	namespace mpl
	{
		namespace base_detail
		{
			template<typename P, uint32 start, uint32 len, typename T = int32, typename U = val_pack<T>>
			struct val_pack_split_impl;

			template< uint32 start, uint32 len, typename T, T c, T... args1, T... args2>
			struct val_pack_split_impl< val_pack<T, c, args1...>, start, len, T, val_pack<T, args2...>>
				: val_pack_split_impl< val_pack<T, args1...>, start - 1, len, T, val_pack<T, args2..., c>> {};


			//
			template< uint32 len, typename T, T c, T... args1, T... args2>
			struct val_pack_split_impl< val_pack<T, c, args1...>, 0, len, T, val_pack<T, args2...>>
				: val_pack_split_impl < val_pack<T, args1...>, 0, len - 1, T, val_pack<T, args2...>> {};

			// >=1 len
			template< typename T, T c, T... args1, T... args2>
			struct val_pack_split_impl< val_pack<T, c, args1...>, 0, 1, T, val_pack<T, args2...>>
			{
				typedef val_pack<T, args2...> head;
				typedef val_pack<T, args1...> tail;
				typedef val_pack<T, args2..., args1...> type;
			};
			// 0 len
			template<typename T, T c, T... args1, T... args2>
			struct val_pack_split_impl< val_pack<T, c, args1...>, 0, 0, T, val_pack<T, args2...>>
			{
				typedef val_pack<T, args2...> head;
				typedef val_pack<T, c, args1...> tail;
				typedef val_pack<T, c, args1..., args2...> type;
			};

			//empty P
			template< uint32 start, uint32 len, typename T, T... args>
			struct val_pack_split_impl< val_pack<T>, start, len, T, val_pack<T, args...>>
			{
				typedef val_pack<T, args...> head;
				typedef val_pack<T> tail;
				typedef val_pack<T, args...> type;
			};
		}
		

		template<typename T, uint32 start, uint32 end, typename P>
		using val_pack_split = base_detail::val_pack_split_impl< P, start, end - start, T>;

		template<typename T, uint32 start, uint32 end, T... args>
		using val_pack_split_exp = base_detail::val_pack_split_impl< val_pack<T, args...>, start, end - start, T>;

	}
}
