#pragma once

#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/container/vector.h>
#include<ccdk/mpl/iterator/algorithm/advance.h>
#include<ccdk/text/text_module.h>

ccdk_namespace_text_start

using namespace mpl;

constexpr uint32 kStringLestElements = 16;

struct string_literial_init {};
constexpr string_literial_init string_literial_init_c{};

template<
	typename Char,                            /* Char type */
	typename IncRatio = units::ratio<2, 1>,   /* 2X incease ratio*/
	uint32  N = kStringLestElements,          /* least allocate 10 elements */
	typename Size = uint32,                   /* size type */
	typename Alloc = mem::simple_new_allocator<T, Size> /* basic allocator */
>
class basic_string : public ct::vector<Char, IncRatio, N, Size, Alloc>
{
public:
	using super_type = ct::vector<Char, IncRatio, N, Size, Alloc>;
	using char_type = Char;
	using this_type = basic_string;
	using traits_type = char_traits<Char>;
	using default_encoding_type = traits_type::default_encoding_type;
	using allocator_type = typename base_type::allocator_type;

	//common
	using value_type = Char;
	using size_type = Size;
	using difference_type = ptr::diff_t;
	using pointer_type = Char * ;
	using const_pointer_type = Char const*;
	using reference_type = Char & ;
	using const_reference_type = Char const&;

	//iterator
	using iterator = typename super_type::iterator;
	using const_iterator = typename super_type::const_iterator;
	using reverse_iterator = typename super_type::reverse_iterator;
	using const_reverse_iterator = typename super_type::const_reverse_iterator;

	template<typename, typename, uint32, typename, typename>
	friend class basic_string;

public:
	// multiplex
	using super_type::swap;
	using super_type::size;
	using super_type::capcity;
	using super_type::empty;
	using super_type::max_size;
	using super_type::front;
	using super_type::back;
	using super_type::operator[];
	using super_type::at;
	using super_type::begin;
	using super_type::end;
	using super_type::cbegin;
	using super_type::cend;
	using super_type::rbegin;
	using super_type::rend;
	using super_type::crbegin;
	using super_type::crend;

	// clear
	CCDK_FORCEINLINE basic_string& clear() noexcept {
		if (size()) {len = 0;at(len) = 0;}
		return *this;
	}

	//content
	CCDK_FORCEINLINE pointer c_str() noexcept { return content; }
	CCDK_FORCEINLINE const_pointer c_str() const noexcept { return content; }


////////////////////////////////////////////////////////////////////////////////////////////////////
//// ctor

		//default and nullptr
	CCDK_FORCEINLINE basic_string() noexcept : super_type{} {}
	CCDK_FORCEINLINE basic_string(ptr::nullptr_t) noexcept : super_type{ nullptr } {}

	// just allocate least n cap
	CCDK_FORCEINLINE basic_string(size_type n) : super_type{ n + 1 } {
		this->at(0) = 0;
	}

	// fill
	CCDK_FORCEINLINE basic_string(size_type n, char_type v) : basic_string{ n } {
		util::construct_fill_n(content, v, n);
		at(n) = 0; //set n-th element '\0'
		len = n;
	}

	// copy-n
	template<
		typename InputIt,
		typename = check_t< is_iterator<InputIt>>>
		CCDK_FORCEINLINE basic_string(InputIt begin, size_type n) : basic_string{ n } {
		util::construct_copy_n(content, begin, n);
		at(n) = 0;
		len = n;
	}

	// copy-n
	template<
		typename InputIt,
		typename = check_t< is_iterator<InputIt>>>
		CCDK_FORCEINLINE basic_string(InputIt begin, InputIt end)
		:basic_string{ begin, util::distance(begin,end) } {}

	//copy ctor
	CCDK_FORCEINLINE basic_string(this_type const& other)
		: basic_string{ other.begin(), other.size() } {}

	//move ctor
	CCDK_FORCEINLINE basic_string(this_type && other)
		: super_type{ util::move(other) } {}

	// literal ctor, copy '\0'
	template<uint32 D>
	CCDK_FORCEINLINE basic_string(
		string_literial_init, char_type const (&arr)[D])
		: super_type{ arr, D } {}

	// c-style string copy 
	CCDK_FORCEINLINE basic_string(char_type const* str)
		: basic_string{ str, traits_type::length(str) } {}

	//template copy ctor
	template<typename IncRatio2, uint32 N2, typename Size2, typename Alloc2>
	CCDK_FORCEINLINE basic_string(
		basic_string<Char, IncRatio2, N2, Size2, Alloc2> const& other)
		: basic_string{ other.begin(), other.size() } {}

	//template move ctor
	template< typename IncRatio2, uint32 N2, typename Size2>
	CCDK_FORCEINLINE basic_string(
		basic_string<Char, IncRatio2, N2, Size2, Alloc> && other)
		: basic_string{ util::move(other) } {}

///////////////////////////////////////////////////////////////////////////////////////////
//// assign 

	// c-style string assign
	CCDK_FORCEINLINE basic_string& operator=(char_type const* str) {
		return assign(str, traits_type::length(str));
	}

