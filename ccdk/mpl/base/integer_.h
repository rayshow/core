#pragma once

namespace ccdk
{
	namespace mpl
	{
		template<bool> struct bool_;

		template<typename T, T v>
		struct integer_
		{
			constexpr integer_() = default;
			typedef integer_<T, v> type;
			typedef T value_type;
			static constexpr T value = v;
			constexpr operator T() const
			{
				return v;
			}
		};

		template<typename T, T v> constexpr integer_<T, v> integer_c{};

		template<typename T> struct is_integer_t
		{
			static constexpr bool value = true;
		}; 

		template<typename T, T v> struct is_integer_t< integer_<T, v> >
		{
			static constexpr bool value = false;
		};




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
				return integer_c< long long, parse_integer<sizeof...(args)>({ args... }) >;
			}

			//for container[ i_th ]
			template<char... args>
			constexpr auto operator""_th()
			{
				return integer_c<int, parse_integer<sizeof...(args)>({ args... }) >;
			}
		}
	}
}