#pragma once

#include<ccdk/mpl/mpl_module.h>

ccdk_namespace_mpl_start

template<typename T, T v>
struct compile_t
{
	constexpr compile_t() = default;
	typedef compile_t<T, v> type;
	typedef T value_type;
	static constexpr T value = v;
	constexpr operator T() const { return v; }
};

/* template int alias */
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

/* template const int */
template<int8 v>  constexpr int8_<v>  int8_c{};
template<int16 v> constexpr int16_<v> int16_c{};
template<int32 v> constexpr int32_<v> int32_c{};
template<int64 v> constexpr int64_<v> int64_c{};

template<int8 v>  constexpr uint8_<v>  uint8_c{};
template<int16 v> constexpr uint16_<v> uint16_c{};
template<int32 v> constexpr uint32_<v> uint32_c{};
template<int64 v> constexpr uint64_<v> uint64_c{};
template<typename Int, Int v> constexpr compile_t<Int,v> compile_c{};

template<ptr::size_t v> constexpr size_<v> size_c{};
template<ptr::diff_t v> constexpr diff_<v> diff_c{};



namespace literals
{
	constexpr long long parse_char(char a) noexcept
	{
		char base = 0;
		if (a <= 'Z' && a >= 'A') base = 'A' - 10;
		else if (a <= 'z' && a >= 'a') base = 'a' - 10;
		else base = '0';
		return a - base;
	}

	template<int size>
	constexpr long long parse_integer(const char(&literals)[size]) noexcept
	{
		long long value = 0ll;
		long long base = 10;
		int offset = 0;
		if (size > 2)
		{
			bool start0 = literals[0] == '0';
			bool is_hex = start0 && literals[1] == 'x';
			bool is_bin = start0 && literals[1] == 'b';

			// 0xff
			if (is_hex) { base = 16; offset = 2; }
			// 0b01001111
			else if (is_bin) { base = 2; offset = 2; }
			// 0234123
			else if (start0) { base = 8; offset = 1; }
		}

		long long multiplier = 1;
		for (int i = size - 1; i >= offset; --i)
		{
			value += parse_char(literals[i]) * multiplier;
			multiplier *= base;
		}
		return value;
	}

	template<int size>
	constexpr long double parse_float(const char(&arr)[size])
	{
		// 1.0f   1e-5f  float
		return 1.0f;
	}

	//constexpr integer
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