#pragma once

#include<ccdk/mpl/util/addressof.h>
#include<ccdk/mpl/iterator/ptr_iterator.h>
#include<ccdk/mpl/fusion/pair.h>
#include<ccdk/container/unordered_map.h>
#include<ccdk/container/range.h>
#include<ccdk/text/text_module.h>
#include<ccdk/text/char_traits.h>

ccdk_namespace_text_start
using namespace mpl;

struct string_store_cache {};
constexpr string_store_cache string_store_cache_c{};

template<typename Char>
struct global_string_slice {
	Char*       content;
	ptr::size_t len;
};

template<typename Char, typename Size = ptr::size_t>
class basic_string_view
{
public:
	typedef Char                                          value_type;
	typedef Char                                          char_type;
	typedef Char*                                         pointer_type;
	typedef Char const*                                   const_pointer_type;
	typedef Char&                                         reference_type;
	typedef Char const&                                   const_reference_type;
	typedef Size                                          size_type;
	typedef basic_string<Char, Size>                      string_type;
	typedef char_traits<Char>                             traits_type;
	typedef basic_string_view                             this_type;
	typedef it::iterator<Char*, this_type>                iterator;
	typedef it::const_iterator<Char*, this_type>          const_iterator;
	typedef it::reverse_iterator<Char*, this_type>        reverse_iterator;
	typedef it::const_reverse_iterator<Char*, this_type>  const_reverse_iterator;
	typedef typename traits_type::default_encoding_type   default_encoding_type;

	template<typename Char, typename Size2>
	friend class basic_string_view;
private:
	fs::range<const_pointer_type>        range;
public:
	/* from c-string */
	CCDK_FORCEINLINE basic_string_view(char_type const* str)
		: range{ str, str + traits_type::length(str) } {
		ccdk_assert(range.valid()); 
	}
	CCDK_FORCEINLINE basic_string_view(char_type const* str, size_type len) 
		: range{ str, str+len } { 
		ccdk_assert(range.valid()); 
	}
	CCDK_FORCEINLINE basic_string_view(char_type const* start, char_type const* end) 
		: range{ start, end } {
		ccdk_assert(range.valid()); 
	}

	/* from basic_string */ 
	template<typename Size2>
	CCDK_FORCEINLINE basic_string_view(basic_string<Char, Size2> const& str) 
		: range{ str.content, str.content+str.length } {
		ccdk_assert(range.valid()); 
	}
	template<typename Size2>
	CCDK_FORCEINLINE basic_string_view(basic_string<Char, Size2> const& str, size_type start, size_type end)
		: range{ str.content+start, str.content+end } {
		ccdk_assert(end>start && end<=str.length && range.valid()); 
	}

	/* copy */
	CCDK_FORCEINLINE basic_string_view(this_type const& other) 
		:range{ other.range } {}

	template<typename Size2>
	CCDK_FORCEINLINE basic_string_view(basic_string_view<Char,Size2> const& other) 
		: range{ other.range } {}

	template<typename Size2>
	CCDK_FORCEINLINE basic_string_view(this_type const& other, size_type start, size_type end) 
		: range{ start, end } { 
		ccdk_assert( end>start && end<= other.range.size() ); 
	}

	CCDK_FORCEINLINE basic_string_view(char_type const* str, ptr::size_t len,  string_store_cache const&)
	{
		static ct::unordered_map<ptr::size_t, global_string_slice<char_type> > pool;
		ptr::size_t hash = util_impl::hash_bytes(reinterpret_cast<uint8*>(str), traits_type::length(str) * sizeof(char_type));
		auto it = pool.find(hash)
		if (it != pool.end()) {
			range._1 = it->content;
			range._2 = it->content + it->len;
		}
		else {
			global_string_slice<char_type> new_slice;
			if (len == 0) {
				new_slice.len = char_traits::length(str);
			}
			else {
				new_slice.len = len;
			}
			new_slice.content = char_traits::copy(str, len);
			if (new_slice.content) {
				pool.emplace(hash, new_slice)
			}
			range._1 = new_slice.content;
			range._2 = new_slice.content + new_slice.len;
		}
	}
	
	/* swap */
	void swap(this_type & other) { range.swap(other.range); }

	/* copy assign */
	CCDK_FORCEINLINE this_type& operator=(this_type const& other) { ccdk_if_not_this(other) { this_type{ other }.swap(*this); } return *this; }
	template<typename Size2>
	CCDK_FORCEINLINE this_type& operator=(basic_string_view<Char, Size2> const& other) { this_type{ other }.swap(*this); return *this; }

	/* assign */


	/* sub_view */
	CCDK_FORCEINLINE this_type sub_view(size_type start, size_type end) const noexcept { ccdk_assert(end>start && end <=range.size());  return this_type{ *this, start, end }; }
 
	/* to_string , copy content in range */
	CCDK_FORCEINLINE string_type to_string() const noexcept { return string_type{ range.start, range.size() }; }
	
	/* size */
	CCDK_FORCEINLINE size_type size() { return range.size(); }
	CCDK_FORCEINLINE bool empty() { return range.size() == 0; }

	/* access */
	CCDK_FORCEINLINE char_type const& operator[](size_type index) const noexcept { ccdk_assert(index < range.size()); return *(range.start + index); }
	CCDK_FORCEINLINE char_type const& front() const noexcept { ccdk_assert(!empty()); return *range.start; }
	CCDK_FORCEINLINE char_type const& back() const noexcept { ccdk_assert(!empty()); return *(range.end-1); }

	CCDK_FORCEINLINE bool starts_with()  const noexcept { return true; }
	CCDK_FORCEINLINE bool ends_with()  const noexcept { return true; }

	/* pop, narrowed view */
	CCDK_FORCEINLINE this_type pop_back(size_type n = 1) const noexcept { ccdk_assert(n <= size()); return this_type{ range.start, range.end - n }; }
	CCDK_FORCEINLINE this_type pop_front(size_type n = 1) const noexcept { ccdk_assert(n <= size()); return this_type{ range.start + n, range.end }; }

	/* erase, generate two narrowed view */
	CCDK_FORCEINLINE auto erase(size_type start, size_type end)  const noexcept { ccdk_assert(end >= start && end <= size());  return fs::make_pair(this_type{ *this, 0, start }, this_type{ *this, end, size() }); }

	/* cut into two narrowed view */
	CCDK_FORCEINLINE auto cut(size_type pos)  const noexcept { ccdk_assert( range.valid() && pos < size()); return erase(pos, pos); }

	/* iterator */
	CCDK_FORCEINLINE auto begin() const noexcept { ccdk_assert(range.valid());  return const_iterator{ range.start }; }
	CCDK_FORCEINLINE auto end() const noexcept { return const_iterator{ range.end }; }
	CCDK_FORCEINLINE auto cbegin() const noexcept { ccdk_assert(range.valid());  return const_iterator{ range.start }; }
	CCDK_FORCEINLINE auto cend() const noexcept { return const_iterator{ range.end }; }
	CCDK_FORCEINLINE auto rbegin() const noexcept { ccdk_assert(range.valid());  return const_iterator{ range.end-1 }; }
	CCDK_FORCEINLINE auto rend() const noexcept { return const_iterator{ range.start-1 }; }
	CCDK_FORCEINLINE auto crbegin() const noexcept { ccdk_assert(range.valid());  return const_iterator{ range.end - 1 }; }
	CCDK_FORCEINLINE auto crend() const noexcept { return const_iterator{ range.start - 1 }; }
};

ccdk_namespace_text_end