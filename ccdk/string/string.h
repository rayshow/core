#pragma once

#include<ccdk/mpl/util/addressof.h>
#include<ccdk/mpl/util/move.h>
#include<ccdk/string/string_module.h>
#include<ccdk/string/char_traits.h>

ccdk_namespace_string_start

using namespace mpl;


/* uint32 is enough for almost env */
template<typename Char, typename Size = uint32 >
class base_string 
{
public:
	typedef Char               char_type;
	typedef Size               size_type;
	typedef char_traits<Char>  traits_type;
	typedef base_string        this_type;
	typedef Char*              pointer_type;
	typedef Char&              reference_type;
	typedef const Char&        const_reference_type;
	static constexpr float     prealloc_factor = 1.6f;
	static constexpr Size npos = Size(-1);
	static constexpr Size max_pos = Size(-2);

	template<typename Size2>
	friend class base_string<Char, Size2>;

private:
	char_type*   content;      /* string  */
	size_type    length;       /* content length */
	size_type    alloc_size;   /* for pre-alloc ex-memory */

	/* alloc memory */
	char_type* alloc_memory(const size_type actual_size)
	{
		ccdk_assert_if(actual_size == 0) return;
		size_type prealloc_size = actual_size * prealloc_factor;
		if (prealloc_size >= max_size || prealloc_size < actual_size)  /* reach max or round to little size */
		{
			ccdk_throw(std::bad_alloc{});
		}
		if (prealloc_size < 24) prealloc_size = 24; 
		char_type* buffer = new char_type[prealloc_size]; /* may throw std::bad_alloc */
		alloc_size = prealloc_size;                       /* alloc success */
		return buffer;                 
	}

	char_type* realloc_copy(const size_type actual_size)
	{
		char_type* buffer = alloc_memory(actual_size);
		traits_type::copy(buffer, content, length+1);    /* copy 0-terminal */
		ptr::safe_delete_array(content);
		content = buffer;
		length = actual_size;
	}

	/* alloc and copy len elements from str  */
	void alloc_copy(const char_type* str, size_type start, size_type end)
	{
		ccdk_assert_if(!str || start >= end ) return;
		size_type len = end - start;
		char_type* buffer = alloc_memory(len);
		traits_type::copy(buffer, str+start, len);      /* copy src to new allocated */
		buffer[len] = Char(0);
		content = buffer;
		length = len;
	}

public:

	/* default and nullptr not alloc memory */
	CCDK_FORCEINLINE constexpr base_string() noexcept : content{ nullptr }, length{ 0 }, alloc_size{ 0 } {}
	CCDK_FORCEINLINE constexpr base_string(ptr::nullptr_t) noexcept : content{ nullptr }, length{ 0 }, alloc_size{ 0 } {}

	/* c-style string copy */
	CCDK_FORCEINLINE constexpr base_string( const char_type* str) :base_string() { alloc_copy(str, 0, traits_type::strlen(str)); }
	CCDK_FORCEINLINE constexpr base_string( char_type const* str, ptr::size_t len) { alloc_copy(str, 0, len);  }
	CCDK_FORCEINLINE constexpr base_string( char_type const* start, char_type const* end) { alloc_copy(start, 0,  end-start); }
	CCDK_FORCEINLINE constexpr base_string( Size cap, char_type const* str, Size len) : content{ new char_type[cap] }, length{ len }, alloc_size{ cap } {}

	/* copy */
	CCDK_FORCEINLINE constexpr base_string(base_string const& other) { alloc_copy(other.content, other.length); }
	CCDK_FORCEINLINE constexpr base_string(base_string const& other, Size len) { ccdk_assert( len < other.length); alloc_copy(other.content,0, len); }
	CCDK_FORCEINLINE constexpr base_string(base_string const& other, Size start, Size end) { ccdk_assert( end <= other.length); alloc_copy(other.content, start, end); }
	template<typename Size2> 
	CCDK_FORCEINLINE constexpr base_string(base_string<Char, Size2> const& other) { alloc_copy(other.content,0, other.length); }
	template<typename Size2>
	CCDK_FORCEINLINE constexpr base_string(base_string<Char, Size2> const& other, Size len) { ccdk_assert(len < other.length); alloc_copy(other.content,0, len); }
	template<typename Size2>
	CCDK_FORCEINLINE constexpr base_string(base_string<Char, Size2> const& other, Size start, Size end) { ccdk_assert( end <= other.length); alloc_copy(other.content, start, end); }

	/* move */
	CCDK_FORCEINLINE constexpr base_string(base_string&& other) noexcept :content{ other.content }, length{ other.length }, alloc_size{ other.alloc_size } { other.content = nullptr; other.length = 0; other.alloc_size = 0; }
	template<typename Size2>
	CCDK_FORCEINLINE constexpr base_string(base_string<Char, Size2>&& other) noexcept : content{ other.content }, length{ other.length }, alloc_size{ other.alloc_size } { other.content = nullptr; other.length = 0; other.alloc_size = 0; }

