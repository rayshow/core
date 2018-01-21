#pragma once

#include<ccdk/mpl/util/addressof.h>
#include<ccdk/mpl/util/move.h>
#include<ccdk/mpl/iterator/ptr_iterator.h>
#include<ccdk/string/string_module.h>
#include<ccdk/string/char_traits.h>


ccdk_namespace_string_start
using namespace mpl;

/* uint32 is enough for almost env */
template<typename Char, typename Size = uint32 >
class base_string 
{
public:
	typedef Char                                           char_type;
	typedef Char                                           value_type;
	typedef Size                                           size_type;
	typedef ptr::diff_t                                    different_type;
	typedef char_traits<Char>                              traits_type;
	typedef base_string                                    this_type;
	typedef Char*                                          pointer_type;
	typedef Char const*                                    const_pointer_type;
	typedef Char&                                          reference_type;
	typedef Char const&                                    const_reference_type;
	typedef it::iterator<Char*,base_string>                iterator;
	typedef it::const_iterator<Char*,base_string>          const_iterator;
	typedef it::reverse_iterator<Char*,base_string>        reverse_iterator;
	typedef it::const_reverse_iterator<Char*,base_string>  const_reverse_iterator;

	static constexpr float  prealloc_factor = 1.6f;
	static constexpr Size   npos = Size(-1);
	static constexpr Size   max_pos = Size(-2);

	template<typename Size2>
	friend class base_string<Char, Size2>;

private:
	char_type      content;      /* string  */
	size_type      length;       /* content length */
	size_type      alloc_size;   /* for pre-alloc ex-memory */

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

	/* alloc memory and copy */
	char_type* realloc_copy(const size_type actual_size)
	{
		char_type* buffer = alloc_memory(actual_size);
		util::copy(buffer, content, length+1);    /* copy 0-terminal */
		ptr::safe_delete_array(content);
		content = buffer;
		length = actual_size;
	}

	/* alloc and copy len elements from str  */
	void alloc_copy(const char_type* str, size_type start, size_type end)
	{
		ccdk_assert_if(!str || start >= end ) return;
		different_type len = end - start;
		char_type* buffer = alloc_memory(len);
		util::copy(buffer, str+start, len);      /* copy src to new allocated */
		buffer[len] = Char(0);
		content = buffer;
		length = len;
	}

