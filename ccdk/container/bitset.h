#pragma once

#include<ccdk/container/container_mudule.h>
#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/base/logic_.h>
#include<ccdk/mpl/type_traits/is_same.h>
#include<ccdk/mpl/iterator/iterator_traits.h>
#include<ccdk/mpl/iterator/bit_iterator.h>
#include<ccdk/mpl/iterator/reverse_iterator.h>
#include<ccdk/mpl/util/copy.h>
#include<ccdk/mpl/util/swap.h>
#include<ccdk/mpl/units/ratio.h>

#include<ccdk/text/char_traits.h>
#include<ccdk/algorithm/distance.h>
#include<ccdk/memory/simple_new_allocator.h>
#include<ccdk/memory/allocator_traits.h>

ccdk_namespace_ct_start

using namespace ccdk::mpl;

template<
	typename T = uint8, 
	typename Size = uint32,
	typename InceaseRatio = units::ratio<2,1>,   /* pre-allocate 1 times more  */
	typename Alloc = mem::simple_new_allocator<T>
>
class bitset : protected Alloc
{
public:

	/* common */
	using this_type = bitset;
	using increase_ratio = InceaseRatio;

	/* container */
	using value_type           = bool;
	using pointer_type         = T*;
	using const_pointer_type   = T const*;
	using reference_type       = it::bit_access<T>;
	using const_reference_type = bool;
	using size_type            = Size;
	using difference_type      = ptr::diff_t;
	using allocator_type       = mem::allocator_traits<Alloc>;

	/* iterator */
	using iterator_type               = iterator<bit_random_category, value_type, size_type>;
	using const_iterator_type         = iterator<bit_random_category, const value_type, size_type>;
	using reverse_iterator_type       = reverse_iterator<iterator_type>;
	using const_reverse_iterator_type = reverse_iterator<const_iterator_type>;

	static constexpr uint32 kStoreBits = sizeof(T)*8;

	/* friend */
	template<typename T2, typename Size2, typename Alloc2>
	friend class bitset;

private:
	pointer_type  content;
	size_type     len;
	size_type     cap;

public:

	/* destruct */
	CCDK_FORCEINLINE ~bitset() { destruct(); len = 0; cap = 0; }

	/* default, lazy */
	CCDK_FORCEINLINE constexpr bitset() :content{ nullptr }, len{ 0 }, cap{ 0 } {}
	CCDK_FORCEINLINE constexpr bitset(ptr::nullptr_t) : content{ nullptr }, len{ 0 }, cap{ 0 } {}

	/* fill byte*/
	CCDK_FORCEINLINE explicit bitset(size_type n, bool value = false) {
		allocate_fill(n, value);
	}

	/* initial from constexpr string */
	template<typename Char, ptr::size_t N>
	CCDK_FORCEINLINE explicit bitset( from_string_literial, Char (&str)[N]) {
		allocate_init_from_string(str, N-1);
	}

	/* initial from string pointer, need count string length */
	template<typename Char>
	CCDK_FORCEINLINE explicit bitset(Char const* str) {
		allocate_init_from_string(str, txt::char_traits<Char>::length(str));
	}

	/* range-n copy */
	template<typename InputIt, typename = check< is_iterator<InputIt>> >
	CCDK_FORCEINLINE bitset(InputIt beginIt, size_type n) {
		allocate_copy(n, beginIt);
	}

	/* range copy */
	template<typename InputIt, typename = check< is_iterator<InputIt>> >
	CCDK_FORCEINLINE bitset(InputIt beginIt, InputIt endIt) {
		ptr::size_t n = alg::distance(beginIt, endIt);
		allocate_copy(n, beginIt);
	}

	/* copy */
	CCDK_FORCEINLINE bitset(bitset const& other){
		content = allocator_type::allocate(*this, other.count());
		len = other.size();
		memcpy(content, other.content, other.bytes());
	}

	/* template copy */
	template<typename Size2, typename T2, typename Alloc2>
	CCDK_FORCEINLINE bitset(bitset<T2, Size2, Alloc2> const& other) {
		len = other.size();
		content = allocator_type::allocate(*this, other.bytes() / sizeof(T));
		memcpy(content, other.content, other.byte());
	}

