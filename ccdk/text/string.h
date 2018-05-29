#pragma once

#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/iterator/algorithm/advance.h>
#include<ccdk/mpl/iterator/algorithm/seq_find.h>
#include<ccdk/mpl/type_traits/is_invocable.h>
#include<ccdk/mpl/type_traits/is_integer.h>
#include<ccdk/mpl/type_traits/is_float.h>
#include<ccdk/mpl/iterator/iterator_traits.h>
#include<ccdk/mpl/iterator/algorithm/distance.h>
#include<ccdk/container/vector.h>
#include<ccdk/text/char_traits.h>
#include<ccdk/text/to_string.h>
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
	typename Alloc = mem::simple_new_allocator<Char, Size> /* basic allocator */
>
class basic_string : public ct::vector<Char, IncRatio, N, Size, Alloc>
{
public:
	using super_type = ct::vector<Char, IncRatio, N, Size, Alloc>;
	using char_type = Char;
	using this_type = basic_string;
	using traits_type = char_traits<Char>;
	using default_encoding_type = typename traits_type::default_encoding_type;
	using allocator_type = typename super_type::allocator_type;

	//common
	using value_type = Char;
	using size_type = Size;
	using difference_type = ptr::diff_t;
	using pointer = Char * ;
	using const_pointer = Char const*;
	using reference = Char & ;
	using const_reference = Char const&;

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
	using super_type::capacity;
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
		if (size()) { len = 0; set_terminal(); }
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
		set_terminal();
	}

	// fill
	CCDK_FORCEINLINE basic_string(size_type n, char_type v) : basic_string{ n } {
		util::construct_fill_n(content, v, n);
		len = n;
		set_terminal();
	}

	// copy-n
	template<
		typename InputIt,
		typename = check_t< is_iterator<InputIt>>>
	CCDK_FORCEINLINE basic_string(InputIt begin, size_type n) : basic_string{ n } {
		util::construct_copy_n(content, begin, n);
		len = n;
		set_terminal();
	}

	// copy-n
	template<
		typename InputIt,
		typename = check_t< is_iterator<InputIt>>>
	CCDK_FORCEINLINE basic_string(InputIt begin, InputIt end)
		:basic_string{ begin, it::distance(begin,end) } {}

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
		: basic_string{ arr, D-1 } {}

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
		return *this;
	}

	// template copy assign
	template<typename IncRatio2, typename Size2, typename Alloc2, uint32 N2>
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
	template<uint32 D>
	CCDK_FORCEINLINE constexpr basic_string& assign(
		string_literial_init, char_type const (&arr)[D]) {
		return assign(arr, D - 1);
	}

	// fill assign
	CCDK_FORCEINLINE basic_string& assign(size_type n, char_type c) {
		if (!content || n > capacity()) { reallocate(n); }
		util::construct_fill_n(content, c, n);
		len = n;
		set_terminal();
		return *this;
	}

	//range assign 
	template<
		typename InputIt,
		typename = check_t< is_iterator< InputIt >>>
		CCDK_FORCEINLINE basic_string& assign(
			InputIt begin, size_type n) {
		if (!content || n > capacity()) { reallocate(n); }
		util::construct_copy_n(content, begin, n);
		len = n;
		set_terminal();
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////
	//// pop / push /emplace back

		// pop back
	CCDK_FORCEINLINE basic_string& pop_back() noexcept {
		if (ccdk_likely(size() > 0)) {
			--len;
			set_terminal();
		}
		return *this;
	}

	//pop back-n
	CCDK_FORCEINLINE basic_string& pop_back(size_type n) noexcept {
		ccdk_assert(n < size() && n>0);
		if (n > 0 && n < size()) {
			len -= n;
			set_terminal();
		}
		return *this;
	}

	// just keep consistent with other container
	CCDK_FORCEINLINE basic_string& emplace_back(char_type c) { return push_back(c); }

	//push char to back
	CCDK_FORCEINLINE basic_string& push_back(char_type c) {
		if (!content || size()+1 == capacity()) { reallocate_move(size()); }
		content[len++] = c;
		set_terminal();
		return *this;
	}


	//////////////////////////////////////////////////////////////////////////////////////////
	//// insert with index

	//insert single char
	CCDK_FORCEINLINE basic_string& insert(size_type pos, char_type c) {
		char arr[2] = { c,0 };
		return insert(pos, arr, 1);
	}

	//insert c-string
	CCDK_FORCEINLINE basic_string& insert(size_type pos, char_type const* str) {
		return insert(pos, str, traits_type::length(str));
	}

	//insert string literial 
	template<uint32 D>
	CCDK_FORCEINLINE basic_string& insert(
		size_type pos, string_literial_init, char_type const (&arr)[D]) {
		return insert(pos, arr, D - 1);
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
		set_terminal();
		return *this;
	}

	//insert fill-n
	CCDK_FORCEINLINE basic_string& insert(size_type pos, size_type n, char_type c) {
		emplace_impl(pos, pos + n, c);
		set_terminal();
		return *this;
	}

	//insert a string
	template<typename IncRatio2, typename Size2, typename Alloc2, uint32 N2>
	CCDK_FORCEINLINE basic_string& insert(size_type pos,
		basic_string<Char, IncRatio2, N2, Size2, Alloc2> const& other) {
		return insert(pos, other.begin(), other.size());
	}


	//////////////////////////////////////////////////////////////////////////////////////////
	//// insert with iterator

		//insert c-string
	CCDK_FORCEINLINE basic_string& insert(const_iterator pos, char_type const* str) {
		return insert(pos - content, str, traits_type::length(str));
	}

	//insert string literial 
	template<uint32 D>
	CCDK_FORCEINLINE basic_string& insert(
		const_iterator pos, string_literial_init, char_type const (&arr)[D]) {
		return insert(pos - content, arr, D - 1);
	}


	//insert range
	template<
		typename InputIt,
		typename = check_t< is_iterator< InputIt>>>
	CCDK_FORCEINLINE basic_string& insert(const_iterator pos, InputIt begin, InputIt end) {
		return insert(pos - content, begin, it::distance(begin, end));
	}

	//insert range-n
	template<
		typename InputIt,
		typename = check_t< is_iterator< InputIt>>>
	CCDK_FORCEINLINE basic_string& insert(const_iterator pos, InputIt begin, size_type n) {
		insert_impl(pos - content, pos + n, begin);
		set_terminal();
		return *this;
	}

	//insert fill-n
	template<
		typename InputIt,
		typename = check_t< is_iterator< InputIt>>>
	CCDK_FORCEINLINE basic_string& insert(const_iterator pos, size_type n, char_type c) {
		emplace_impl(pos - content, pos + n, c);
		set_terminal();
		return *this;
	}

	//insert a string
	template<typename IncRatio2, typename Size2, typename Alloc2, uint32 N2>
	CCDK_FORCEINLINE basic_string& insert(const_iterator pos,
		basic_string<Char, IncRatio2, N2, Size2, Alloc2> const& other) {
		insert(pos - content, other.begin(), other.size());
	}

	/////////////////////////////////////////////////////////////////////////////////////////////
	//// erase
	CCDK_FORCEINLINE basic_string& erase(size_type begin, size_type end) noexcept {
		ccdk_assert(end > begin && end <= size());
		size_type tail_len = size() - end;
		util::move_n(content + begin, content + end, tail_len);
		len -= end - begin;
		set_terminal();
		return *this;
	}

	CCDK_FORCEINLINE basic_string& erase(size_type pos) noexcept {
		return erase(pos, pos + 1);
	}

	CCDK_FORCEINLINE basic_string& erase(const_iterator begin, const_iterator end) noexcept {
		return erase(begin - content, end - content);
	}

	CCDK_FORCEINLINE basic_string& erase(const_iterator pos) noexcept {
		return erase(pos - content);
	}

	/////////////////////////////////////////////////////////////////////////////////////////////
	//// replace

	CCDK_FORCEINLINE basic_string& replace(size_type start, size_type end, char_type const* str) {
		return replace(start, end, str, traits_type::length(str));
	}

	CCDK_FORCEINLINE basic_string& replace(size_type begin, size_type end, size_type n, char_type c) {
		size_type reduce = end - begin;
		if (reduce >= n) {
			util::construct_fill_n(content + begin, c, n);
			if (reduce > n) erase(begin+n, end);
		}
		else {
			util::construct_fill_n(content + begin, c, reduce);
			insert(end,n-reduce,c);
		}
		return *this;
	}

	template<typename InputIt, typename = check_t< is_iterator<InputIt>>>
	CCDK_FORCEINLINE basic_string& replace(size_type begin, size_type end, InputIt begin2, size_type n) {
		size_type reduce = end - begin;
		if (reduce >= n) {
			util::construct_copy_n(content + begin, begin2, n);
			if (reduce > n) erase(begin+n, end);
		}
		else {
			util::construct_copy_n(content + begin, begin2, reduce);
			insert(end, it::advance(begin2, reduce), n - reduce);
		}
		return *this;
	}

	template<typename InputIt, typename = check_t< is_iterator<InputIt>>>
	CCDK_FORCEINLINE basic_string& replace(size_type begin, size_type end, InputIt begin2, InputIt end2) {
		return replace(start, end, begin2, it::distance(begin2, end2));
	}

	template<typename IncRatio2, typename Size2, typename Alloc2, uint32 N2>
	CCDK_FORCEINLINE basic_string& replace(size_type start, size_type end,
		basic_string<Char, IncRatio2, N2, Size2, Alloc2> const& other) {
		return replace(start, end, other.begin(), other.size());
	}

	////////////////////////////////////////////////////////////////////////////////////////////
	//// find / find_index / find_in / find_not_in

	//find i-th matching char
	CCDK_FORCEINLINE size_type find_index(char_type c, difference_type ith=1) const noexcept {
		return find_index_impl([=](char_type ch) { return ch == c; }, ith);
	}

	//find position of i-th  matching prediction FN  char
	template<
		typename FN,
		typename = check_t< is_invocable<FN, char_type>> >
	CCDK_FORCEINLINE size_type find_index(
		FN Fn, difference_type ith=1) const noexcept {
		return find_index_impl(Fn, ith);
	}

	//find i-th position of matching c-string  
	CCDK_FORCEINLINE size_type find_index(
		const_pointer str, difference_type ith=1) const noexcept {
		return find_index(str, traits_type::length(str), ith);
	}

	//find i-th position of matching range-n-string
	template<
		typename RandomIt,
		typename = check_t< is_random_iterator<RandomIt>>,
		typename = is_same< char_type, iterator_value_t<RandomIt>> >
	CCDK_FORCEINLINE size_type find_index(
		RandomIt begin, size_type n, difference_type ith = 1) const noexcept {
		return it::seq_find(content, len, begin, n, ith);
	}

	//find i-th position of matching range-string
	template<
		typename RandomIt,
		typename = check_t< is_random_iterator<RandomIt>>,
		typename = is_same< char_type, iterator_value_t<RandomIt>> >
		CCDK_FORCEINLINE size_type find_index(
			RandomIt begin, RandomIt end, difference_type ith = 1) const noexcept {
		return it::seq_find(content, len, begin, it::distance(begin, end), ith);
	}

	//find i-th position of matching basic_string
	template<typename IncRatio2, typename Size2, typename Alloc2, uint32 N2>
	CCDK_FORCEINLINE size_type find_index(
		basic_string<Char, IncRatio2, N2, Size2, Alloc2> const& other, difference_type ith = 1) const noexcept {
		return find_index(other.c_str(), other.size(), ith );
	}

	//find i-th iterator of matching char
	CCDK_FORCEINLINE const_iterator find(
			char_type c, difference_type ith = 1) const noexcept {
		return content + find_index(c, ith);
	}

	//find i-th iterator of matching char
	template<
		typename FN,
		typename = check_t< is_invocable<FN,char_type>> >
	CCDK_FORCEINLINE const_iterator find(
		FN Fn, difference_type ith = 1) const noexcept {
		return content + find_index(Fn, ith);
	}

	//find i-th iterator of matching c-string
	template<int32 Cnt = 1, typename = check< Cnt != 0 > >
	CCDK_FORCEINLINE const_iterator find(
		const_pointer str, difference_type ith = 1) const noexcept {
		return content + find_index(str, ith);
	}

	//find i-th iterator of matching range-n
	template<
		typename RandomIt,
		typename = check_t< is_random_iterator<RandomIt>>,
		typename = is_same< char_type, iterator_value_t<RandomIt>> >
		CCDK_FORCEINLINE const_iterator find(
			RandomIt begin, size_type len, difference_type ith = 1) const noexcept {
		return content + find_index(begin, len, ith);
	}

	//find i-th iterator of matching range
	template<
		typename RandomIt,
		typename = check_t< is_random_iterator<RandomIt>>,
		typename = is_same< char_type, iterator_value_t<RandomIt>> >
		CCDK_FORCEINLINE const_iterator find(
			RandomIt begin, RandomIt end, difference_type ith = 1) const noexcept {
		return content + find_index(begin, end, ith);
	}

	//find i-th iterator of matching basic_string
	template<typename IncRatio2, typename Size2, typename Alloc2, uint32 N2>
	CCDK_FORCEINLINE const_iterator find(
		basic_string<Char, IncRatio2, N2, Size2, Alloc2> const& other, difference_type ith = 1) const noexcept {
		return content+find_index(other,ith);
	}

	////////////////////////////////////////////////////////////////////////////////////////////
	//// trim 

	template<typename Encoding = default_encoding_type >
	CCDK_FORCEINLINE basic_string& ltrim() noexcept {
		len = traits_type::ltrim<Encoding>(content, len);
		return *this;
	}

	CCDK_FORCEINLINE basic_string& ltrim(encoding_value ev) noexcept {
		len = traits_type::ltrim(content, len, ev);
		return *this;
	}

	template<typename Encoding = default_encoding_type >
	CCDK_FORCEINLINE basic_string& rtrim() noexcept {
		len = traits_type::rtrim<Encoding>(content, len);
		return *this;
	}
	CCDK_FORCEINLINE basic_string& rtrim(encoding_value ev) noexcept {
		len = traits_type::rtrim(content, len, ev);
		return *this;
	}

	template<typename Encoding = default_encoding_type >
	CCDK_FORCEINLINE basic_string& trim() noexcept {
		len = traits_type::trim<Encoding>(content, len);
		return *this;
	}
	CCDK_FORCEINLINE basic_string& trim(encoding_value ev) noexcept {
		len = traits_type::trim(content, len, ev);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	//// append

	//push back range-fill
	CCDK_FORCEINLINE this_type& append(size_type n, char_type c) {
		if (!content || size() + n > capacity()) { reallocate_move(size() + n); }
		util::construct_fill_n(content + size(), c, n);
		len += n;
		set_terminal();
		return *this;
	}

	//push back range-n
	template<
		typename InputIt,
		typename = check_t< is_iterator< InputIt>> >
		CCDK_FORCEINLINE this_type& append(InputIt begin, size_type n) {
		if (!content || size() + n > capacity()) { reallocate_move(size() + n); }
		util::construct_copy_n(content + size(), begin, n);
		len += n;
		set_terminal();
		return *this;
	}

	//push back range
	template<
		typename InputIt,
		typename = check_t< is_iterator< InputIt>> >
		CCDK_FORCEINLINE this_type& append(InputIt begin, InputIt end) {
		return append(begin, it::distance(begin, end));
	}

	//push back c-string
	CCDK_FORCEINLINE this_type& append(const_pointer str) {
		return append(str, traits_type::length(str));
	}

	//push back string literial
	template<uint32 D>
	CCDK_FORCEINLINE this_type& append(
		string_literial_init, char_type const (&arr)[D]) {
		return append(arr, D - 1);
	}

	//append a string
	template<
		typename IncRatio2, 
		typename Size2,
		typename Alloc2, uint32 N2>
		CCDK_FORCEINLINE this_type& append(
			basic_string<Char, IncRatio2, N2, Size2, Alloc2> const& other) {
		return append(other.c_str(), other.size());
	}

	//append integer
	template<
		typename Int, 
		typename = check_t< is_integer<Int>> >
	CCDK_FORCEINLINE this_type& append(Int val) {
		return to_string<Int, this_type>(val,*this);
	}

	//append float
	CCDK_FORCEINLINE this_type& append(float val) {
		append(128, '\0');
		sprintf(content+len-128, "%f", val);
		return *this;
	}

	//append double 
	CCDK_FORCEINLINE this_type& append(double val) {
		append(128, '\0');
		snprintf(content + len - 128,128, "%lf", val);
		return *this;
	}

	// append string
	template<
		typename IncRatio2,
		typename Size2,
		typename Alloc2, uint32 N2>
	CCDK_FORCEINLINE this_type operator+(basic_string<Char, IncRatio2, N2, Size2, Alloc2> const& other) {
		string tmp{ *this };
		tmp.append(other);
		return tmp;
	}

	// append c-string
	CCDK_FORCEINLINE this_type operator+(const_pointer str) {
		string tmp{ *this };
		tmp.append(str);
		return tmp;
	}

	//append integer
	template<
		typename Int,
		typename = check_t< is_integer<Int>> >
	CCDK_FORCEINLINE this_type operator+(Int val) {
		string tmp{ *this };
		tmp.append(val);
		return tmp;
	}

	//append integer
	template<
		typename Float,
		typename = check_t< is_float<Float>>, typename=void>
	CCDK_FORCEINLINE this_type operator+(Float val) {
		string tmp{ *this };
		tmp.append(val);
		return tmp;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////
	//// substr

	CCDK_FORCEINLINE void substr(size_type begin, size_type end) {

	}


	/////////////////////////////////////////////////////////////////////////////////////////////////
	//// starts / ends with

private:

	//set end
	CCDK_FORCEINLINE void set_terminal() noexcept { *(content + len) = 0; }

	//find ith macthing FN backward
	template<typename FN>
	CCDK_FORCEINLINE size_type find_index_impl(FN Fn, difference_type ith) const noexcept {
		uint32 count = 0;
		if (ith > 0) {
			for (size_type i = 0; i < len; ++i) {
				if (Fn(content[i])) {
					if (++count == ith) {
						return i;
					}
				}
			}
		}
		else {
			ith = -ith;
			for (size_type i = 1; i <= len; ++i) {
				if (Fn(content[len - i])) {
					if (++count == ith) {
						return i;
					}
				}
			}
		}
		return len;
	}

public:

	CCDK_FORCEINLINE void debug_value(const char* title="") {
		DebugValueItBegin(title);
		if(content) DebugValueIt(content);
		DebugValueItEnd();
	}


	void debug_content(const char* title = "") {
		DebugValueItBegin(title);
		DebugValueIt(content);
		DebugValueItEnd();
	}
};

using string  = basic_string<achar>;
using wstring = basic_string<wchar>;
using u16string = basic_string<char16>;
using u32string = basic_string<char32>;


ccdk_namespace_text_end