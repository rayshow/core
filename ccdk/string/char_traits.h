#pragma once

#include<ccdk/string/string_module.h>
#include<ccdk/mpl/util/copy.h>
#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/type_traits/array_length.h>
#include<ccdk/mpl/type_traits/remove_all_dim.h>
#include<ccdk/mpl/type_traits/is_same.h>

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

	constexpr static ptr::size_t find(char_type const* source,
		ptr::size_t source_len, char_type const* search, ptr::size_t search_len) noexcept
	{
		ccdk_assert(search != nullptr && search_len > 0);
		ptr::size_t *offset = new ptr::size_t[search_len];
		if (!offset) return source_len;
		for (ptr::size_t i = 0; i < search_len; ++i)
		{

		}
	}
};

template<typename Char>
struct char_traits;

template<>
struct char_traits<achar> : public common_char_traits<achar>
{
	typedef common_char_traits<achar> base_type;
	using base_type::length;
};

ccdk_namespace_string_end