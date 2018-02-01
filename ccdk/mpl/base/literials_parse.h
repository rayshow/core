#pragma once

#include<ccdk/mpl/mpl_module.h>

ccdk_namespace_mpl_start

	constexpr int parse_char(char a) noexcept
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

	constexpr long double exp10(int exp)
	{
		long double val = 1.0L;
		if (exp > 0) { while (exp--) val *= 10.0L; }
		else if (exp < 0) { while (exp++) val /= 10.0L; }
		return val;
	}

	template<int size>
	constexpr long double parse_float(const char(&literials)[size])
	{
		// 1.11f / 1.11 / 1e-5f / 1e2f  float
		// 
		int len = size;
		int dot_pos = len;
		int e_pos = len;
		for (int i = 0; i < len; ++i)
		{
			if (literials[i] == '.') dot_pos = i;
			if (literials[i] == 'e') e_pos = i;
		}

		int exp = 0;
		bool negtive_exp = false;
		int e_start = e_pos + 1;

		//is exp mode 
		if (e_pos != len)
		{
			if (literials[e_pos + 1] == '-')
			{
				negtive_exp = true;
				++e_start;
			}
			//112
			int multiplier = 1LL;
			for (int i = len - 1; i >= e_start; --i)
			{
				exp += parse_char(literials[i]) * multiplier;
				multiplier *= 10;
			}
			if (negtive_exp) exp = -exp;
		}

		long double val = 0.0L;
		long double multiplier = 1.0L;
		if (dot_pos != len) multiplier = exp10(dot_pos - e_pos + 1);

		for (int i = e_pos - 1; i >= 0; --i)
		{
			if (literials[i] != '.')
			{
				val += parse_char(literials[i]) * multiplier;
				multiplier *= 10.0L;
			}
		}

		return val * exp10(exp);
	}

#define ccdk_literial_parse_int(args)   parse_integer<sizeof...(args)>({ args... })
#define ccdk_literial_parse_float(args) parse_float<sizeof...(args)>({ args... })

ccdk_namespace_mpl_end
