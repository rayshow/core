#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/literials_parse.h>

ccdk_namespace_mpl_start

template<typename T, T v>
struct compile_t
{
	typedef compile_t<T, v> type;
	typedef T               value_type;
	static constexpr T value = v;
	constexpr compile_t() = default;
	constexpr operator T() const { return v; }

	/* add two compile_t */
	template<typename P> struct add   : compile_t< decltype(v +  P::value),  (v +  P::value)> {};
	template<typename P> struct sub   : compile_t< decltype(v -  P::value),  (v -  P::value)> {};
	template<typename P> struct mul   : compile_t< decltype(v *  P::value),  (v *  P::value)> {};
	template<typename P> struct div   : compile_t< decltype(v /  P::value),  (v /  P::value)> {};
	template<typename P> struct mod   : compile_t< decltype(v %  P::value),  (v %  P::value)> {};
	template<typename P> struct shl   : compile_t< decltype(v << P::value),  (v << P::value)> {};
	template<typename P> struct shr   : compile_t< decltype(v >> P::value),  (v >> P::value)> {};
	template<typename P> struct band  : compile_t< decltype(v &  P::value),  (v &  P::value)> {};
	template<typename P> struct bor   : compile_t< decltype(v |  P::value),  (v |  P::value)> {};
	template<typename P> struct equal : compile_t< decltype(v == P::value),  (v == P::value)> {};
	template<typename P> struct less  : compile_t< decltype(v <  P::value),  (v <  P::value)> {};

	/* unary  */
	struct next     :compile_t<T, ++v> {};
	struct front    :compile_t<T, --v> {};
};

/* bool wrap */
template<bool v>
using bool_  = compile_t<bool, v>;
using false_ = compile_t<bool, false>;
using true_  = compile_t<bool, true>;
constexpr false_ false_c{};
constexpr true_  true_c{};

/* int wrap  */
template<uint8 v>  using uint8_  = compile_t<uint8, v>;
template<uint16 v> using uint16_ = compile_t<uint16, v>;
template<uint32 v> using uint32_ = compile_t<uint32, v>;
template<uint64 v> using uint64_ = compile_t<uint64, v>;

template<int8 v>   using int8_   = compile_t<int8, v>;
template<int16 v>  using int16_  = compile_t<int16, v>;
template<int32 v>  using int32_  = compile_t<int32, v>;
template<int64 v>  using int64_  = compile_t<int64, v>;

template<ptr::size_t v> using size_ = compile_t<ptr::size_t, v>;
template<ptr::diff_t v> using diff_ = compile_t<ptr::diff_t, v>;

/* template constexpr int */
template<int8 v>  constexpr int8_<v>  int8_c{};
template<int16 v> constexpr int16_<v> int16_c{};
template<int32 v> constexpr int32_<v> int32_c{};
template<int64 v> constexpr int64_<v> int64_c{};

template<int8 v>  constexpr uint8_<v>  uint8_c{};
template<int16 v> constexpr uint16_<v> uint16_c{};
template<int32 v> constexpr uint32_<v> uint32_c{};
template<int64 v> constexpr uint64_<v> uint64_c{};
template<typename T, T v> constexpr compile_t<T,v> compile_c{};

template<ptr::size_t v> constexpr size_<v> size_c{};
template<ptr::diff_t v> constexpr diff_<v> diff_c{};

/* char wrap */
template<achar c>  using achar_  = compile_t<achar, c>;
template<wchar c>  using wchar_  = compile_t<wchar, c>;
template<char16 c> using char16_ = compile_t<char16, c>;
template<char32 c> using char32_ = compile_t<char32, c>;

template<achar  c> constexpr achar_<c>  achar_c{};
template<wchar  c> constexpr wchar_<c>  wchar_c{};
template<char16 c> constexpr char16_<c> char16_c{};
template<char32 c> constexpr char32_<c> char32_c{};

/* template type null_ */
struct null_ {};
template<typename T> struct is_null :public false_ {};
template<> struct is_null< null_> : public true_ {};



namespace literals
{
	/* constexpr integer */
	template<char... args>
	constexpr auto operator""_ci()
	{
		return compile_c< int64, parse_integer<sizeof...(args)>({ args... }) >;
	}

	//for container[ i_th ]
	template<char... args>
	constexpr auto operator""_th()
	{
		return compile_c< int32, parse_integer<sizeof...(args)>({ args... }) >;
	}
}

ccdk_namespace_mpl_end