	/* move */
	CCDK_FORCEINLINE bitset(bitset && other)  noexcept
		: content{ other.content }, len{ other.size() } {
		other.rvalue_reset();
	}

	/* template move */
	template<typename Size2>
	CCDK_FORCEINLINE bitset(bitset<T,Size2,Alloc> && other)  noexcept
		: content{ other.content }, len{ other.size() } {
		other.rvalue_reset();
	}

	/* swap */
	CCDK_FORCEINLINE void swap(bitset& other) {
		util::swap(content, other.content);
		util::swap(len, other.len);
	}

	/* copy assign */
	CCDK_FORCEINLINE this_type& operator=(bitset const& other) {
		
		if( other.count() > count() ){
			// reallocate 
			this->destruct();
			content = allocate_n(other.len);
		}
		memcpy(content, other.content, other.bytes());
		memset(content, 0, bytes())
		len = other.len;
	}

	/* move assign */
	CCDK_FORCEINLINE this_type& operator=(bitset && other) noexcept {
		this->destruct();
		rvalue_set(other.content, other.len);
		other.rvalue_reset();
	}

	/* assign fill */
	CCDK_FORCEINLINE this_type& operator=(bool v) noexcept {
		memset(content, v ? 0b11111111 : 0b00000000, sizeof(T)*count_store(len));
	}

	/* fit size n and fill */
	CCDK_FORCEINLINE this_type& assign(size_type n, bool v) {
		
	}

	/* from start to endd fill v*/
	CCDK_FORCEINLINE this_type& assign(size_type start, size_type end, bool v)
	{

	}


	template<typename InputIt, typename = check_t< is_iterator<InputIt>> >
	CCDK_FORCEINLINE this_type& assign(InputIt beginIt, ptr::size_t n) {
		assign_copy(beginIt, n);
	}

	template<typename InputIt, typename = check_t< is_iterator<InputIt>> >
	CCDK_FORCEINLINE this_type& assign(InputIt beginIt, InputIt endIt) {
		assign_copy(beginIt, alg::distance(beginIt, endIt));
	}

	/* iterator */
	CCDK_FORCEINLINE iterator_type begin() 
		noexcept { return { content, 0 }; }
	CCDK_FORCEINLINE iterator_type end() 
		noexcept { return { content, len }; }
	CCDK_FORCEINLINE const_iterator_type cbegin() 
		const noexcept { return { content, 0 }; }
	CCDK_FORCEINLINE const_iterator_type cend() 
		const noexcept { return { content, len }; }
	CCDK_FORCEINLINE reverse_iterator_type rbegin() 
		noexcept { return { content, len-1 };}
	CCDK_FORCEINLINE reverse_iterator_type rend() 
		noexcept { return { content, -1 }; }
	CCDK_FORCEINLINE const_reverse_iterator_type crbegin() 
		const noexcept { return { content, len-1 }; }
	CCDK_FORCEINLINE const_reverse_iterator_type crend() 
		const noexcept { return { content, -1 }; }

	/* attribute */
	CCDK_FORCEINLINE size_type size() { return len; }
	CCDK_FORCEINLINE size_type count() { return count_store(len); }
	CCDK_FORCEINLINE size_type byte() { return count() * sizeof(T); }
	CCDK_FORCEINLINE size_type capcity() { return count() * kStoreBits; }
	CCDK_FORCEINLINE size_type max_size() { return size_type(-1) / sizeof(T); }

	/* access */
	CCDK_FORCEINLINE reference_type operator[](size_type index) noexcept {
		ccdk_assert(index < len);
		return { content[index / kStoreBits], 1 << (index % kStoreBits) };
	}
	CCDK_FORCEINLINE const_reference_type operator[](size_type index) const noexcept{
		ccdk_assert(index < len);
		return { content[index / kStoreBits], 1 << (index % kStoreBits) };
	}
	CCDK_FORCEINLINE reference_type at(size_type index) noexcept {
		ccdk_assert(index < len);
		return { content[index / kStoreBits], 1 << (index % kStoreBits) };
	}
	CCDK_FORCEINLINE const_reference_type at(size_type index) const noexcept {
		ccdk_assert(index < len);
		return { content[index / kStoreBits], 1 << (index % kStoreBits) };
	}
	CCDK_FORCEINLINE reference_type back()  noexcept {
		return { content[len / kStoreBits], 1 << (len % kStoreBits) };
	}