	// copy assign
	CCDK_FORCEINLINE basic_string& operator=(basic_string const& other) {
		ccdk_if_not_this(other) {
			return assign(other.begin(), other.size());
		}
		return *this;
	}

	// copy assign
	CCDK_FORCEINLINE basic_string& operator=(basic_string && other) {
		ccdk_if_not_this(other) {
			destroy();
			rvalue_set(other.content, other.len, other.cap);
			other.rvalue_reset();
		}
	}

	// template copy assign
	template<typename IncRatio2, typename Size2, typename Allo2, uint32 N2>
	CCDK_FORCEINLINE basic_string& operator=(
		basic_string<char_type, IncRatio2, N2, Size2, Alloc2> const& other) {
		return assign(other.begin(), other.size());
	}

	// template move assign
	template<typename IncRatio2, typename Size2, uint32 N2>
	CCDK_FORCEINLINE basic_string& operator=(
		basic_string<char_type, IncRatio2, N2, Size2, Alloc> && other) {
		destroy();
		rvalue_set(other.content, other.len, other.cap);
		other.rvalue_reset();
	}

	//assign string literial
	template<uint32 N>
	CCDK_FORCEINLINE constexpr basic_string& assign(
		string_literial_init, char_type const (&arr)[N]) {
		return assign(arr, N - 1);
	}

	// fill assign
	CCDK_FORCEINLINE basic_string& assign(size_type n, char_type c) {
		if (!content || n > capcity()) { reallocate(n); }
		util::construct_fill_n(content, c, n);
		len = n;
		at(len) = 0;
		return *this;
	}

	//range assign 
	template<
		typename InputIt,
		typename = check_t< is_iterator< InputIt >>>
		CCDK_FORCEINLINE basic_string& assign(
			InputIt begin, size_type n) {
		if (!content || n > capcity()) { reallocate(n); }
		util::construct_copy_n(content, begin, n);
		len = n;
		at(len) = 0;
		return *this;
	}

////////////////////////////////////////////////////////////////////////////////////////
//// pop / push /emplace back

	// pop back
	CCDK_FORCEINLINE basic_string& pop_back() noexcept {
		if (ccdk_likely(size() > 0)) {
			at(--len) = 0;
		}
		return *this;
	}

	//pop back-n
	CCDK_FORCEINLINE basic_string& pop_back(size_type n) noexcept {
		ccdk_assert(n < size() && n>0);
		if (n > 0 && n < size()) {
			len -= n;
			at(len) = 0;
		}
		return *this;
	}

	// just keep consistent with other container
	CCDK_FORCEINLINE basic_string& emplace_back(char_type c) { return push_back(c); }

	//push char to back
	CCDK_FORCEINLINE basic_string& push_back(char_type c) {
		if (size() == capacity()) { reallocate_move(size()); }
		content[len++] = c;
		at(len) = 0;
		return *this;
	}

	//push back range-fill
	CCDK_FORCEINLINE basic_string& push_back(size_type n, char_type c) {
		if (!content || size() + n > capacity()) { reallocate_move(size() + n); }
		util::construct_fill_n(content + size(), c, n);
		len += n;
		at(len) = 0;
		return *this;
	}

	//push back range-n
	template<
		typename InputIt,
		typename = check_t< is_iterator< InputIt>> >
		CCDK_FORCEINLINE basic_string& push_back(InputIt begin, size_type n) {
		if (!content || size() + n > capacity()) { reallocate_move(size() + n); }
		util::construct_copy_n(content + size(), begin, n);
		len += n;
		at(len) = 0;
		return *this;
	}

	//push back range
	template<
		typename InputIt,
		typename = check_t< is_iterator< InputIt>> >
		CCDK_FORCEINLINE basic_string& push_back(InputIt begin, InputIt end) {
		return push_back(begin, it::distance(begin, end));
	}

	//push back c-string
	CCDK_FORCEINLINE basic_string& push_back(char_type const& str) {
		return push_back(str, traits_type::length(str));
	}

	//push back string literial
	template<uint32 D>
	CCDK_FORCEINLINE basic_string& push_back(
		string_literial_init, char_type const (&arr)[D]) {
		return push_back(arr, D-1);
	}

//////////////////////////////////////////////////////////////////////////////////////////
//// insert with index

	//insert single char
	CCDK_FORCEINLINE basic_string& insert(size_type pos, char_type c) {
		char arr[2] = { c,0 };
		return insert(pos, arr, 2);
	}

	//insert c-string
	CCDK_FORCEINLINE basic_string& insert(size_type pos, char_type const* str) {
		return insert(pos, str, traits_type::length(str));
	}

	//insert string literial 
	template<uint32 D>
	CCDK_FORCEINLINE basic_string& insert(
		size_type pos, string_literial_init, char_type const (&arr)[D]) {
		return insert(pos, arr, D-1);
	}


	//insert range
	template<
		typename InputIt,
		typename = check_t< is_iterator< InputIt>>>
	CCDK_FORCEINLINE basic_string& insert(size_type pos, InputIt begin, InputIt end) {
		return insert(pos, begin, it::distance(begin, end));
	}

