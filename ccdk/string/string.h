#pragma once

#include<ccdk/mpl/util/addressof.h>
#include<ccdk/mpl/util/move.h>
#include<ccdk/mpl/iterator/ptr_iterator.h>
#include<ccdk/string/string_module.h>
#include<ccdk/string/char_traits.h>

ccdk_namespace_string_start
using namespace mpl;

template<typename Char, typename Size = ptr::size_t >
class basic_string 
{
public:
	typedef Char                                           char_type;
	typedef Char                                           value_type;
	typedef Size                                           size_type;
	typedef ptr::diff_t                                    different_type;
	typedef char_traits<Char>                              traits_type;
	typedef basic_string                                   this_type;
	typedef Char*                                          pointer_type;
	typedef Char const*                                    const_pointer_type;
	typedef Char&                                          reference_type;
	typedef Char const&                                    const_reference_type;
	typedef it::iterator<Char*,basic_string>               iterator;
	typedef it::const_iterator<Char*,basic_string>         const_iterator;
	typedef it::reverse_iterator<Char*,basic_string>       reverse_iterator;
	typedef it::const_reverse_iterator<Char*,basic_string> const_reverse_iterator;
	typedef typename traits_type::default_encoding_type    default_encoding_type;

	static constexpr float  prealloc_factor = 1.6f;
	static constexpr Size   npos = Size(-1);
	static constexpr Size   max_pos = Size(-2);

	template<typename Size2>
	friend class basic_string<Char, Size2>;

private:
	pointer_type   content;      /* string  */
	size_type      length;       /* content length */
	size_type      alloc_size;   /* for pre-alloc ex-memory */

	/* alloc memory */
	char_type* alloc_memory(const size_type actual_size)
	{
		ccdk_assert_if(actual_size == 0) return nullptr;
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
		if (actual_size == 0) return nullptr;
		char_type* buffer = alloc_memory(actual_size);
		util::copy(buffer, content, length+1);    /* copy 0-terminal */
		ptr::safe_delete_array(content);
		content = buffer;
		length = actual_size;
	}

