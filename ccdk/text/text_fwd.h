#pragma once

#include<ccdk/text/text_module.h>

ccdk_namespace_text_start

class ascii_charset {};       /* ascii  7-bit achar */
class iso8859_1_charset {};   /* latin1 8-bit achar */
class unicode_charset {};     /* utf-8(achar) / utf-16(char16) / utf-32(char32) */

enum class Charset:uint8 {
	eAscii,
	eISO8859_1,  //or latin1
	eUnicode,
};

enum class Encoding :uint8 {
	eAscii,
	eISO8859_1,
	eUTF8,
	eUTF16,
	eUTF32
};

template<typename T>
struct charset;

template<typename T, typename String>
struct to_string_t;

ccdk_namespace_text_end
