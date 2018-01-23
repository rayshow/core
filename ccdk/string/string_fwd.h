#pragma once

#include<ccdk/string/string_module.h>

ccdk_namespace_string_start

class ascii_encoding {};      /* ascii  7-bit achar */
class iso8859_1_encoding {};  /* latin1 8-bit achar */
class unicode_encoding {};    /* utf-8(achar) / utf-16(char16) / utf-32(char32) */

enum class encoding_value:uint8
{
	kAscii,
	kIso8859_1,
	kUnicode,
};

template<typename T>
struct encoding;

template<typename T, typename String>
struct to_string_t;



ccdk_namespace_string_end