	/* replace content[start, end) with str[0, len) */
	void realloc_replace(size_type start, size_type end, const char_type* str, size_type len)
	{
		different_type replace_len = end - start;
		size_type actual_len = length + len - replace_len;
		if (actual_len > alloc_size)
		{
			/* memory not enough */
			char_type const* buffer = alloc_memory(actual_len);
			util::copy(buffer, content, start);                                 /* copy head   */
			util::copy(buffer + start, str, len);                               /* copy middle */
			util::copy(buffer + start + len , content + end, length - end + 1); /* copy tail with 0-terminal */
			ptr::safe_delete_array(content);
			content = buffer;
			length = actual_len;
			return;
		}
		/* memory enough */
		util::move(content + start + len, content + end, length - end + 1); /* move to tail with 0-terminal */
		util::copy(content + start, str, len);                              /* copy to middle */
		length = actual_len;
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
	CCDK_FORCEINLINE base_string& clear() const noexcept { ccdk_assert(content); content[0] = char_type(0); return *this; }

	/* access */
	CCDK_FORCEINLINE constexpr char_type& operator[](uint32 at) noexcept { ccdk_assert(content); return content[at]; }
	CCDK_FORCEINLINE constexpr char_type const& operator[](uint32 at) const noexcept { ccdk_assert(content); return content[at]; }

	CCDK_FORCEINLINE constexpr char_type& at(uint32 at) noexcept { ccdk_assert(content); return content[at]; }
	CCDK_FORCEINLINE constexpr char_type const& at(uint32 at) const noexcept  { ccdk_assert(content); return content[at]; }

	CCDK_FORCEINLINE constexpr char_type& front() noexcept { ccdk_assert(content); return content[0]; }
	CCDK_FORCEINLINE constexpr char_type const&  front() const noexcept { ccdk_assert(content); return content[0]; }
	 
	CCDK_FORCEINLINE constexpr char_type& back() noexcept { ccdk_assert(content); return content[length-1]; }
	CCDK_FORCEINLINE constexpr char_type const&  back() const noexcept { ccdk_assert(content); return content[length-1]; }
	CCDK_FORCEINLINE constexpr char_type* c_str() noexcept { return content; }
	CCDK_FORCEINLINE constexpr char_type const* c_str() const noexcept { return content; }

	/* sequence */
	CCDK_FORCEINLINE base_string& pop_back() noexcept { if (ccdk_likely(length > 0)) { content[length--] = char_type(0); } return *this; }
	CCDK_FORCEINLINE base_string& push_back(char_type c) { if (length + 1 > alloc_size) { realloc_copy(length + 1); } content[length++] = c; content[length] = char_type(0); return *this; }

	/* insert  */
	CCDK_FORCEINLINE base_string& insert(size_type pos, char_type const* str, size_type len) { ccdk_assert(len != 0 && str!=nullptr); realloc_replace(pos, pos, str, len); return *this; }
	CCDK_FORCEINLINE base_string& insert(size_type pos, char_type const* str) { insert(pos, str, traits_type::length(str));return *this; }
	template<typename Size2>
	CCDK_FORCEINLINE base_string& insert(size_type pos, base_string<Char, Size2> const& str) { insert(pos, str.content, traits_type::length(str)); return *this;}
	template<typename Size2>
	CCDK_FORCEINLINE base_string& insert(size_type pos, base_string<Char, Size2> const& str, size_type len) { insert(pos, str.content, len); return *this;}
	template<typename Size2>
	CCDK_FORCEINLINE base_string& insert(size_type pos, base_string<Char, Size2> const& str, size_type start, size_type end) { insert(pos, str.content + start, end - start); return *this; }

	/*erase */
	CCDK_FORCEINLINE base_string& erase(size_type start, size_type end) { ccdk_assert(end > start); realloc_replace(start, end, nullptr, 0); return *this;}

	/* replace */
	CCDK_FORCEINLINE base_string& replace(size_type start, size_type end, char_type const* str, size_type len) { ccdk_assert(end > start && str != nullptr && len != 0); realloc_replace(start, end, str, len); return *this; }
	CCDK_FORCEINLINE base_string& replace(size_type start, size_type end, char_type const* str) { ccdk_assert(end > start && str != nullptr); realloc_replace(start, end, str, traits_type::length(str)); return *this; }
	template<typename Size2>
	CCDK_FORCEINLINE base_string& replace(size_type start, size_type end, base_string<Char, Size2> const& str) { ccdk_assert(end > start); realloc_replace(start, end, str.content, str.length); return *this; }
	template<typename Size2>
	CCDK_FORCEINLINE base_string& replace(size_type start, size_type end, base_string<Char, Size2> const& str, size_type len) { ccdk_assert(end > start); realloc_replace(start, end, str.content, len); return *this; }
	template<typename Size2>
	CCDK_FORCEINLINE base_string& replace(size_type start, size_type end, base_string<Char, Size2> const& str, size_type rstart, size_type rend) { ccdk_assert(end > start); realloc_replace(start, end, str.content+rstart, rend); return *this; }

	/* iterator */
	CCDK_FORCEINLINE constexpr iterator begin() const noexcept { return iterator{ content }; }
	CCDK_FORCEINLINE constexpr iterator end() const noexcept { return iterator{ content+length }; }
	
	CCDK_FORCEINLINE constexpr const_iterator cbegin() const noexcept { return const_iterator{ content }; }
	CCDK_FORCEINLINE constexpr const_iterator cend() const noexcept { return const_iterator{ content + length }; }
	
	CCDK_FORCEINLINE constexpr reverse_iterator rbegin() const noexcept { return reverse_iterator{ content + length-1 }; }
	CCDK_FORCEINLINE constexpr reverse_iterator rend() const noexcept { return reverse_iterator{ content -1 }; }
	
	CCDK_FORCEINLINE constexpr const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator{ content+length-1 }; }
	CCDK_FORCEINLINE constexpr const_reverse_iterator crend() const noexcept { return const_reverse_iterator{ content - 1 }; }

	/* find */
	CCDK_FORCEINLINE constexpr size_type find(char_type ch) const noexcept { for (size_type i = 0; i < length; ++i) { if (content[i] == ch) return i; } return npos; }
};




ccdk_namespace_string_end