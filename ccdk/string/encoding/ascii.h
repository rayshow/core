#pragma once

#include<ccdk/mpl/util/addressof.h>
#include<ccdk/mpl/util/move.h>
#include<ccdk/mpl/iterator/ptr_iterator.h>
#include<ccdk/string/string_module.h>
#include<ccdk/string/char_traits.h>

ccdk_namespace_string_start

struct ascii_category 
{
	static constexpr uint8 kDigit  = 0x0001;
	static constexpr uint8 kXDigit = 0x0002;
	static constexpr uint8 kAlpha  = 0x0004;
	static constexpr uint8 kCtrl   = 0x0008;
	static constexpr uint8 kLower  = 0x0010;
	static constexpr uint8 kUpper  = 0x0020;
	static constexpr uint8 kSpace  = 0x0040;
	static constexpr uint8 kPunct  = 0x0080;
	static constexpr uint8 kCtrl_Space   = kCtrl  | kSpace;
	static constexpr uint8 kDigit_XDigit = kDigit | kXDigit;
	static constexpr uint8 kUpper_Alpha  = kAlpha | kUpper;
	static constexpr uint8 kLower_Alpha  = kAlpha | kLower;
	static constexpr uint8 kUpper_Alpha_XDigit = kUpper_Alpha | kXDigit;
	static constexpr uint8 kLower_Alpha_XDigit = kLower_Alpha | kXDigit;
};