	//insert range-n
	template<
		typename InputIt, 
		typename = check_t< is_iterator< InputIt>>>
	CCDK_FORCEINLINE basic_string& insert(size_type pos, InputIt begin, size_type n) {
		insert_impl(pos, pos + n, begin);
		at(len) = 0;
		return *this;
	}

	//insert fill-n
	template<
		typename InputIt,
		typename = check_t< is_iterator< InputIt>>>
	CCDK_FORCEINLINE basic_string& insert(size_type pos, size_type n, char_type c) {
		emplace_impl(pos, pos + n, c);
		at(len) = 0;
		return *this;
	}

	//insert a string
	template<typename IncRatio2, typename Size2, typename Alloc2, uint32 N2>
	CCDK_FORCEINLINE basic_string& insert( size_type pos,
		basic_string<Char, IncRatio2, N2, Size2, Alloc2> const& other) {
		insert(pos, other.begin(), other.size());
	}


//////////////////////////////////////////////////////////////////////////////////////////
//// insert with iterator

	//insert c-string
	CCDK_FORCEINLINE basic_string& insert(const_iterator pos, char_type const* str) {
		return insert(pos-content, str, traits_type::length(str));
	}

	//insert string literial 
	template<uint32 D>
	CCDK_FORCEINLINE basic_string& insert(
		const_iterator pos, string_literial_init, char_type const (&arr)[D]) {
		return insert(pos-content, arr, D - 1);
	}


	//insert range
	template<
		typename InputIt,
		typename = check_t< is_iterator< InputIt>>>
		CCDK_FORCEINLINE basic_string& insert(const_iterator pos, InputIt begin, InputIt end) {
		return insert(pos-content, begin, it::distance(begin, end));
	}

	//insert range-n
	template<
		typename InputIt,
		typename = check_t< is_iterator< InputIt>>>
		CCDK_FORCEINLINE basic_string& insert(const_iterator pos, InputIt begin, size_type n) {
		insert_impl(pos-content, pos + n, begin);
		at(len) = 0;
		return *this;
	}

	//insert fill-n
	template<
		typename InputIt,
		typename = check_t< is_iterator< InputIt>>>
		CCDK_FORCEINLINE basic_string& insert(const_iterator pos, size_type n, char_type c) {
		emplace_impl(pos-content, pos + n, c);
		at(len) = 0;
		return *this;
	}

	//insert a string
	template<typename IncRatio2, typename Size2, typename Alloc2, uint32 N2>
	CCDK_FORCEINLINE basic_string& insert( const_iterator pos,
		basic_string<Char, IncRatio2, N2, Size2, Alloc2> const& other) {
		insert(pos-content,other.begin(), other.size());
	}

/////////////////////////////////////////////////////////////////////////////////////////////
//// erase
	CCDK_FORCEINLINE basic_string& erase(size_type begin, size_type end) noexcept {
		ccdk_assert(end > begin && end <= size());
		size_type tail_len = size() - end;
		util::move_n(content + begin, content + end, tail_len);
		len -= end - start;
		at(len) = 0;
	}

	CCDK_FORCEINLINE basic_string& erase(const_iterator pos) noexcept {
		return erase(pos, pos + 1);
	}

	CCDK_FORCEINLINE basic_string& erase(size_type begin, size_type end) noexcept {
		return erase(begin - content, end - content);
	}

/////////////////////////////////////////////////////////////////////////////////////////////
//// replace

	CCDK_FORCEINLINE basic_string& replace(size_type start, size_type end, char_type const* str) {
		return replace(start, end, str, traits_type::length(str));
	}

	CCDK_FORCEINLINE basic_string& replace(size_type start, size_type end, size_type n, char_type c) {
		size_type reduce = end - start;
		if (reduce >= n) {
			util::construct_fill_n(content + start, c, n);
			if(reduce>n) erase(n, end);
		}
		else {
			util::construct_fill_n(content + start, c, reduce);
			insert(end, it::advance(begin, reduce), n - reduce);
		}
	}

	template<typename InputIt, typename = check_t< is_iterator<InputIt>>>
	CCDK_FORCEINLINE basic_string& replace(size_type start, size_type end, InputIt begin, size_type n) {
		size_type reduce = end - start;
		if (reduce >= n) {
			util::construct_copy_n(content + start, begin, n);
			if (reduce>n) erase(n, end);
		}
		else {
			util::construct_copy_n(content + start, begin, reduce);
			insert(end, it::advance(begin,reduce), n - reduce);
		}
	}

	template<typename InputIt, typename = check_t< is_iterator<InputIt>>>
	CCDK_FORCEINLINE basic_string& replace(size_type start, size_type end, InputIt begin, InputIt end) {
		return replace(start, end, begin, it::distance(begin, end));
	}

	template<typename IncRatio2, typename Size2, typename Alloc2, uint32 N2>
	CCDK_FORCEINLINE basic_string& replace(size_type start, size_type end,
		basic_string<Char, IncRatio2, N2, Size2, Alloc2> const& other) {
		return replace(start, end, other.begin(), other.size());
	}

////////////////////////////////////////////////////////////////////////////////////////////
//// find

	template<uint32 Count = 1>
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