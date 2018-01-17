#pragma once

#include<ccdk/string/string_module.h>
#include<ccdk/string/char_traits.h>

ccdk_namespace_string_start

/* uint32 is enough for almost env */
template<typename Char, typename Size = uint32 >
class base_string 
{
public:

	typedef Char               char_type;
	typedef Size               size_type;
	typedef char_traits<Char>  traits_type;
	typedef base_string        this_type;
	static constexpr float     prealloc_factor = 1.5f;
	 
private:
	char_type*   content;      /* string  */
	size_type    length;       /* content length */
	size_type    alloc_size;   /* for pre-alloc ex-memory */

	/* alloc and copy len elements from str  */
	void alloc_content(const char_type* str, ptr::size_t len)
	{
		assert(len >= 0);
		ptr::size_t pre_alloc_size = len * prealloc_factor;
		char_type buffer = new(ptr::nothrow) char[pre_alloc_size];
		assert(buffer);
		if (!buffer) { ccdk_throw(std::bad_alloc{}); } /* out of memory */
		content = buffer;
		traits_type::strcpy(content, str, len);
		length = len;
		return;
	}

public:

	/* default and nullptr */
	CCDK_FORCEINLINE constexpr base_string() noexcept : content{ nullptr }, length{ 0 }, alloc_size{ 0 } {}
	CCDK_FORCEINLINE constexpr base_string(ptr::nullptr_t) noexcept : content{ nullptr }, length{ 0 }, alloc_size{ 0 } {}

	/* c-style string initalize */
	CCDK_FORCEINLINE constexpr base_string(const char_type* str) :base_string() { if (str) { alloc_content(str, traits_type::strlen(str)); } }
	CCDK_FORCEINLINE constexpr base_string( char_type const* str, ptr::size_t len) { if (str&&len > 0) { alloc_content(str, len); } }
	CCDK_FORCEINLINE constexpr base_string( char_type const* start, char_type const* end) { if (start && end && end > start) { alloc_content(start, end-start); }  }

	/* copy */
	CCDK_FORCEINLINE constexpr base_string(base_string const& str): 
	{

	}
};




ccdk_namespace_string_end