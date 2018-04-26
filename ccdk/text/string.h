#pragma once

#include<ccdk/mpl/base/type_.h>
#include<ccdk/mpl/util/addressof.h>
#include<ccdk/mpl/util/move.h>
#include<ccdk/mpl/util/fill.h>
#include<ccdk/mpl/type_traits/is_unsigned.h>
#include<ccdk/mpl/iterator/ptr_iterator.h>
#include<ccdk/mpl/fusion/pair.h>
#include<ccdk/memory/allocator_traits.h>
#include<ccdk/memory/simple_new_allocator.h>
#include<ccdk/container/vector.h>
#include<ccdk/text/text_module.h>
#include<ccdk/text/text_fwd.h>
#include<ccdk/text/char_traits.h>

ccdk_namespace_text_start
using namespace mpl;
using namespace mem;

template<
	typename Char,
	typename Size = uint32,                              /* offen uint32 is enough*/
	typename Alloc = mem::simple_new_allocator<Char>
	
>
class basic_string 
{
public:
	typedef ct::vector<Char, Size, Alloc, units::ratio<2, 1>>   vector_type;
	typedef Char                                           char_type;
	typedef basic_string                                   this_type;
	typedef char_traits<Char>                              traits_type;
	typedef typename traits_type::default_encoding_type    default_encoding_type;
	typedef allocator_traits<Alloc>  allocator_type;

	/* common */
	typedef Char                     value_type;
	typedef Size                     size_type;
	typedef ptr::diff_t              different_type;
	typedef Char*                    pointer_type;
	typedef Char const*              const_pointer_type;
	typedef Char&                    reference_type;
	typedef Char const&              const_reference_type;

	/* iterator */
	typedef Char*                         iterator_type;
	typedef Char const*                   const_iterator_type;
	typedef reverse_iterator<Char*>       reverse_iterator_type;
	typedef reverse_iterator<Char const*> const_reverse_iterator_type;

	template<typename Char2, typename Size2, typename Alloc2>
	friend class basic_string;

private:
	vector_type content;

public:

	/* default and nullptr not alloc memory */
	CCDK_FORCEINLINE constexpr basic_string() noexcept : content{} {}
	CCDK_FORCEINLINE constexpr basic_string(ptr::nullptr_t) noexcept : content{ nullptr }{}

	/* fill */
	CCDK_FORCEINLINE basic_string(size_type n, char_type c = char_type(0))
		: content{ n,c } {}

	/* c-style string copy */
	CCDK_FORCEINLINE basic_string(char_type const* str) 
		: content{ str, traits_type::length(str) } {}

	CCDK_FORCEINLINE basic_string(char_type const* str, size_type len) 
		: content{ str, len } {}
	
	/* copy */
	CCDK_FORCEINLINE  basic_string(basic_string const& other) : content{ other.content } {}

	CCDK_FORCEINLINE  basic_string(basic_string const& other, Size start, Size end)
		: content{ &other.content[start], end - start } {}

	template<typename Alloc2, typename Size2>
	CCDK_FORCEINLINE  basic_string(basic_string<Char, Size2, Alloc2> const& other)
		: content{ other.content } {}

	template<typename Alloc2, typename Size2>
	CCDK_FORCEINLINE  basic_string(basic_string<Char, Size2, Alloc2> const& other, Size start, Size end)
		: content{ &other.content[start], end - start } {}

	/* move */
	CCDK_FORCEINLINE  basic_string(basic_string&& other) noexcept : content{ util::move(other.content) } {}

	template<typename Alloc2, typename Size2>
	CCDK_FORCEINLINE  basic_string(basic_string<Char, Size2, Alloc2>&& other) noexcept
		: content{ util::move(other.content) } {}

	/* swap */
	CCDK_FORCEINLINE constexpr void swap(basic_string& other) noexcept { 
		content.swap(other.content);
	}

	/* copy assign */
	CCDK_FORCEINLINE constexpr basic_string& operator=(char_type const* str) { 
		content.assign(str, traits_type::length(str)); return *this;
	}
	CCDK_FORCEINLINE constexpr basic_string& operator=(basic_string const& other) { 
		content = other.content; return *this;
	}
	template<typename Alloc2, typename Size2>
	CCDK_FORCEINLINE constexpr basic_string& operator=(basic_string<Char, Size2, Alloc2> const& other) {
		content = other.content; return *this;
	}

	/* move assign */
	CCDK_FORCEINLINE constexpr basic_string& operator=(basic_string&& other) {
		content = util::move(other.content); return *this;
	}
	template<typename Alloc2, typename Size2>
	CCDK_FORCEINLINE constexpr basic_string& operator=(basic_string<Char, Alloc2, Size2>&& other) {
		content = util::move(other.content); return *this;
	}

