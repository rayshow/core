
#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/type_.h>

ccdk_namespace_mpl_start

/* convert to common type to do operation */
template<typename Ret> CCDK_FORCEINLINE constexpr Ret cadd(Ret a, Ret b) noexcept { return a + b; }
template<typename Ret> CCDK_FORCEINLINE constexpr Ret csub(Ret a, Ret b) noexcept { return a - b; }
template<typename Ret> CCDK_FORCEINLINE constexpr Ret cmul(Ret a, Ret b) noexcept { return a * b; }
template<typename Ret> CCDK_FORCEINLINE constexpr Ret cdiv(Ret a, Ret b) noexcept { return a / b; }
template<typename Ret> CCDK_FORCEINLINE constexpr Ret cmod(Ret a, Ret b) noexcept { return a / b; }
template<typename Ret> CCDK_FORCEINLINE constexpr Ret cshl(Ret a, Ret b) noexcept { return a << b; }
template<typename Ret> CCDK_FORCEINLINE constexpr Ret cshr(Ret a, Ret b) noexcept { return a >> b; }
template<typename Ret> CCDK_FORCEINLINE constexpr Ret cbitor(Ret a, Ret b) noexcept { return a | b; }
template<typename Ret> CCDK_FORCEINLINE constexpr Ret cbitand(Ret a, Ret b) noexcept { return a & b; }
template<typename C> CCDK_FORCEINLINE constexpr bool cle(C a, C b) noexcept { return a < b; }
template<typename C> CCDK_FORCEINLINE constexpr bool cge(C a, C b) noexcept { return a > b; }
template<typename Ret> CCDK_FORCEINLINE constexpr Ret cneg(Ret a) noexcept { return -a; }

namespace base_impl {
	template<uint32 Size> struct shift_bit_mpl;
	template<> struct shift_bit_mpl<8> :mpl::uint32_<3> {};
	template<> struct shift_bit_mpl<16> :mpl::uint32_<4> {};
	template<> struct shift_bit_mpl<32> :mpl::uint32_<5> {};
	template<> struct shift_bit_mpl<64> :mpl::uint32_<6> {};
}

template<typename T, typename Ret> CCDK_FORCEINLINE constexpr Ret shl_type(Ret v) noexcept {
	return v << Ret(base_impl::shift_bit_mpl<sizeof(T) * 8>::value);
}

template<typename T, typename Ret> CCDK_FORCEINLINE constexpr Ret shr_type(Ret v) noexcept {
	return v >> Ret(base_impl::shift_bit_mpl<sizeof(T) * 8>::value);
}

template<typename T, typename Ret> CCDK_FORCEINLINE constexpr Ret mod_type(Ret v) noexcept {
	return v & (shl_type<T, T>(1) - T(1));
}

template<typename T, typename Ret> CCDK_FORCEINLINE constexpr Ret div_type(Ret v) noexcept {
	return v & ~(shl_type<T, T>(1) - T(1));
}

template<uint32 bits, typename Ret> CCDK_FORCEINLINE constexpr Ret shl_bits(Ret v) noexcept {
	return v << Ret(base_impl::shift_bit_mpl<bits>::value);
}
template<uint32 bits, typename Ret> CCDK_FORCEINLINE constexpr Ret shr_bits(Ret v) noexcept {
	return v >> Ret(base_impl::shift_bit_mpl<bits>::value);
}

ccdk_namespace_mpl_end