	/* swap */
	CCDK_FORCEINLINE constexpr swap(base_string& other) noexcept { using mpl::util; swap(content, other.content); swap(length, other.length); swap(alloc_size, other.alloc_size); }

	/* copy assign, avoid self assign */
	CCDK_FORCEINLINE constexpr base_string& operator=(char_type const* str) { if (str != content) { base_string{ str }.swap(*this); } }
	CCDK_FORCEINLINE constexpr base_string& operator=(base_string const& other) { ccdk_if_not_this(other) { base_string{ other }.swap(*this); } }
	template<typename Size2>
	CCDK_FORCEINLINE constexpr base_string& operator=(base_string<Char, Size2> const& other) { base_string{ other }.swap(*this); }

	/* move assign, avoid self assign*/
	CCDK_FORCEINLINE constexpr base_string& operator=(base_string&& other) { ccdk_if_not_this(other) { base_string{ util::move(other) }.swap(*this); } }
	template<typename Size2>
	CCDK_FORCEINLINE constexpr base_string& operator=(base_string<Char, Size2>&& other) { base_string{ util::move(other) }.swap(*this); }

	/* assign */
	CCDK_FORCEINLINE constexpr base_string& assign(char_type const* str) { base_string{ str }.swap(*this); }
	CCDK_FORCEINLINE constexpr base_string& assign(char_type const* start, char_type const* end) { base_string{ start, end }.swap(*this); }
	CCDK_FORCEINLINE constexpr base_string& assign(char_type const* str, size_type len) { base_string{ str, len }.swap(*this); }
	template<typename Size2>
	CCDK_FORCEINLINE constexpr base_string& assign(base_string<Char,Size2> const& other){ base_string{ other }.swap(*this); }
	template<typename Size2>
	CCDK_FORCEINLINE constexpr base_string& assign(base_string<Char, Size2> const& other, Size len) { base_string{ other,len }.swap(*this); }
	template<typename Size2>
	CCDK_FORCEINLINE constexpr base_string& assign(base_string<Char, Size2> const& other, Size start, Size end) { base_string{ other,start, end }.swap(*this); }
	template<typename Size2>
	CCDK_FORCEINLINE constexpr base_string& assign(base_string<Char, Size2>&& other) { base_string{ util::move(other) }.swap(*this); }

	/* size */
	CCDK_FORCEINLINE constexpr size_type size() const noexcept { return length; }
	CCDK_FORCEINLINE constexpr size_type capcity() const noexcept { return alloc_size; }
	CCDK_FORCEINLINE constexpr size_type max_size() const noexcept { return max_pos; }
	CCDK_FORCEINLINE constexpr bool empty() const noexcept { return length == 0; }

	/* clear */
	CCDK_FORCEINLINE constexpr void clear() const noexcept { ccdk_assert(content); content[0] = char_type(0); }

	/* access */
	CCDK_FORCEINLINE constexpr char_type& operator[](uint32 at) noexcept { ccdk_assert(content); return content[at]; }
	CCDK_FORCEINLINE constexpr char_type  operator[](uint32 at) const noexcept { ccdk_assert(content); return content[at]; }

	CCDK_FORCEINLINE constexpr char_type& at(uint32 at) noexcept { ccdk_assert(content); return content[at]; }
	CCDK_FORCEINLINE constexpr char_type  at(uint32 at) const noexcept  { ccdk_assert(content); return content[at]; }

	CCDK_FORCEINLINE constexpr char_type& front() noexcept { ccdk_assert(content); return content[0]; }
	CCDK_FORCEINLINE constexpr char_type  front() const noexcept { ccdk_assert(content); return content[0]; }
	 
	CCDK_FORCEINLINE constexpr char_type& back() noexcept { ccdk_assert(content); return content[length-1]; }
	CCDK_FORCEINLINE constexpr char_type  back() const noexcept { ccdk_assert(content); return content[length-1]; }

	/* sequence */
	CCDK_FORCEINLINE constexpr void pop_back() noexcept { if (ccdk_likely(length > 0)) { content[length--] = char_type(0); } }
	CCDK_FORCEINLINE constexpr void push_back(char_type c) { if (length + 1 > alloc_size) { realloc_copy(length+1); } content[length++] = c; content[length] = char_type(0); }

	/* change  */
	CCDK_FORCEINLINE constexpr base_string& insert(size_type pos, char_type const* str)
	{
		size_type str_eln = traits_type::length(str);
		/* need realloc */
		if (str_len + length > alloc_size)
		{
			char_type const* buffer = alloc_memory(str_len + length);
			traits_type::copy(buffer, content, 0, pos, 0);                  /* copy head */
			traits_type::copy(buffer, str, 0, str_len, pos);                /* copy middle */
			traits_type::copy(buffer, content, pos, length, pos + str_len); /* copy tail */
			ptr::safe_delete_array(content);
			content = buffer;
			return;
		}
		/* memory enough */

	}


};




ccdk_namespace_string_end