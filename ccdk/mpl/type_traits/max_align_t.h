#pragma once

#include<ccdk/type.h>
#include<ccdk/mpl/base/val_pack_max.h>

namespace ccdk
{
	namespace mpl
	{
		//get max aligned type and size
		template< ptr::size_t MinAlign, typename T, typename... Args>
		struct max_align
		{
			static constexpr ptr::size_t MaxSize = val_pack_max_v<ptr::size_t, sizeof(T), sizeof(Args)...>;
			static constexpr ptr::size_t MaxAlign = val_pack_max_v< ptr::size_t, MinAlign, alignof(T), alignof(Args)...>;
			struct type
			{
				alignas(MaxAlign) uint8 data[MaxSize];
			};
			static constexpr int value = sizeof(type);
		};

		template<ptr::size_t MinAlign, typename... Args>
		using max_align_t = typename max_align<MinAlign, Args...>::type;

		template<ptr::size_t MinAlign, typename... Args>
		constexpr ptr::size_t max_align_v = max_align<MinAlign, Args...>::value;

	}
}