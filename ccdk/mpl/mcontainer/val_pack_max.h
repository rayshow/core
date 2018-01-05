
#pragma once

#include<ccdk/type.h>

#include<ccdk/mpl/base/arithmatic_.h>
#include<ccdk/mpl/base/integer_.h>
#include<ccdk/mpl/mcontainer/val_pack.h>

namespace ccdk
{
	namespace mpl
	{
		template<typename T, T v, T... args>
		struct val_pack_max :compile_t<T, max2(v, val_pack_max<T,args...>::value)> {};

		template<typename T, T v>
		struct val_pack_max<T, v> :compile_t<T, v> {};
		
		template<typename T, T... args>
		static constexpr  T val_pack_max_v = val_pack_max< T, args...>::value;
	}
}