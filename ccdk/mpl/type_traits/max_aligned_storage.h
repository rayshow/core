#pragma once

#include<ccdk/type.h>
#include<ccdk/mpl/mcontainer/val_pack_max.h>

namespace ccdk
{
	namespace mpl
	{
		//get max aligned type and size
		template< ptr::size_t MinAlign, ptr::size_t MinSize, typename T, typename... Args>
		struct max_aligned_storage
		{
			static constexpr ptr::size_t MaxSize = val_pack_max_v<ptr::size_t, MinSize, sizeof(T), sizeof(Args)...>;
			static constexpr ptr::size_t MaxAlign = val_pack_max_v< ptr::size_t, MinAlign, alignof(T), alignof(Args)...>;
			struct type
			{
				alignas(MaxAlign) uint8 data[MaxSize];
			};
			static constexpr int value = sizeof(type);
		};

		template<ptr::size_t MinAlign, ptr::size_t MinSize, typename... Args>
		using max_aligned_storage_t = typename max_aligned_storage<MinAlign, MinSize, Args...>::type;

		template<ptr::size_t MinAlign, typename... Args>
		constexpr ptr::size_t max_align_v = max_aligned_storage<MinAlign, 0, Args...>::value;

		template<ptr::size_t MinSize, typename... Args>
		constexpr ptr::size_t max_size_v = max_aligned_storage<0, MinSize, Args...>::value;


	}
}