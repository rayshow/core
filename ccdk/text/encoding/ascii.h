#pragma once

#include<ccdk/mpl/util/addressof.h>
#include<ccdk/mpl/util/move.h>
#include<ccdk/mpl/iterator/ptr_iterator.h>
#include<ccdk/text/string_module.h>
#include<ccdk/text/string_fwd.h>
#include<ccdk/text/char_traits.h>

ccdk_namespace_string_start


template<>
struct encoding<ascii_encoding>
{
	struct ascii_category
	{
		static constexpr uint8 kDigit = 0x0001;
		static constexpr uint8 kXDigit = 0x0002;
		static constexpr uint8 kAlpha = 0x0004;
		static constexpr uint8 kCtrl = 0x0008;
		static constexpr uint8 kLower = 0x0010;
		static constexpr uint8 kUpper = 0x0020;
		static constexpr uint8 kSpace = 0x0040;
		static constexpr uint8 kPunct = 0x0080;
		static constexpr uint8 kCtrl_Space = kCtrl | kSpace;
		static constexpr uint8 kDigit_XDigit = kDigit | kXDigit;
		static constexpr uint8 kUpper_Alpha = kAlpha | kUpper;
		static constexpr uint8 kLower_Alpha = kAlpha | kLower;
		static constexpr uint8 kUpper_Alpha_XDigit = kUpper_Alpha | kXDigit;
		static constexpr uint8 kLower_Alpha_XDigit = kLower_Alpha | kXDigit;
	};

