
#pragma once

#include<ccdk/type.h>
#include<ccdk/mpl/mcontainer/val_pack.h>


namespace ccdk
{
	namespace mpl
	{
		namespace base_detail
		{
			template<typename T, typename P, uint32 n, uint32 i = 0>
			struct val_pack_at_impl;

			template<typename T, uint32 n, uint32 i, T c, T... args>
			struct val_pack_at_impl<T, val_pack<T, c, args...>, n, i> 
				: val_pack_at_impl<T, val_pack<T, args...>, n, i + 1>
			{};

			template<typename T, uint32 n, T c, T... args>
			struct val_pack_at_impl<T, val_pack<T, c, args...>, n, n>
			{
				static constexpr T value = c;
			};
		}

		template<typename T, uint32 index, typename P>
		static constexpr  T val_pack_at = base_detail::val_pack_at_impl< T, P, index>::value;

		template<typename T, uint32 index, T... args>
		static constexpr  T val_pack_at_exp = base_detail::val_pack_at_impl< T, val_pack<T, args...>, index>::value;
	}
}