#pragma once

#include<ccdk/string/string_module.h>
#include<ccdk/mpl/util/copy.h>
#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/type_traits/array_length.h>
#include<ccdk/mpl/type_traits/remove_all_dim.h>
#include<ccdk/mpl/type_traits/is_same.h>
#include<vector>

ccdk_namespace_string_start

template<typename Char>
struct common_char_traits
{
	typedef Char char_type;

	constexpr static ptr::size_t length(char_type const* str) noexcept
	{
		if (!str) return 0; 
		char_type const* it = str;
		ptr::size_t len = 0;
		for (; *it; ++it, ++len);
		return len;
	}

	/* kmp */
	constexpr static ptr::size_t find(
		char_type const* source, ptr::size_t source_len, 
		char_type const* search, ptr::size_t search_len) 
	{
		ccdk_assert(search != nullptr && search_len > 0);
		ptr::size_t *next = new ptr::size_t[search_len];
		
		/* constructor next */
		ptr::size_t fore = 0;
		next[0] = 0;
		for (ptr::size_t i = 1; i < search_len; ++i)
		{
			if (search[fore] == search[i]){ next[i] = ++fore; }
			else { fore = 0; next[i] = 0; }
		}

		/* search */
		ptr::size_t offset = 0;
		ptr::size_t i = 0;
		while (i < source_len && offset<search_len)
		{
			if ( search[offset] == source[i] ) { ++offset; ++i; /* same char, simultaneous move to next char */ }
			else if( offset > 0 )  /* fore have same part */
			{
				if (next[offset] == 0 && next[offset - 1] > 0) { offset = next[offset - 1]; /* move to next*/ }
				else { offset = 0; ++i; /* next[offset]!=0 || next[offset-1] == 0 */  }
			}
			else  { ++i; /* not equal && offset == 0, source move to next char */ }
		}
		if (offset == search_len) return i-search_len;
		return source_len;
	}
};

template<typename Char>
struct char_traits;

template<>
struct char_traits<achar> : public common_char_traits<achar>
{
	typedef common_char_traits<achar> base_type;
	using base_type::length;
	using base_type::find;

	void ltrim(achar const* content)
	{

	}

	void rtrim(achar const* content)
	{

	}

	void trim(achar const* content)
	{

	}


};

template<>
struct char_traits<wchar> : public common_char_traits<wchar>
{
	typedef common_char_traits<wchar> base_type;
	using base_type::length;
	using base_type::find;
};

template<>
struct char_traits<char16> : public common_char_traits<char16>
{
	typedef common_char_traits<char16> base_type;
	using base_type::length;
	using base_type::find;
};

template<>
struct char_traits<char32> : public common_char_traits<char32>
{
	typedef common_char_traits<char32> base_type;
	using base_type::length;
	using base_type::find;
};

ccdk_namespace_string_end