	/* assign */
	CCDK_FORCEINLINE constexpr basic_string& assign(char_type const* str) { basic_string{ str }.swap(*this); return *this; }
	CCDK_FORCEINLINE constexpr basic_string& assign(char_type const* str, size_type len) { basic_string{ str, len }.swap(*this); return *this; }
	template<typename Alloc2, typename Size2>
	CCDK_FORCEINLINE constexpr basic_string& assign(basic_string<Char, Alloc2, Size2> const& other){ basic_string{ other }.swap(*this); return *this; }
	template<typename Alloc2, typename Size2>
	CCDK_FORCEINLINE constexpr basic_string& assign(basic_string<Char, Alloc2, Size2> const& other, size_type start, size_type end) { basic_string{ other,start, end }.swap(*this); return *this; }
	template<typename Alloc2, typename Size2>
	CCDK_FORCEINLINE constexpr basic_string& assign(basic_string<Char, Alloc2, Size2>&& other) { basic_string{ util::move(other) }.swap(*this); return *this; }

	/* size */
	CCDK_FORCEINLINE constexpr size_type size() const noexcept { return length; }
	CCDK_FORCEINLINE constexpr size_type capcity() const noexcept { return alloc_size; }
	CCDK_FORCEINLINE constexpr size_type max_size() const noexcept { return allocator_type::max_allocate_size(); }
	CCDK_FORCEINLINE constexpr bool empty() const noexcept { return length == 0; }
	CCDK_FORCEINLINE constexpr size_type back_index(size_type pos) { return length - pos; }

	/* clear */
	CCDK_FORCEINLINE basic_string& clear() noexcept { ccdk_assert(content); content[0] = char_type(0); length = 0; return *this; }

	/* access */
	CCDK_FORCEINLINE constexpr char_type& operator[](different_type pos) noexcept  { ccdk_assert(content && -pos<(different_type)length && pos<(different_type)length); return content[reverse_index(pos) ]; }
	CCDK_FORCEINLINE constexpr char_type const& operator[](different_type pos) const noexcept { ccdk_assert(content && -pos<(different_type)length && pos<(different_type)length); return content[reverse_index(pos) ]; }

	CCDK_FORCEINLINE constexpr char_type& at(size_type pos) noexcept { ccdk_assert(content && pos<length);  return content[pos];  }
	CCDK_FORCEINLINE constexpr char_type const& at(size_type pos) const noexcept { ccdk_assert(content && pos<length); return content[pos]; }

	CCDK_FORCEINLINE constexpr char_type& front(size_type pos = 0) noexcept { ccdk_assert(content); return content[0]; }
	CCDK_FORCEINLINE constexpr char_type const& front(size_type pos = 0) const noexcept { ccdk_assert(content); return content[0]; }
	 
	CCDK_FORCEINLINE constexpr char_type& back(size_type pos = 0) noexcept { ccdk_assert(content); return content[length-1-pos]; }
	CCDK_FORCEINLINE constexpr char_type const& back(size_type pos = 0) const noexcept { ccdk_assert(content); return content[length-1-pos]; }

	CCDK_FORCEINLINE constexpr char_type* c_str() noexcept { return content; }
	CCDK_FORCEINLINE constexpr char_type const* c_str() const noexcept { return content; }

	/* sequence */
	CCDK_FORCEINLINE basic_string& pop_back() noexcept { if (ccdk_likely(length > 0)) { content[--length] = char_type(0); } return *this; }
	CCDK_FORCEINLINE basic_string& push_back(char_type c) { if (length + 1 > alloc_size) { realloc_copy(length + 1); } content[length++] = c; content[length] = char_type(0); return *this; }

	/* insert  */
	CCDK_FORCEINLINE basic_string& insert(different_type pos, char_type const* str, size_type len) { ccdk_assert(len != 0 && str != nullptr); pos = range_reverse_index(pos);  realloc_replace(pos, pos, str, len); return *this; }
	CCDK_FORCEINLINE basic_string& insert(different_type pos, char_type const* str) { insert(pos, str, traits_type::length(str));return *this; }
	template<typename Alloc2, typename Size2>
	CCDK_FORCEINLINE basic_string& insert(different_type pos, basic_string<Char, Alloc2, Size2> const& str) { insert(pos, str.content, str.size() ); return *this;}
	template<typename Alloc2, typename Size2>
	CCDK_FORCEINLINE basic_string& insert(different_type pos, basic_string<Char, Alloc2, Size2> const& str, size_type start, size_type end) { insert(pos, str.content + start, end - start); return *this; }