	/* alloc and copy len elements from str  */
	void alloc_copy(const char_type* str, size_type start, size_type end)
	{
		if (!str || start >= end) return;
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
		different_type replace_len = end - start;           /* maybe zero */
		size_type actual_len = length + len - replace_len;
		if (actual_len >= alloc_size)
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
	CCDK_FORCEINLINE constexpr basic_string() noexcept : content{ nullptr }, length{ 0 }, alloc_size{ 0 } {}
	CCDK_FORCEINLINE constexpr basic_string(ptr::nullptr_t) noexcept : content{ nullptr }, length{ 0 }, alloc_size{ 0 } {}

	/* c-style string copy */
	CCDK_FORCEINLINE constexpr basic_string(char_type const* str) :basic_string() { alloc_copy(str, 0, traits_type::strlen(str)); }
	CCDK_FORCEINLINE constexpr basic_string(char_type const* str, size_type len) { alloc_copy(str, 0, len);  }
	
	/* copy */
	CCDK_FORCEINLINE constexpr basic_string(basic_string const& other) { alloc_copy(other.content, other.length); }
	CCDK_FORCEINLINE constexpr basic_string(basic_string const& other, Size start, Size end) { ccdk_assert( end <= other.length); alloc_copy(other.content, start, end); }
	template<typename Size2> 
	CCDK_FORCEINLINE constexpr basic_string(basic_string<Char, Size2> const& other) { alloc_copy(other.content,0, other.length); }
	template<typename Size2>
	CCDK_FORCEINLINE constexpr basic_string(basic_string<Char, Size2> const& other, Size start, Size end) { ccdk_assert( end <= other.length); alloc_copy(other.content, start, end); }

	/* move */
	CCDK_FORCEINLINE constexpr basic_string(basic_string&& other) noexcept :content{ other.content }, length{ other.length }, alloc_size{ other.alloc_size } { other.content = nullptr; other.length = 0; other.alloc_size = 0; }
	template<typename Size2>
	CCDK_FORCEINLINE constexpr basic_string(basic_string<Char, Size2>&& other) noexcept : content{ other.content }, length{ other.length }, alloc_size{ other.alloc_size } { other.content = nullptr; other.length = 0; other.alloc_size = 0; }

	/* swap */
	CCDK_FORCEINLINE constexpr swap(basic_string& other) noexcept { using mpl::util; swap(content, other.content); swap(length, other.length); swap(alloc_size, other.alloc_size); }

	/* copy assign, avoid self assign */
	CCDK_FORCEINLINE constexpr basic_string& operator=(char_type const* str) { if (str != content) { basic_string{ str }.swap(*this); } }
	CCDK_FORCEINLINE constexpr basic_string& operator=(basic_string const& other) { ccdk_if_not_this(other) { basic_string{ other }.swap(*this); } }
	template<typename Size2>
	CCDK_FORCEINLINE constexpr basic_string& operator=(basic_string<Char, Size2> const& other) { basic_string{ other }.swap(*this); }

	/* move assign, avoid self assign*/
	CCDK_FORCEINLINE constexpr basic_string& operator=(basic_string&& other) { ccdk_if_not_this(other) { basic_string{ util::move(other) }.swap(*this); } }
	template<typename Size2>
	CCDK_FORCEINLINE constexpr basic_string& operator=(basic_string<Char, Size2>&& other) { basic_string{ util::move(other) }.swap(*this); }

	/* assign */
	CCDK_FORCEINLINE constexpr basic_string& assign(char_type const* str) { basic_string{ str }.swap(*this); }
	CCDK_FORCEINLINE constexpr basic_string& assign(char_type const* str, size_type len) { basic_string{ str, len }.swap(*this); }
	template<typename Size2>
	CCDK_FORCEINLINE constexpr basic_string& assign(basic_string<Char,Size2> const& other){ basic_string{ other }.swap(*this); }
	template<typename Size2>
	CCDK_FORCEINLINE constexpr basic_string& assign(basic_string<Char, Size2> const& other, size_type start, size_type end) { basic_string{ other,start, end }.swap(*this); }
	template<typename Size2>
	CCDK_FORCEINLINE constexpr basic_string& assign(basic_string<Char, Size2>&& other) { basic_string{ util::move(other) }.swap(*this); }

	/* size */
	CCDK_FORCEINLINE constexpr size_type size() const noexcept { return length; }
	CCDK_FORCEINLINE constexpr size_type capcity() const noexcept { return alloc_size; }
	CCDK_FORCEINLINE constexpr size_type max_size() const noexcept { return max_pos; }
	CCDK_FORCEINLINE constexpr bool empty() const noexcept { return length == 0; }

	/* clear */
	CCDK_FORCEINLINE basic_string& clear() const noexcept { ccdk_assert(content); content[0] = char_type(0); return *this; }

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
	CCDK_FORCEINLINE basic_string& pop_back() noexcept { if (ccdk_likely(length > 0)) { content[length--] = char_type(0); } return *this; }
	CCDK_FORCEINLINE basic_string& push_back(char_type c) { if (length + 1 > alloc_size) { realloc_copy(length + 1); } content[length++] = c; content[length] = char_type(0); return *this; }

	/* insert  */
	CCDK_FORCEINLINE basic_string& insert(size_type pos, char_type const* str, size_type len) { ccdk_assert(len != 0 && str!=nullptr); realloc_replace(pos, pos, str, len); return *this; }
	CCDK_FORCEINLINE basic_string& insert(size_type pos, char_type const* str) { insert(pos, str, traits_type::length(str));return *this; }
	template<typename Size2>
	CCDK_FORCEINLINE basic_string& insert(size_type pos, basic_string<Char, Size2> const& str) { insert(pos, str.content, traits_type::length(str)); return *this;}
	template<typename Size2>
	CCDK_FORCEINLINE basic_string& insert(size_type pos, basic_string<Char, Size2> const& str, size_type start, size_type end) { insert(pos, str.content + start, end - start); return *this; }

	/*erase */
	CCDK_FORCEINLINE basic_string& erase(size_type start, size_type end) { ccdk_assert(end > start); realloc_replace(start, end, nullptr, 0); return *this;}

	/* replace */
	CCDK_FORCEINLINE basic_string& replace(size_type start, size_type end, char_type const* str, size_type len) { ccdk_assert(end > start && str != nullptr && len != 0); realloc_replace(start, end, str, len); return *this; }
	CCDK_FORCEINLINE basic_string& replace(size_type start, size_type end, char_type const* str) { ccdk_assert(end > start && str != nullptr); realloc_replace(start, end, str, traits_type::length(str)); return *this; }
	template<typename Size2>
	CCDK_FORCEINLINE basic_string& replace(size_type start, size_type end, basic_string<Char, Size2> const& str) { ccdk_assert(end > start); realloc_replace(start, end, str.content, str.length); return *this; }
	template<typename Size2>
	CCDK_FORCEINLINE basic_string& replace(size_type start, size_type end, basic_string<Char, Size2> const& str, size_type rstart, size_type rend) { ccdk_assert(end > start); realloc_replace(start, end, str.content+rstart, rend); return *this; }

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
	CCDK_FORCEINLINE constexpr size_type find(char_type const* str) { return traits_type::find(content, length, str, traits_type::length(str)); }
	CCDK_FORCEINLINE constexpr size_type find(char_type const* str, size_type len){ return traits_type::find(content, length, str, traits_type::length(str)); }
	template<typename Size2>
	CCDK_FORCEINLINE constexpr size_type find(basic_string<Char, Size2> const& str) { return traits_type::find(content, length, str.c_str(), str.length()); }
	template<typename Size2>
	CCDK_FORCEINLINE constexpr size_type find(basic_string<Char, Size2> const& str, size_type start, size_type end) { ccdk_assert(end > start); return traits_type::find(content, length, str.c_str() + start, end - start); }

	/* trim */
	template<typename Encoding = default_encoding_type >
	CCDK_FORCEINLINE basic_string& ltrim() { traits_type::ltrim<Encoding>(content, length); return *this; }
	CCDK_FORCEINLINE basic_string& ltrim(encoding_value ev) { length = traits_type::ltrim(content, length, ev); return *this; }

	template<typename Encoding = default_encoding_type >
	CCDK_FORCEINLINE basic_string& rtrim() { traits_type::rtrim<Encoding>(content, length); return *this; }
	CCDK_FORCEINLINE basic_string& rtrim(encoding_value ev) { length = traits_type::rtrim(content, length, ev); return *this; }

	template<typename Encoding = default_encoding_type >
	CCDK_FORCEINLINE basic_string& trim() { traits_type::trim<Encoding>(content, length); return *this; }
	CCDK_FORCEINLINE basic_string& trim(encoding_value ev) { length = traits_type::trim(content, length, ev); return *this; }

	/* append */
	CCDK_FORCEINLINE basic_string& append(char_type const* str, size_type len) { realloc_replace(length, length, str, len); return *this; }
	CCDK_FORCEINLINE basic_string& append(char_type const* str) { return append(str, traits_type::length(str));  }
	template<typename Size2> 
	CCDK_FORCEINLINE basic_string& append(basic_string<Char, Size2> const& str) { return append(str.content, str.length); }
	template<typename Size2>
	CCDK_FORCEINLINE basic_string& append(basic_string<Char, Size2> const& str, size_type start, size_type end) { return append(str.content+start, end-start); }
	template<typename T>
	CCDK_FORCEINLINE basic_string& append(T const& t) { return *this; }
};


ccdk_namespace_string_end