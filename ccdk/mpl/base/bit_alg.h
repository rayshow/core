#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/type_.h>

ccdk_namespace_mpl_start

template<typename T>  struct bit_alg {};

template<>  
struct bit_alg<uint8> {
	static constexpr uint32 flp2(uint32 x) noexcept {
		x = x | (x >> 1);
		x = x | (x >> 2);
		x = x | (x >> 4);
		return x - (x >> 1);
	}

	static constexpr uint32 clp2(uint32 x) noexcept {
		//ccdk_assert(x < 128);
		--x;
		x = x | (x >> 1);
		x = x | (x >> 2);
		x = x | (x >> 4);
		return x + 1;
	}
};


template<>
struct bit_alg<uint16> {
	static constexpr uint32 flp2(uint32 x) noexcept {
		x = x | (x >> 1);
		x = x | (x >> 2);
		x = x | (x >> 4);
		x = x | (x >> 8);
		return x - (x >> 1);
	}
	static constexpr uint32 clp2(uint32 x) noexcept {
		--x;
		x = x | (x >> 1);
		x = x | (x >> 2);
		x = x | (x >> 4);
		x = x | (x >> 8);
		return ++x;
	}
};


template<>
struct bit_alg<uint32> {
	static constexpr uint32 flp2(uint32 x) noexcept {
		x = x | (x >> 1);
		x = x | (x >> 2);
		x = x | (x >> 4);
		x = x | (x >> 8);
		x = x | (x >> 16);
		return x - (x >> 1);
	}
	static constexpr uint32 clp2(uint32 x) noexcept {
		--x;
		x = x | (x >> 1);
		x = x | (x >> 2);
		x = x | (x >> 4);
		x = x | (x >> 8);
		x = x | (x >> 16);
		return ++x;
	}
};

template<>
struct bit_alg<uint64> {
	static constexpr uint64 flp2(uint64 x) noexcept {
		x = x | (x >> 1);
		x = x | (x >> 2);
		x = x | (x >> 4);
		x = x | (x >> 8);
		x = x | (x >> 16);
		x = x | (x >> 32);
		return x - (x >> 1);
	}
	static constexpr uint64 clp2(uint64 x) noexcept {
		--x;
		x = x | (x >> 1);
		x = x | (x >> 2);
		x = x | (x >> 4);
		x = x | (x >> 8);
		x = x | (x >> 16);
		x = x | (x >> 32);
		return ++x;
	}
};

template<uint64 N>
struct clp2_ : uint64_< bit_alg<uint64>::clp2(N) > {};

template<uint64 N>
struct flp2_ : uint64_< bit_alg<uint64>::flp2(N) > {};

template<uint64 N> static constexpr uint64 clp2_v = clp2_<N>::value;
template<uint64 N> static constexpr uint64 flp2_v = flp2_<N>::value;


ccdk_namespace_mpl_end