	CCDK_FORCEINLINE const_reference_type back() const noexcept {
		return { content[len / kStoreBits], 1 << (len % kStoreBits) };
	}

	CCDK_FORCEINLINE this_type& pop_back() noexcept {
		if (len > 0) --len;
	}

	CCDK_FORCEINLINE this_type& push_back(bool v) noexcept {
		if (len == capcity()) {
			this->destruct();
			content = allocator_type::allocate(*this, store_size(len + 1));
		}
		++len;
		back() = v;
	}

	CCDK_FORCEINLINE this_type& insert(size_type pos, bool v) noexcept {
		ccdk_assert(pos >= 0 && pos <= len);
		if (len == capcity()) {
			this->destruct();
			content = allocator_type::allocate(*this, store_size(len + 1));
		}
		for (int i = len-1; i > pos; ++i) {
			*(begin() + i + 1) = *(begin() + i);
		}
		*(begin() + i) = v;
		++len;
	}

	CCDK_FORCEINLINE this_type& erase(size_type ibegin, size_type iend) noexcept {
		ccdk_assert(ibegin >= 0 && iend > ibegin && iend <= len);
		size_type j = ibegin;
		for (size_type i = iend - 1; i < len; ++i, ++j)
			*(begin() + j) = *(begin() + i);
		len -= (iend - ibegin);
	}

	CCDK_FORCEINLINE this_type& erase(size_type pos) noexcept {
		return erase(pos, pos + 1);
	}

	CCDK_FORCEINLINE this_type& erase(const_iterator_type begin, const_iterator_type end) noexcept {
		iterator_type it2 = begin;
		for (iterator_type it = end; it != end(); ++it, ++it2)
			*it2 = *it;
		len -= alg::distance(begin, end);
	}

	CCDK_FORCEINLINE this_type& erase(const_iterator_type pos) noexcept {
		return erase(pos, pos+1);
	}

private:

	CCDK_FORCEINLINE void rvalue_reset() noexcept {
		len = 0;
		content = nullptr;
	}

	CCDK_FORCEINLINE void rvalue_set(T* inContent, size_type n) noexcept {
		content = inContent;
		len = n;
	}

	CCDK_FORCEINLINE size_type count_store(size_type n) const noexcept {
		return (n + sizeof(T) - 1) / sizeof(T);
	}

	CCDK_FORCEINLINE T* allocate_n(size_type n) {
		return allocator_type::allocate(*this, count_store(n));
	}

	CCDK_FORCEINLINE void destruct() {
		allocator_type::deallocate(content, count_store(len));
	}

	CCDK_FORCEINLINE void allocate_fill(size_type n, bool val) {
		if (n > 0) {
			ccdk_increase_allocate3(count_store(n), content, cap, len);
			cap <<= 3;
			len <<= 3;
		}
	}

	template<typename T2>
	CCDK_FORCEINLINE T* reallocate_copy(size_type count, T const* other) {
		if (count > 0) {
			T* memory{ nullptr };
			ccdk_increase_allocate1(count, memory);
			memcpy(memory, content, count / sizeof(T));
		}
	}


	template<typename InputIt>
	CCDK_FORCEINLINE void allocate_copy(size_type n, InputIt beginIt) {
		if (n == 0) return;
		T* memory = allocate_n(n);
		util::copy_n(begin(), beginIt, n);
		len = n;
		content = memory;
	}

	template<typename Char>
	CCDK_FORCEINLINE void allocate_init_from_string(Char const* str, ptr::size_t n) {
		if (!str || n == 0) return;
		content = allocate_n(n);
		len = n;
		for (ptr::size_t i = 0; i < n; ++i) {
			ccdk_assert(str[i] == Char('0') || str[i] == Char('1'));
			this->at(i) =  bool(str[i]-Char('0'));
		}
	}

	template<typename InputIt>
	CCDK_FORCEINLINE void assign_copy(InputIt beginIt, size_type n) {
		if (n > capcity()) {
			this->destruct();
			content = allocator_type::allocate(*this, store_size(n));
		}
		util::copy_n(begin(), beginIt, n);
		len = n;
	}

};

ccdk_namespace_ct_end