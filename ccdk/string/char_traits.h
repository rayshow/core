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
	typedef Char                     char_type;

	constexpr static ptr::size_t length(char_type const* str) noexcept
	{
		if (!str) return 0;
		char_type const* it = str;
		ptr::size_t len = 0;
		for (; *it; ++it, ++len);
		return len;
	}

	/* copy n char from src to dest, need ensure dest has large enough memory */
	CCDK_FORCEINLINE static char_type* copy(char_type *dest, char_type const* src, ptr::size_t n)
	{
		using mpl::util::copy;
		return copy(dest, src, n);
	}

	/* copy src[start, end) to dest[pos, pos+ end -start) */
	CCDK_FORCEINLINE static char_type* copy(char_type* dest, char_type const* src, ptr::size_t start, ptr::size_t end, ptr::size_t pos)
	{
		using mpl::util::copy;
		return copy(dest+pos, src+start, end-start);
	}

	/* move src[start, end) to dest */

	/* literal copy to  string buffer, string buffer need large enough to hold copy */
	template< typename From, 
		typename = mpl::check_t< mpl::is_same<char_type, mpl::remove_all_dim_t<From>>>
	>
	CCDK_FORCEINLINE static char_type* copy(char_type* dest, From const& src )
	{
		using mpl::util::copy;
		return copy( dest, src, mpl::array_len_v<From>);
	}
};

template<typename Char>
struct char_traits;

template<>
struct char_traits<achar> : public common_char_traits<achar>
{
	typedef common_char_traits<achar> base_type;

	using base_type::length;
	using base_type::copy;

};

ccdk_namespace_string_end