	/*erase */
	CCDK_FORCEINLINE basic_string& erase(different_type start, different_type end) { start = range_reverse_index(start); end = range_reverse_index(end);  ccdk_assert(end > start && end<=length); realloc_replace(start, end, nullptr, 0); return *this; }

	/* replace */
	CCDK_FORCEINLINE basic_string& replace(size_type start, size_type end, char_type const* str, size_type len) { start = range_reverse_index(start); end = range_reverse_index(end); ccdk_assert(end > start && str && len != 0); realloc_replace(start, end, str, len); return *this; }
	CCDK_FORCEINLINE basic_string& replace(size_type start, size_type end, char_type const* str) { return replace(start, end, str, traits_type::length(str)); }
	template<typename Alloc2, typename Size2>
	CCDK_FORCEINLINE basic_string& replace(size_type start, size_type end, basic_string<Char, Alloc2, Size2> const& str) { return replace(start, end, str.c_str(), str.size()); }
	template<typename Alloc2, typename Size2>
	CCDK_FORCEINLINE basic_string& replace(size_type start, size_type end, basic_string<Char, Alloc2, Size2> const& str, size_type rstart, size_type rend) { ccdk_assert(rend > rstart && rend < str.size());  return replace(start, end, str.c_str() + rstart, rend - rstart); }

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
	template<different_type Count = 1>
	CCDK_FORCEINLINE constexpr size_type find(char_type ch) const noexcept { return find_impl<Count>(mpl::bool_c<(Count < 0)>, ch); }

	CCDK_FORCEINLINE constexpr size_type find(char_type const* str) { return traits_type::find(content, length, str, traits_type::length(str)); }
	CCDK_FORCEINLINE constexpr size_type find(char_type const* str, size_type len){ return traits_type::find(content, length, str, traits_type::length(str)); }
	template<typename Alloc2, typename Size2>
	CCDK_FORCEINLINE constexpr size_type find(basic_string<Char, Alloc2, Size2> const& str) { return traits_type::find(content, length, str.c_str(), str.length()); }
	template<typename Alloc2, typename Size2>
	CCDK_FORCEINLINE constexpr size_type find(basic_string<Char, Alloc2, Size2> const& str, size_type start, size_type end) { ccdk_assert(end > start); return traits_type::find(content, length, str.c_str() + start, end - start); }

	/* trim */
	template<typename Encoding = default_encoding_type >
	CCDK_FORCEINLINE basic_string& ltrim() noexcept { length = traits_type::ltrim<Encoding>(content, length); return *this; }
	CCDK_FORCEINLINE basic_string& ltrim(encoding_value ev) noexcept { length = traits_type::ltrim(content, length, ev); return *this; }

	template<typename Encoding = default_encoding_type >
	CCDK_FORCEINLINE basic_string& rtrim() noexcept { length = traits_type::rtrim<Encoding>(content, length); return *this; }
	CCDK_FORCEINLINE basic_string& rtrim(encoding_value ev) noexcept { length = traits_type::rtrim(content, length, ev); return *this; }

	template<typename Encoding = default_encoding_type >
	CCDK_FORCEINLINE basic_string& trim() noexcept { length = traits_type::trim<Encoding>(content, length); return *this; }
	CCDK_FORCEINLINE basic_string& trim(encoding_value ev) noexcept { length = traits_type::trim(content, length, ev); return *this; }

	/* append */
	CCDK_FORCEINLINE basic_string& append(char_type const* str, size_type len) { realloc_replace(length, length, str, len); return *this; }
	CCDK_FORCEINLINE basic_string& append(char_type const* str) { return append(str, traits_type::length(str));  }
	template<typename Alloc2, typename Size2>
	CCDK_FORCEINLINE basic_string& append(basic_string<Char, Alloc2, Size2> const& str) { return append(str.content, str.length); }
	template<typename Alloc2, typename Size2>
	CCDK_FORCEINLINE basic_string& append(basic_string<Char, Alloc2, Size2> const& str, size_type start, size_type end) { return append(str.content+start, end-start); }
	template<typename T>
	CCDK_FORCEINLINE basic_string& append(T const& t) { return *this; }

	/* destroy */
	~basic_string() { allocator_type::deallocate(*this, content, length); length = 0;  }
};

using string  = basic_string<achar>;
using wstring = basic_string<wchar>;
using u16string = basic_string<char16>;
using u32string = basic_string<char32>;


ccdk_namespace_text_end