	static constexpr uint16 ascii_category_table[] =
	{
		/* mean Dec            bits         */
		/* NUL  0    */ ascii_category::kCtrl,
		/* SOH  1    */ ascii_category::kCtrl,
		/* STX  2    */ ascii_category::kCtrl,
		/* ETX  3    */ ascii_category::kCtrl,
		/* EOT  4    */ ascii_category::kCtrl,
		/* ENQ  5    */ ascii_category::kCtrl,
		/* ACK  6    */ ascii_category::kCtrl,
		/* BEL  7    */ ascii_category::kCtrl,
		/* BS   8    */ ascii_category::kCtrl,
		/* HT   9    */ ascii_category::kCtrl_Space,
		/* LF  10    */ ascii_category::kCtrl_Space,
		/* VT  11    */ ascii_category::kCtrl_Space,
		/* NP  12    */ ascii_category::kCtrl_Space,
		/* CR  13    */ ascii_category::kCtrl_Space,
		/* SO  14    */ ascii_category::kCtrl,
		/* SI  15    */ ascii_category::kCtrl,
		/*DLE  16    */ ascii_category::kCtrl,
		/*DC1  17    */ ascii_category::kCtrl,
		/*DC2  18    */ ascii_category::kCtrl,
		/*DC3  19    */ ascii_category::kCtrl,
		/*DC4  20    */ ascii_category::kCtrl,
		/*NAK  21    */ ascii_category::kCtrl,
		/*SYN  22    */ ascii_category::kCtrl,
		/*ETB  23    */ ascii_category::kCtrl,
		/*CAN  24    */ ascii_category::kCtrl,
		/* EM  25    */ ascii_category::kCtrl,
		/*SUB  26    */ ascii_category::kCtrl,
		/*ESC  27    */ ascii_category::kCtrl,
		/* FS  28    */ ascii_category::kCtrl,
		/* GS  29    */ ascii_category::kCtrl,
		/* RS  30    */ ascii_category::kCtrl,
		/* US  31    */ ascii_category::kCtrl,
		/* SP  32    */ ascii_category::kCtrl_Space,
		/* !   33    */ ascii_category::kPunct,
		/* "   34    */ ascii_category::kPunct,
		/* #   35    */ ascii_category::kPunct,
		/* $   36    */ ascii_category::kPunct,
		/* %   37    */ ascii_category::kPunct,
		/* &   38    */ ascii_category::kPunct,
		/* '   39    */ ascii_category::kPunct,
		/* (   40    */ ascii_category::kPunct,
		/* )   41    */ ascii_category::kPunct,
		/* *   42    */ ascii_category::kPunct,
		/* +   43    */ ascii_category::kPunct,
		/* ,   44    */ ascii_category::kPunct,
		/* -   45    */ ascii_category::kPunct,
		/* .   46    */ ascii_category::kPunct,
		/* /   47    */ ascii_category::kPunct,
		/* 0   48    */ ascii_category::kDigit_XDigit,
		/* 1   49    */ ascii_category::kDigit_XDigit,
		/* 2   50    */ ascii_category::kDigit_XDigit,
		/* 3   51    */ ascii_category::kDigit_XDigit,
		/* 4   52    */ ascii_category::kDigit_XDigit,
		/* 5   53    */ ascii_category::kDigit_XDigit,
		/* 6   54    */ ascii_category::kDigit_XDigit,
		/* 7   55    */ ascii_category::kDigit_XDigit,
		/* 8   56    */ ascii_category::kDigit_XDigit,
		/* 9   57    */ ascii_category::kDigit_XDigit,
		/* :   58    */ ascii_category::kPunct,
		/* ;   59    */ ascii_category::kPunct,
		/* <   60    */ ascii_category::kPunct,
		/* =   61    */ ascii_category::kPunct,
		/* >   62    */ ascii_category::kPunct,
		/* ?   63    */ ascii_category::kPunct,
		/* @   64    */ ascii_category::kPunct,
		/* A   65    */ ascii_category::kUpper_Alpha_XDigit,
		/* B   66    */ ascii_category::kUpper_Alpha_XDigit,
		/* C   67    */ ascii_category::kUpper_Alpha_XDigit,
		/* D   68    */ ascii_category::kUpper_Alpha_XDigit,
		/* E   69    */ ascii_category::kUpper_Alpha_XDigit,
		/* F   70    */ ascii_category::kUpper_Alpha_XDigit,
		/* G   71    */ ascii_category::kUpper_Alpha,
		/* H   72    */ ascii_category::kUpper_Alpha,
		/* I   73    */ ascii_category::kUpper_Alpha,
		/* J   74    */ ascii_category::kUpper_Alpha,
		/* K   75    */ ascii_category::kUpper_Alpha,
		/* L   76    */ ascii_category::kUpper_Alpha,
		/* M   77    */ ascii_category::kUpper_Alpha,
		/* N   78    */ ascii_category::kUpper_Alpha,
		/* O   79    */ ascii_category::kUpper_Alpha,
		/* P   80    */ ascii_category::kUpper_Alpha,
		/* Q   81    */ ascii_category::kUpper_Alpha,
		/* R   82    */ ascii_category::kUpper_Alpha,
		/* S   83    */ ascii_category::kUpper_Alpha,
		/* T   84    */ ascii_category::kUpper_Alpha,
		/* U   85    */ ascii_category::kUpper_Alpha,
		/* V   86    */ ascii_category::kUpper_Alpha,
		/* W   87    */ ascii_category::kUpper_Alpha,
		/* X   88    */ ascii_category::kUpper_Alpha,
		/* Y   89    */ ascii_category::kUpper_Alpha,
		/* Z   90    */ ascii_category::kUpper_Alpha,
		/* [   91    */ ascii_category::kPunct,
		/* \   92    */ ascii_category::kPunct,
		/* ]   93    */ ascii_category::kPunct,
		/* ^   94    */ ascii_category::kPunct,
		/* _   95    */ ascii_category::kPunct,
		/* `   96    */ ascii_category::kPunct,
		/* a   97    */ ascii_category::kLower_Alpha_XDigit,
		/* b   98    */ ascii_category::kLower_Alpha_XDigit,
		/* c   99    */ ascii_category::kLower_Alpha_XDigit,
		/* d  100    */ ascii_category::kLower_Alpha_XDigit,
		/* e  101    */ ascii_category::kLower_Alpha_XDigit,
		/* f  102    */ ascii_category::kLower_Alpha_XDigit,
		/* g  103    */ ascii_category::kLower_Alpha,
		/* h  104    */ ascii_category::kLower_Alpha,
		/* i  105    */ ascii_category::kLower_Alpha,
		/* j  106    */ ascii_category::kLower_Alpha,
		/* k  107    */ ascii_category::kLower_Alpha,
		/* l  108    */ ascii_category::kLower_Alpha,
		/* m  109    */ ascii_category::kLower_Alpha,
		/* n  110    */ ascii_category::kLower_Alpha,
		/* o  111    */ ascii_category::kLower_Alpha,
		/* p  112    */ ascii_category::kLower_Alpha,
		/* q  113    */ ascii_category::kLower_Alpha,
		/* r  114    */ ascii_category::kLower_Alpha,
		/* s  115    */ ascii_category::kLower_Alpha,
		/* t  116    */ ascii_category::kLower_Alpha,
		/* u  117    */ ascii_category::kLower_Alpha,
		/* v  118    */ ascii_category::kLower_Alpha,
		/* w  119    */ ascii_category::kLower_Alpha,
		/* x  120    */ ascii_category::kLower_Alpha,
		/* y  121    */ ascii_category::kLower_Alpha,
		/* z  122    */ ascii_category::kLower_Alpha,
		/* {  123    */ ascii_category::kPunct,
		/* |  124    */ ascii_category::kPunct,
		/* }  125    */ ascii_category::kPunct,
		/* ~  126    */ ascii_category::kPunct,
		/*DEL 127    */ ascii_category::kCtrl,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	};


