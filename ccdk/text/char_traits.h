#pragma once

#include<ccdk/mpl/util/copy.h>
#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/type_traits/array_length.h>
#include<ccdk/mpl/type_traits/remove_all_dim.h>
#include<ccdk/mpl/type_traits/is_same.h>
#include<ccdk/text/text_module.h>
#include<ccdk/text/encoding/ascii.h>
#include<ccdk/text/encoding/iso8859_1.h>
#include<ccdk/text/encoding/unicode.h>
#include<vector>

ccdk_namespace_text_start

template<typename Char>
struct common_char_traits
{
	using char_type = Char ;
	using pointer = Char * ;
	using const_pointer = Char const*;

	constexpr static ptr::size_t length(char_type const* str) noexcept
	{
		if (!str) return 0; 
		char_type const* it = str;
		ptr::size_t len = 0;
		for (; *it; ++it, ++len);
		return len;
	}
};

template<typename Char>
struct char_traits;

template<>
struct char_traits<achar> : public common_char_traits<achar>
{
	typedef common_char_traits<achar> base_type;
	typedef ascii_encoding            default_encoding_type;

	using base_type::length;

	/* encoding know at compile-time */
	template<typename Encoding = default_encoding_type >
	static constexpr ptr::size_t ltrim(achar* content, ptr::size_t len)
	{
		achar const* it = content;
		achar const* end = content + len;
		for (; it != end; ++it)
			if (!encoding<Encoding>::is_space(*it)) 
				break;
		if(it!=end)
			util::move_n(content, it, len - (it-content) + 1); /* move forward with 0-terminal */
		return len - (it-content);
	}

	/* encoding know at run-time */
	static constexpr ptr::size_t ltrim(achar* content, ptr::size_t len, encoding_value v)
	{
		switch (v)
		{
		case encoding_value::kAscii:
			return ltrim<ascii_encoding>(content, len);
		case encoding_value::kIso8859_1:
			return ltrim<iso8859_1_encoding>(content, len);
		case encoding_value::kUnicode:
			return ltrim<unicode_encoding>(content, len);
		default: ccdk_assert(0);
		}
		ccdk_assert(0);
	}

	template<typename Encoding = default_encoding_type >
	static constexpr ptr::size_t rtrim(achar * content, ptr::size_t len)
	{
		achar const* it = content + len-1;
		achar const* end = content - 1;
		ptr::size_t space_count = 0;
		for(;it!=end;--it,++space_count) if (!encoding<Encoding>::is_space(*it)) break;
		content[len - space_count] = char_type(0);
		return len - space_count;
	}

	/* encoding know at run-time */
	static constexpr ptr::size_t rtrim(achar* content, ptr::size_t len, encoding_value v)
	{
		switch (v)
		{
		case encoding_value::kAscii:
			return rtrim<ascii_encoding>(content, len);
		case encoding_value::kIso8859_1:
			return rtrim<iso8859_1_encoding>(content, len);
		case encoding_value::kUnicode:
			return rtrim<unicode_encoding>(content, len);
		default: ccdk_assert(0);
		}
		ccdk_assert(0);
	}

	template<typename Encoding = default_encoding_type >
	static constexpr ptr::size_t trim(achar * content, ptr::size_t len)
	{
		return ltrim<Encoding>(content, rtrim<Encoding>(content, len));
	}

	/* encoding know at run-time */
	static constexpr  ptr::size_t trim(achar* content, ptr::size_t len, encoding_value v)
	{
		ptr::size_t rlen = 0;
		switch (v)
		{
		case encoding_value::kAscii:
			return ltrim<ascii_encoding>(content, rtrim<ascii_encoding>(content, len));
		case encoding_value::kIso8859_1:
			return ltrim<iso8859_1_encoding>(content, rtrim<iso8859_1_encoding>(content, len));
		case encoding_value::kUnicode:
			return ltrim<unicode_encoding>(content, rtrim<unicode_encoding>(content, len));
		default: ccdk_assert(0);
		}
		ccdk_assert(0);
	}
};

template<>
struct char_traits<wchar> : public common_char_traits<wchar>
{
	typedef common_char_traits<wchar> base_type;
	using base_type::length;
};

template<>
struct char_traits<char16> : public common_char_traits<char16>
{
	typedef common_char_traits<char16> base_type;
	using base_type::length;
};

template<>
struct char_traits<char32> : public common_char_traits<char32>
{
	typedef common_char_traits<char32> base_type;
	using base_type::length;
};

ccdk_namespace_text_end