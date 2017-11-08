#pragma once

#include<Core/type.h>
#include<Core/mpl/bit_or.h>
#include<Core/mpl/bit_shift_left.h>

namespace Aurora3D
{
	namespace mpl
	{
		// Lazy
		// value = 1 << V1 | ( 1<< V2 ) | (1<<V3) | ...
		template<uint32 V1, uint32... VS> struct IntBitCompose
			:public BitOr< Int_< (1 << V1) >, IntBitCompose< VS...> > {};
		template<uint32 V> struct IntBitCompose<V> :Int_< (1 << V) > {};

		// Lazy
		// value = 1 << T1::value | ( 1<< T2::value ) | (1<<T3::value) | ...
		template<typename T1, typename... TS> struct BitCompose
			:public BitOr< Shl<int1_, T1>, BitCompose< TS...> > {};
		template<typename T> struct BitCompose<T> :public Shl< int1_, T> {};
	}
}