	CCDK_FORCEINLINE static bool is_ascii(char32 ch) { return 0 == (ch & ~0x7f); }

	CCDK_FORCEINLINE static bool is_achar(char32 ch) { return is_ascii(ch); }

	CCDK_FORCEINLINE static bool is_alpha(char32 ch) { ccdk_assert(ch < 256);  return ascii_category_table[ch] & ascii_category::kAlpha; }

	CCDK_FORCEINLINE static bool is_digit(char32 ch) { ccdk_assert(ch < 256);  return ascii_category_table[ch] & ascii_category::kDigit; }

	CCDK_FORCEINLINE static bool is_xdigit(char32 ch) { ccdk_assert(ch < 256);  return ascii_category_table[ch] & ascii_category::kXDigit; }

	CCDK_FORCEINLINE static bool is_punct(char32 ch) { ccdk_assert(ch < 256);  return ascii_category_table[ch] & ascii_category::kPunct; }

	CCDK_FORCEINLINE static bool is_ctrl(char32 ch) { ccdk_assert(ch < 256);  return ascii_category_table[ch] & ascii_category::kCtrl; }

	CCDK_FORCEINLINE static bool is_space(char32 ch) { ccdk_assert(ch < 256);  return ascii_category_table[ch] & ascii_category::kSpace; }

	CCDK_FORCEINLINE static bool is_lower(char32 ch) { ccdk_assert(ch < 256);  return ascii_category_table[ch] & ascii_category::kLower; }

	CCDK_FORCEINLINE static bool is_upper(char32 ch) { ccdk_assert(ch < 256);  return ascii_category_table[ch] & ascii_category::kUpper; }

	CCDK_FORCEINLINE static bool is_alnum(char32 ch) { ccdk_assert(ch < 256); is_alpha(ch) || is_digit(ch); }

	CCDK_FORCEINLINE static bool to_lower(char32 ch) { ccdk_assert(ch < 256); is_upper(ch) ? ch + 32 : ch; }

	CCDK_FORCEINLINE static bool to_upper(char32 ch) { ccdk_assert(ch < 256); is_lower(ch) ? ch - 32 : ch; }
};

ccdk_namespace_string_end