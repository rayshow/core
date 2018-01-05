
#pragma once

#include<ccdk/type.h>
#include<ccdk/mpl/base/val_pack.h>

namespace ccdk
{
	namespace mpl
	{
		namespace base_detail
		{
			template<typename T, typename P, uint32 i, T v, bool found>
			struct val_pack_find_impl;

			template<typename T, uint32 i, T v, T c, T... args>
			struct val_pack_find_impl< T, val_pack<T, c, args...>, i, v, false> 
				: val_pack_find_impl<T, val_pack<T, args...>, i + 1, v, c == v> {};

			template<typename T, uint32 i, T v>
			struct val_pack_find_impl<T, val_pack<T>, i, v, false> :uint32_<i> {};

			template<typename T, uint32 i, T v, T c, T... args>
			struct val_pack_find_impl<T, val_pack<T, c, args...>, i, v, true> :uint32_<i - 1> {};
		}		

		template<typename T, T v, typename P>
		static constexpr uint32 val_pack_find = base_detail::val_pack_find_impl<T, P, 0, v, false>::value;

		template<typename T, T v, T... args>
		static constexpr uint32 val_pack_find_exp = base_detail::val_pack_find_impl<T, val_pack<T, args...>, 0, v, false>::value;
	}
}