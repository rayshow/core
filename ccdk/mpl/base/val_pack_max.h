
#pragma once

#include<ccdk/type.h>
#include<ccdk/mpl/base/val_pack.h>
#include<ccdk/mpl/base/op.h>
#include<ccdk/mpl/base/integer_.h>


namespace ccdk
{
	namespace mpl
	{
		template<typename T, T v, T... args>
		struct val_pack_max :integer_<T, max2(v, val_pack_max<T,args...>::value)> {};

		template<typename T, T v>
		struct val_pack_max<T, v> :integer_<T, v> {};
		
		template<typename T, T... args>
		static constexpr  T val_pack_max_v = val_pack_max< T, args...>::value;
	}
}