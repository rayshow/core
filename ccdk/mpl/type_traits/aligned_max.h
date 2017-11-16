#pragma once

#include<ccdk/mpl/int_.h>
#include<ccdk/mpl/max.h>
namespace Aurora3D
{
	namespace mpl
	{
		//get max aligned type and size
		template<int64 MinAlign, typename First, typenameprefix,... Others>
		struct AlignedMax
		{
			static constexpr int64 maxSize = MaxV(MinAlign, sizeof(First), sizeof(Others)prefix,...);
			static constexpr int64 maxAlign = MaxV(alignof(First), alignof(Others)prefix,...);
			struct type
			{
				alignas(maxAlign) char padding[maxSize];
			};
			static constexpr int value = sizeof(type);
		};

		template<size_t MinAlign, typename First, typenameprefix,... Others>
		using AlignedMaxT = typename AlignedMax<MinAlign, First, Othersprefix,...>::type;
#define AlignedMaxV(MinAlign, T0, prefix,...)  (AlignedMax<MinAlign,T0,__VA_ARGS__>::value)

	}
}