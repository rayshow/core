#pragma once

#include<ccdk/mpl/util/addressof.h>
#include<ccdk/mpl/util/move.h>
#include<ccdk/mpl/iterator/ptr_iterator.h>
#include<ccdk/string/string_module.h>
#include<ccdk/string/string_fwd.h>
#include<ccdk/string/char_traits.h>

ccdk_namespace_string_start

template<>
struct encoding<iso8859_1_encoding>
{
	struct iso_8859_1_category
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
		static constexpr uint8 kDigit_Punct = kDigit | kPunct;
	};

	static constexpr achar iso_8859_1_category_table[] =
	{
		/* mean Dec            bits         */
		/* NUL  0    */ iso_8859_1_category::kCtrl,
		/* SOH  1    */ iso_8859_1_category::kCtrl,
		/* STX  2    */ iso_8859_1_category::kCtrl,
		/* ETX  3    */ iso_8859_1_category::kCtrl,
		/* EOT  4    */ iso_8859_1_category::kCtrl,
		/* ENQ  5    */ iso_8859_1_category::kCtrl,
		/* ACK  6    */ iso_8859_1_category::kCtrl,
		/* BEL  7    */ iso_8859_1_category::kCtrl,
		/* BS   8    */ iso_8859_1_category::kCtrl,
		/* HT   9    */ iso_8859_1_category::kCtrl_Space,
		/* LF  10    */ iso_8859_1_category::kCtrl_Space,
		/* VT  11    */ iso_8859_1_category::kCtrl_Space,
		/* NP  12    */ iso_8859_1_category::kCtrl_Space,
		/* CR  13    */ iso_8859_1_category::kCtrl_Space,
		/* SO  14    */ iso_8859_1_category::kCtrl,
		/* SI  15    */ iso_8859_1_category::kCtrl,
		/*DLE  16    */ iso_8859_1_category::kCtrl,
		/*DC1  17    */ iso_8859_1_category::kCtrl,
		/*DC2  18    */ iso_8859_1_category::kCtrl,
		/*DC3  19    */ iso_8859_1_category::kCtrl,
		/*DC4  20    */ iso_8859_1_category::kCtrl,
		/*NAK  21    */ iso_8859_1_category::kCtrl,
		/*SYN  22    */ iso_8859_1_category::kCtrl,
		/*ETB  23    */ iso_8859_1_category::kCtrl,
		/*CAN  24    */ iso_8859_1_category::kCtrl,
		/* EM  25    */ iso_8859_1_category::kCtrl,
		/*SUB  26    */ iso_8859_1_category::kCtrl,
		/*ESC  27    */ iso_8859_1_category::kCtrl,
		/* FS  28    */ iso_8859_1_category::kCtrl,
		/* GS  29    */ iso_8859_1_category::kCtrl,
		/* RS  30    */ iso_8859_1_category::kCtrl,
		/* US  31    */ iso_8859_1_category::kCtrl,
		/* SP  32    */ iso_8859_1_category::kCtrl_Space,
		/* !   33    */ iso_8859_1_category::kPunct,
		/* "   34    */ iso_8859_1_category::kPunct,
		/* #   35    */ iso_8859_1_category::kPunct,
		/* $   36    */ iso_8859_1_category::kPunct,
		/* %   37    */ iso_8859_1_category::kPunct,
		/* &   38    */ iso_8859_1_category::kPunct,
		/* '   39    */ iso_8859_1_category::kPunct,
		/* (   40    */ iso_8859_1_category::kPunct,
		/* )   41    */ iso_8859_1_category::kPunct,
		/* *   42    */ iso_8859_1_category::kPunct,
		/* +   43    */ iso_8859_1_category::kPunct,
		/* ,   44    */ iso_8859_1_category::kPunct,
		/* -   45    */ iso_8859_1_category::kPunct,
		/* .   46    */ iso_8859_1_category::kPunct,
		/* /   47    */ iso_8859_1_category::kPunct,
		/* 0   48    */ iso_8859_1_category::kDigit_XDigit,
		/* 1   49    */ iso_8859_1_category::kDigit_XDigit,
		/* 2   50    */ iso_8859_1_category::kDigit_XDigit,
		/* 3   51    */ iso_8859_1_category::kDigit_XDigit,
		/* 4   52    */ iso_8859_1_category::kDigit_XDigit,
		/* 5   53    */ iso_8859_1_category::kDigit_XDigit,
		/* 6   54    */ iso_8859_1_category::kDigit_XDigit,
		/* 7   55    */ iso_8859_1_category::kDigit_XDigit,
		/* 8   56    */ iso_8859_1_category::kDigit_XDigit,
		/* 9   57    */ iso_8859_1_category::kDigit_XDigit,
		/* :   58    */ iso_8859_1_category::kPunct,
		/* ;   59    */ iso_8859_1_category::kPunct,
		/* <   60    */ iso_8859_1_category::kPunct,
		/* =   61    */ iso_8859_1_category::kPunct,
		/* >   62    */ iso_8859_1_category::kPunct,
		/* ?   63    */ iso_8859_1_category::kPunct,
		/* @   64    */ iso_8859_1_category::kPunct,
		/* A   65    */ iso_8859_1_category::kUpper_Alpha_XDigit,
		/* B   66    */ iso_8859_1_category::kUpper_Alpha_XDigit,
		/* C   67    */ iso_8859_1_category::kUpper_Alpha_XDigit,
		/* D   68    */ iso_8859_1_category::kUpper_Alpha_XDigit,
		/* E   69    */ iso_8859_1_category::kUpper_Alpha_XDigit,
		/* F   70    */ iso_8859_1_category::kUpper_Alpha_XDigit,
		/* G   71    */ iso_8859_1_category::kUpper_Alpha,
		/* H   72    */ iso_8859_1_category::kUpper_Alpha,
		/* I   73    */ iso_8859_1_category::kUpper_Alpha,
		/* J   74    */ iso_8859_1_category::kUpper_Alpha,
		/* K   75    */ iso_8859_1_category::kUpper_Alpha,
		/* L   76    */ iso_8859_1_category::kUpper_Alpha,
		/* M   77    */ iso_8859_1_category::kUpper_Alpha,
		/* N   78    */ iso_8859_1_category::kUpper_Alpha,
		/* O   79    */ iso_8859_1_category::kUpper_Alpha,
		/* P   80    */ iso_8859_1_category::kUpper_Alpha,
		/* Q   81    */ iso_8859_1_category::kUpper_Alpha,
		/* R   82    */ iso_8859_1_category::kUpper_Alpha,
		/* S   83    */ iso_8859_1_category::kUpper_Alpha,
		/* T   84    */ iso_8859_1_category::kUpper_Alpha,
		/* U   85    */ iso_8859_1_category::kUpper_Alpha,
		/* V   86    */ iso_8859_1_category::kUpper_Alpha,
		/* W   87    */ iso_8859_1_category::kUpper_Alpha,
		/* X   88    */ iso_8859_1_category::kUpper_Alpha,
		/* Y   89    */ iso_8859_1_category::kUpper_Alpha,
		/* Z   90    */ iso_8859_1_category::kUpper_Alpha,
		/* [   91    */ iso_8859_1_category::kPunct,
		/* \   92    */ iso_8859_1_category::kPunct,
		/* ]   93    */ iso_8859_1_category::kPunct,
		/* ^   94    */ iso_8859_1_category::kPunct,
		/* _   95    */ iso_8859_1_category::kPunct,
		/* `   96    */ iso_8859_1_category::kPunct,
		/* a   97    */ iso_8859_1_category::kLower_Alpha_XDigit,
		/* b   98    */ iso_8859_1_category::kLower_Alpha_XDigit,
		/* c   99    */ iso_8859_1_category::kLower_Alpha_XDigit,
		/* d  100    */ iso_8859_1_category::kLower_Alpha_XDigit,
		/* e  101    */ iso_8859_1_category::kLower_Alpha_XDigit,
		/* f  102    */ iso_8859_1_category::kLower_Alpha_XDigit,
		/* g  103    */ iso_8859_1_category::kLower_Alpha,
		/* h  104    */ iso_8859_1_category::kLower_Alpha,
		/* i  105    */ iso_8859_1_category::kLower_Alpha,
		/* j  106    */ iso_8859_1_category::kLower_Alpha,
		/* k  107    */ iso_8859_1_category::kLower_Alpha,
		/* l  108    */ iso_8859_1_category::kLower_Alpha,
		/* m  109    */ iso_8859_1_category::kLower_Alpha,
		/* n  110    */ iso_8859_1_category::kLower_Alpha,
		/* o  111    */ iso_8859_1_category::kLower_Alpha,
		/* p  112    */ iso_8859_1_category::kLower_Alpha,
		/* q  113    */ iso_8859_1_category::kLower_Alpha,
		/* r  114    */ iso_8859_1_category::kLower_Alpha,
		/* s  115    */ iso_8859_1_category::kLower_Alpha,
		/* t  116    */ iso_8859_1_category::kLower_Alpha,
		/* u  117    */ iso_8859_1_category::kLower_Alpha,
		/* v  118    */ iso_8859_1_category::kLower_Alpha,
		/* w  119    */ iso_8859_1_category::kLower_Alpha,
		/* x  120    */ iso_8859_1_category::kLower_Alpha,
		/* y  121    */ iso_8859_1_category::kLower_Alpha,
		/* z  122    */ iso_8859_1_category::kLower_Alpha,
		/* {  123    */ iso_8859_1_category::kPunct,
		/* |  124    */ iso_8859_1_category::kPunct,
		/* }  125    */ iso_8859_1_category::kPunct,
		/* ~  126    */ iso_8859_1_category::kPunct,
		/*DEL 127    */ iso_8859_1_category::kCtrl,
		/*XXX 128    */ iso_8859_1_category::kCtrl,
		/*XXX 129    */ iso_8859_1_category::kCtrl,
		/*BPH 130    */ iso_8859_1_category::kCtrl,
		/*NBH 131    */ iso_8859_1_category::kCtrl,
		/*IND 132    */ iso_8859_1_category::kCtrl,
		/*NEL 133    */ iso_8859_1_category::kCtrl,
		/*SSA 134    */ iso_8859_1_category::kCtrl,
		/*ESA 135    */ iso_8859_1_category::kCtrl,
		/*HTS 136    */ iso_8859_1_category::kCtrl,
		/*HTJ 137    */ iso_8859_1_category::kCtrl,
		/*VTS 138    */ iso_8859_1_category::kCtrl,
		/*PLD 139    */ iso_8859_1_category::kCtrl,
		/*PLB 140    */ iso_8859_1_category::kCtrl,
		/* RL 141    */ iso_8859_1_category::kCtrl,
		/*SS2 142    */ iso_8859_1_category::kCtrl,
		/*SS3 143    */ iso_8859_1_category::kCtrl,
		/*DCS 144    */ iso_8859_1_category::kCtrl,
		/*PU1 145    */ iso_8859_1_category::kCtrl,
		/*PU2 146    */ iso_8859_1_category::kCtrl,
		/*STS 147    */ iso_8859_1_category::kCtrl,
		/*CCH 148    */ iso_8859_1_category::kCtrl,
		/* MW 149    */ iso_8859_1_category::kCtrl,
		/*SGA 150    */ iso_8859_1_category::kCtrl,
		/*EGA 151    */ iso_8859_1_category::kCtrl,
		/*SOS 152    */ iso_8859_1_category::kCtrl,
		/*XXX 153    */ iso_8859_1_category::kCtrl,
		/*SCI 154    */ iso_8859_1_category::kCtrl,
		/*CSI 155    */ iso_8859_1_category::kCtrl,
		/* ST 156    */ iso_8859_1_category::kCtrl,
		/*OSC 157    */ iso_8859_1_category::kCtrl,
		/* PM 158    */ iso_8859_1_category::kCtrl,
		/*APC 159    */ iso_8859_1_category::kCtrl,
		/*NSP 160    */ iso_8859_1_category::kSpace,
		/* ¡  161    */ iso_8859_1_category::kPunct,
		/* ¢  162    */ iso_8859_1_category::kPunct,
		/* £  163    */ iso_8859_1_category::kPunct,
		/* ¤  164    */ iso_8859_1_category::kPunct,
		/* ¥  165    */ iso_8859_1_category::kPunct,
		/* ¦  166    */ iso_8859_1_category::kPunct,
		/* §  167    */ iso_8859_1_category::kPunct,
		/* ¨  168    */ iso_8859_1_category::kPunct,
		/* ©  169    */ iso_8859_1_category::kPunct,
		/* ª  170    */ iso_8859_1_category::kPunct,
		/* «  171    */ iso_8859_1_category::kPunct,
		/* ¬  172    */ iso_8859_1_category::kPunct,
		/*SHT 173    */ iso_8859_1_category::kPunct,
		/* ®  174    */ iso_8859_1_category::kPunct,
		/* ¯  175    */ iso_8859_1_category::kPunct,
		/* °  176    */ iso_8859_1_category::kPunct,
		/* ±  177    */ iso_8859_1_category::kPunct,
		/* ²  178    */ iso_8859_1_category::kDigit_Punct,
		/* ³  179    */ iso_8859_1_category::kDigit_Punct,
		/* ´  180    */ iso_8859_1_category::kPunct,
		/* μ  181    */ iso_8859_1_category::kPunct,
		/* ¶  182    */ iso_8859_1_category::kPunct,
		/* ·  183    */ iso_8859_1_category::kPunct,
		/* ¸  184    */ iso_8859_1_category::kPunct,
		/* ¹  185    */ iso_8859_1_category::kDigit_Punct,
		/* º  186    */ iso_8859_1_category::kPunct,
		/* »  187    */ iso_8859_1_category::kPunct,
		/* ¼  188    */ iso_8859_1_category::kPunct,
		/* ½  189    */ iso_8859_1_category::kPunct,
		/* ¾  190    */ iso_8859_1_category::kPunct,
		/* ¿  191    */ iso_8859_1_category::kPunct,
		/* À  192    */ iso_8859_1_category::kUpper_Alpha,
		/* Á  193    */ iso_8859_1_category::kUpper_Alpha,
		/* Â  194    */ iso_8859_1_category::kUpper_Alpha,
		/* Ã  195    */ iso_8859_1_category::kUpper_Alpha,
		/* Ä  196    */ iso_8859_1_category::kUpper_Alpha,
		/* Å  197    */ iso_8859_1_category::kUpper_Alpha,
		/* Æ  198    */ iso_8859_1_category::kUpper_Alpha,
		/* Ç  199    */ iso_8859_1_category::kUpper_Alpha,
		/* È  200    */ iso_8859_1_category::kUpper_Alpha,
		/* É  201    */ iso_8859_1_category::kUpper_Alpha,
		/* Ê  202    */ iso_8859_1_category::kUpper_Alpha,
		/* Ë  203    */ iso_8859_1_category::kUpper_Alpha,
		/* Ì  204    */ iso_8859_1_category::kUpper_Alpha,
		/* Í  205    */ iso_8859_1_category::kUpper_Alpha,
		/* Î  206    */ iso_8859_1_category::kUpper_Alpha,
		/* Ï  207    */ iso_8859_1_category::kUpper_Alpha,
		/* Ð  208    */ iso_8859_1_category::kUpper_Alpha,
		/* Ñ  209    */ iso_8859_1_category::kUpper_Alpha,
		/* Ò  210    */ iso_8859_1_category::kUpper_Alpha,
		/* Ó  211    */ iso_8859_1_category::kUpper_Alpha,
		/* Ô  212    */ iso_8859_1_category::kUpper_Alpha,
		/* Õ  213    */ iso_8859_1_category::kUpper_Alpha,
		/* Ö  214    */ iso_8859_1_category::kUpper_Alpha,
		/* ×  215    */ iso_8859_1_category::kPunct,
		/* Ø  216    */ iso_8859_1_category::kUpper_Alpha,
		/* Ù  217    */ iso_8859_1_category::kUpper_Alpha,
		/* Ú  218    */ iso_8859_1_category::kUpper_Alpha,
		/* Û  219    */ iso_8859_1_category::kUpper_Alpha,
		/* Ü  220    */ iso_8859_1_category::kUpper_Alpha,
		/* Ý  221    */ iso_8859_1_category::kUpper_Alpha,
		/* Þ  222    */ iso_8859_1_category::kUpper_Alpha,
		/* ß  223    */ iso_8859_1_category::kLower_Alpha,
		/* à  224    */ iso_8859_1_category::kLower_Alpha,
		/* á  225    */ iso_8859_1_category::kLower_Alpha,
		/* â  226    */ iso_8859_1_category::kLower_Alpha,
		/* ã  227    */ iso_8859_1_category::kLower_Alpha,
		/* ä  228    */ iso_8859_1_category::kLower_Alpha,
		/* å  229    */ iso_8859_1_category::kLower_Alpha,
		/* æ  230    */ iso_8859_1_category::kLower_Alpha,
		/* ç  231    */ iso_8859_1_category::kLower_Alpha,
		/* è  232    */ iso_8859_1_category::kLower_Alpha,
		/* é  233    */ iso_8859_1_category::kLower_Alpha,
		/* ê  234    */ iso_8859_1_category::kLower_Alpha,
		/* ë  235    */ iso_8859_1_category::kLower_Alpha,
		/* ì  236    */ iso_8859_1_category::kLower_Alpha,
		/* í  237    */ iso_8859_1_category::kLower_Alpha,
		/* î  238    */ iso_8859_1_category::kLower_Alpha,
		/* ï  239    */ iso_8859_1_category::kLower_Alpha,
		/* ð  240    */ iso_8859_1_category::kLower_Alpha,
		/* ñ  241    */ iso_8859_1_category::kLower_Alpha,
		/* ò  242    */ iso_8859_1_category::kLower_Alpha,
		/* ó  243    */ iso_8859_1_category::kLower_Alpha,
		/* ô  244    */ iso_8859_1_category::kLower_Alpha,
		/* õ  245    */ iso_8859_1_category::kLower_Alpha,
		/* ö  246    */ iso_8859_1_category::kLower_Alpha,
		/* ÷  247    */ iso_8859_1_category::kPunct,
		/* ø  248    */ iso_8859_1_category::kLower_Alpha,
		/* ù  249    */ iso_8859_1_category::kLower_Alpha,
		/* ú  250    */ iso_8859_1_category::kLower_Alpha,
		/* û  251    */ iso_8859_1_category::kLower_Alpha,
		/* ü  252    */ iso_8859_1_category::kLower_Alpha,
		/* ý  253    */ iso_8859_1_category::kLower_Alpha,
		/* þ  254    */ iso_8859_1_category::kLower_Alpha,
		/* ÿ  255    */ iso_8859_1_category::kLower_Alpha,	
	};


	static constexpr achar iso_8859_1_conv_table[] =
	{
		0   , 0   , 0   , 0   , 0   , 0   , 0   , 0   , 0   , 0   , 0   , 0   , 0   , 0   , 0   , 0   ,  /*16*/
		0   , 0   , 0   , 0   , 0   , 0   , 0   , 0   , 0   , 0   , 0   , 0   , 0   , 0   , 0   , 0   ,  /*32*/
		0   , 0   , 0   , 0   , 0   , 0   , 0   , 0   , 0   , 0   , 0   , 0   , 0   , 0   , 0   , 0   ,  /*48*/
		0   , 0   , 0   , 0   , 0   , 0   , 0   , 0   , 0   , 0   , 0   , 0   , 0   , 0   , 0   , 0   ,  /*64*/
		0   , 'a' , 'b' , 'c' , 'd' , 'e' , 'f' , 'g' , 'h' , 'i' , 'j' , 'k' , 'l' , 'm' , 'n' , 'o' ,  /*80*/
	    'p' , 'q' , 'r' , 's' , 't' , 'u' , 'v' , 'w' , 'x' , 'y' , 'z' , 0   , 0   , 0   , 0   , 0   ,  /*96*/
		0   , 'A' , 'B' , 'C' , 'D' , 'E' , 'F' , 'G' , 'H' , 'I' , 'G' , 'K' , 'L' , 'M' , 'N' , 'O' ,  /*112*/
	    'P' , 'Q' , 'R' , 'S' , 'T' , 'U' , 'V' , 'W' , 'X' , 'Y' , 'Z' , 0   , 0   , 0   , 0   , 0   ,  /*128*/
		0   , 0   , 0   , 0   , 0   , 0   , 0   , 0   , 0   , 0   , 0   , 0   , 0   , 0   , 0   , 0   ,  /*144*/
		0   , 0   , 0   , 0   , 0   , 0   , 0   , 0   , 0   , 0   , 0   , 0   , 0   , 0   , 0   , 0   ,  /*160*/
		0   , 0   , 0   , 0   , 0   , 0   , 0   , 0   , 0   , 0   , 0   , 0   , 0   , 0   , 0   , 0   ,  /*176*/
		0   , 0   , 0   , 0   , 0   , 0   , 0   , 0   , 0   , 0   , 0   , 0   , 0   , 0   , 0   , 0   ,  /*192*/
		224 , 225 , 226 , 227 , 228 , 229 , 230 , 231 , 232 , 233 , 234 , 235 , 236 , 237 , 238 , 239 ,  /*208*/
		240 , 241 , 242 , 243 , 244 , 245 , 246 , 0   , 248 , 249 , 250 , 251 , 252 , 253 , 254 , 0   ,  /*224*/
		192 , 193 , 194 , 195 , 196 , 197 , 198 , 199 , 200 , 201 , 202 , 203 , 204 , 205 , 206 , 207 ,  /*240*/
		208 , 209 , 210 , 211 , 212 , 213 , 214 , 0   , 216 , 217 , 218 , 219 , 220 , 221 , 222 , 255 ,  /*256*/
	};

	CCDK_FORCEINLINE bool is_iso_8859_1(char32 ch) { return 0 == (ch & ~0xff); }

	CCDK_FORCEINLINE bool is_latin1(char32 ch) { return is_iso_8859_1(ch); }

	CCDK_FORCEINLINE bool is_alpha(char32 ch) { ccdk_assert(ch < 256);  return iso_8859_1_category_table[ch] & iso_8859_1_category::kAlpha; }

	CCDK_FORCEINLINE bool is_digit(char32 ch) { ccdk_assert(ch < 256);  return iso_8859_1_category_table[ch] & iso_8859_1_category::kDigit; }

	CCDK_FORCEINLINE bool is_xdigit(char32 ch) { ccdk_assert(ch < 256);  return iso_8859_1_category_table[ch] & iso_8859_1_category::kXDigit; }

	CCDK_FORCEINLINE bool is_punct(char32 ch) { ccdk_assert(ch < 256);  return iso_8859_1_category_table[ch] & iso_8859_1_category::kPunct; }

	CCDK_FORCEINLINE bool is_ctrl(char32 ch) { ccdk_assert(ch < 256);  return iso_8859_1_category_table[ch] & iso_8859_1_category::kCtrl; }

	CCDK_FORCEINLINE bool is_space(char32 ch) { ccdk_assert(ch < 256);  return iso_8859_1_category_table[ch] & iso_8859_1_category::kSpace; }

	CCDK_FORCEINLINE bool is_lower(char32 ch) { ccdk_assert(ch < 256);  return iso_8859_1_category_table[ch] & iso_8859_1_category::kLower; }

	CCDK_FORCEINLINE bool is_upper(char32 ch) { ccdk_assert(ch < 256);  return iso_8859_1_category_table[ch] & iso_8859_1_category::kUpper; }

	CCDK_FORCEINLINE bool is_alnum(char32 ch) { ccdk_assert(ch < 256);  is_alpha(ch) || is_digit(ch); }

	CCDK_FORCEINLINE achar to_lower(char32 ch) { ccdk_assert(ch < 256); return is_upper(ch) ? iso_8859_1_conv_table[ch] : ch; }

	CCDK_FORCEINLINE achar to_upper(char32 ch) { ccdk_assert(ch < 256); return is_lower(ch) ? iso_8859_1_conv_table[ch] : ch; }
};

ccdk_namespace_string_end