constexpr uint16 ascii_category_table[] = 
{
	/* mean Dec  Hex           bits         */
	/* NUL  0    0  */ ascii_category::kCtrl,
	/* SOH  1    1  */ ascii_category::kCtrl,
	/* STX  2    2  */ ascii_category::kCtrl,
	/* ETX  3    3  */ ascii_category::kCtrl,
	/* EOT  4    3  */ ascii_category::kCtrl,
	/* ENQ  5    3  */ ascii_category::kCtrl,
	/* ACK  6    3  */ ascii_category::kCtrl,
	/* BEL  7    3  */ ascii_category::kCtrl,
	/* BS   8    3  */ ascii_category::kCtrl,
	/* HT   9    3  */ ascii_category::kCtrl_Space,
	/* LF  10    3  */ ascii_category::kCtrl_Space,
	/* VT  11    3  */ ascii_category::kCtrl_Space,
	/* NP  12    3  */ ascii_category::kCtrl_Space,
	/* CR  13    3  */ ascii_category::kCtrl_Space,
	/* SO  14    3  */ ascii_category::kCtrl,
	/* SI  15    3  */ ascii_category::kCtrl,
	/* SO  16    3  */ ascii_category::kCtrl,
	/* SO  17    3  */ ascii_category::kCtrl,
	/* SO  18    3  */ ascii_category::kCtrl,
	/* SO  19    3  */ ascii_category::kCtrl,
	/* SO  20    3  */ ascii_category::kCtrl,
	/* SO  21    3  */ ascii_category::kCtrl,
	/* SO  22    3  */ ascii_category::kCtrl,
	/* SO  23    3  */ ascii_category::kCtrl,
	/* SO  24    3  */ ascii_category::kCtrl,
	/* SO  25    3  */ ascii_category::kCtrl,
	/* SO  26    3  */ ascii_category::kCtrl,
	/* SO  27    3  */ ascii_category::kCtrl,
	/* SO  28    3  */ ascii_category::kCtrl,
	/* SO  29    3  */ ascii_category::kCtrl,
	/* SO  30    3  */ ascii_category::kCtrl,
	/* SO  31    3  */ ascii_category::kCtrl,
	/* SP  32    3  */ ascii_category::kCtrl_Space,
	/* SO  33    3  */ ascii_category::kPunct,
	/* SO  34    3  */ ascii_category::kPunct,
	/* SO  35    3  */ ascii_category::kPunct,
	/* SO  36    3  */ ascii_category::kPunct,
	/* SO  37    3  */ ascii_category::kPunct,
	/* SO  38    3  */ ascii_category::kPunct,
	/* SO  39    3  */ ascii_category::kPunct,
	/* SO  40    3  */ ascii_category::kPunct,
	/* SO  41    3  */ ascii_category::kPunct,
	/* SO  42    3  */ ascii_category::kPunct,
	/* SO  43    3  */ ascii_category::kPunct,
	/* SO  44    3  */ ascii_category::kPunct,
	/* SO  45    3  */ ascii_category::kPunct,
	/* SO  46    3  */ ascii_category::kPunct,
	/* SO  47    3  */ ascii_category::kPunct,
	/* 0   48    3  */ ascii_category::kDigit_XDigit,
	/* 1   49    3  */ ascii_category::kDigit_XDigit,
	/* 2   50    3  */ ascii_category::kDigit_XDigit,
	/* 3   51    3  */ ascii_category::kDigit_XDigit,
	/* 4   52    3  */ ascii_category::kDigit_XDigit,
	/* 5   53    3  */ ascii_category::kDigit_XDigit,
	/* 6   54    3  */ ascii_category::kDigit_XDigit,
	/* 7   55    3  */ ascii_category::kDigit_XDigit,
	/* 8   56    3  */ ascii_category::kDigit_XDigit,
	/* 9   57    3  */ ascii_category::kDigit_XDigit,
	/* SO  58    3  */ ascii_category::kPunct,
	/* SO  59    3  */ ascii_category::kPunct,
	/* SO  60    3  */ ascii_category::kPunct,
	/* SO  61    3  */ ascii_category::kPunct,
	/* SO  62    3  */ ascii_category::kPunct,
	/* SO  63    3  */ ascii_category::kPunct,
	/* SO  64    3  */ ascii_category::kPunct,
	/* A   65    3  */ ascii_category::kUpper_Alpha_XDigit,
	/* B   66    3  */ ascii_category::kUpper_Alpha_XDigit,
	/* C   67    3  */ ascii_category::kUpper_Alpha_XDigit,
	/* D   68    3  */ ascii_category::kUpper_Alpha_XDigit,
	/* E   69    3  */ ascii_category::kUpper_Alpha_XDigit,
	/* F   70    3  */ ascii_category::kUpper_Alpha_XDigit,
	/* G   71    3  */ ascii_category::kUpper_Alpha,
	/* H   72    3  */ ascii_category::kUpper_Alpha,
	/* I   73    3  */ ascii_category::kUpper_Alpha,
	/* J   74    3  */ ascii_category::kUpper_Alpha,
	/* K   75    3  */ ascii_category::kUpper_Alpha,
	/* L   76    3  */ ascii_category::kUpper_Alpha,
	/* M   77    3  */ ascii_category::kUpper_Alpha,
	/* N   78    3  */ ascii_category::kUpper_Alpha,
	/* O   79    3  */ ascii_category::kUpper_Alpha,
	/* P   80    3  */ ascii_category::kUpper_Alpha,
	/* Q   81    3  */ ascii_category::kUpper_Alpha,
	/* R   82    3  */ ascii_category::kUpper_Alpha,
	/* S   83    3  */ ascii_category::kUpper_Alpha,
	/* T   84    3  */ ascii_category::kUpper_Alpha,
	/* U   85    3  */ ascii_category::kUpper_Alpha,
	/* V   86    3  */ ascii_category::kUpper_Alpha,
	/* W   87    3  */ ascii_category::kUpper_Alpha,
	/* X   88    3  */ ascii_category::kUpper_Alpha,
	/* Y   89    3  */ ascii_category::kUpper_Alpha,
	/* Z   90    3  */ ascii_category::kUpper_Alpha,
	/* SO  64    3  */ ascii_category::kPunct,
	/* SO  64    3  */ ascii_category::kPunct,
	/* SO  64    3  */ ascii_category::kPunct,
	/* SO  64    3  */ ascii_category::kPunct,
	/* SO  64    3  */ ascii_category::kPunct,
	/* SO  64    3  */ ascii_category::kPunct,
	/* a   65    3  */ ascii_category::kLower_Alpha_XDigit,
	/* b   65    3  */ ascii_category::kLower_Alpha_XDigit,
	/* c   65    3  */ ascii_category::kLower_Alpha_XDigit,
	/* d   65    3  */ ascii_category::kLower_Alpha_XDigit,
	/* e   65    3  */ ascii_category::kLower_Alpha_XDigit,
	/* f   65    3  */ ascii_category::kLower_Alpha_XDigit,
	/* g   65    3  */ ascii_category::kLower_Alpha,
	/* h   65    3  */ ascii_category::kLower_Alpha,
	/* i   65    3  */ ascii_category::kLower_Alpha,
	/* j   65    3  */ ascii_category::kLower_Alpha,
	/* k   65    3  */ ascii_category::kLower_Alpha,
	/* l   65    3  */ ascii_category::kLower_Alpha,
	/* m   65    3  */ ascii_category::kLower_Alpha,
	/* n   65    3  */ ascii_category::kLower_Alpha,
	/* o   65    3  */ ascii_category::kLower_Alpha,
	/* p   65    3  */ ascii_category::kLower_Alpha,
	/* q   65    3  */ ascii_category::kLower_Alpha,
	/* r   65    3  */ ascii_category::kLower_Alpha,
	/* s   65    3  */ ascii_category::kLower_Alpha,
	/* t   65    3  */ ascii_category::kLower_Alpha,
	/* u   65    3  */ ascii_category::kLower_Alpha,
	/* v   65    3  */ ascii_category::kLower_Alpha,
	/* w   65    3  */ ascii_category::kLower_Alpha,
	/* x   65    3  */ ascii_category::kLower_Alpha,
	/* y   65    3  */ ascii_category::kLower_Alpha,
	/* z   65    3  */ ascii_category::kLower_Alpha,
};


ccdk_namespace_string_end