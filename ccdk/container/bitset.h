#pragma once

#include<ccdk/container/container_mudule.h>
#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/base/logic_.h>
#include<ccdk/mpl/base/compatible_op.h>
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
	using value_type           = T;
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
	template<typename T2, typename Size2, typename Ratio2, typename Alloc2>
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
		local_allocate_n(other.len);
		memcpy(content, other.content, other.bytes());
	}

	/* template copy */
	template<typename Size2, typename T2, typename Alloc2>
	CCDK_FORCEINLINE bitset(bitset<T2, Size2, Alloc2> const& other) {
		local_allocate_n(other.len);
		memcpy(content, other.content, other.bytes());
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

	/*  assign copy */
	CCDK_FORCEINLINE this_type& operator=(bitset const& other) {
		assign_copy(other.content, other.len);
	}

	/* template assign copy   */
	template<typename T2, typename Size2, typename Ratio2, typename Alloc2>
	CCDK_FORCEINLINE this_type& operator=(bitset<T2,Size2,Ratio2,Alloc2> const& other) {
		assign_copy(other.content, other.len);
	}

	/* move assign */
	CCDK_FORCEINLINE this_type& operator=(bitset && other) noexcept {
		this->deallocate();
		rvalue_set(other.content, other.len, other.cap);
		other.rvalue_reset();
	}

	/* move assign */
	template<typename Size2, typename Ratio2>
	CCDK_FORCEINLINE this_type& operator=(bitset<T,Size2,Ratio2,Alloc> && other) noexcept {
		this->deallocate();
		rvalue_set(other.content, other.len, other.cap);
		other.rvalue_reset();
	}

	/* assign fill */
	CCDK_FORCEINLINE this_type& operator=(bool v) noexcept {
		memset(content, v ? 0b11111111 : 0b00000000, bytes());
		clear_ext_bits();
	}

	/* fit size n and fill */
	CCDK_FORCEINLINE this_type& assign(size_type n, bool v) {
		if (n > cap) {
			this->deallocate();
			this->local_allocate_n(n);
		}
		else { len = n; }
		memset(content, v ? 0b11111111 : 0b00000000, bytes());
		clear_ext_bits();
	}

	/* from start to endd fill v*/
	CCDK_FORCEINLINE this_type& assign(size_type istart, size_type iend, bool v) {
		ccdk_assert(start < end && end <= cap);
		auto end = begin() + iend;
		for (auto it = begin() + istart; it != end; ++it) {
			*it = v;
		}
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
	CCDK_FORCEINLINE iterator_type begin() noexcept {
		return { content, 0, 1 };
	}
	CCDK_FORCEINLINE iterator_type end() noexcept {
		return { content, count()-1, cshl<T>(1, keep_low<T,T>(len)) };
	}
	CCDK_FORCEINLINE constexpr const_iterator_type cbegin() const noexcept {
		return { content, 0, 1 };
	}
	CCDK_FORCEINLINE constexpr const_iterator_type cend() const noexcept {
		return { content, count() - 1, cshl<T>(1, keep_low<T,T>(len)) };
	}
	CCDK_FORCEINLINE reverse_iterator_type rbegin() noexcept {
		return { --end() };
	}
	CCDK_FORCEINLINE reverse_iterator_type rend() noexcept {
		return { --begin() };
	}
	CCDK_FORCEINLINE constexpr const_reverse_iterator_type crbegin() const noexcept {
		return { end()-1 };
	}
	CCDK_FORCEINLINE constexpr const_reverse_iterator_type crend() const noexcept {
		return { begin()-1 };
	}

	/* attribute */
	CCDK_FORCEINLINE size_type size() const noexcept { return len; }
	CCDK_FORCEINLINE size_type capcity() const noexcept { return cap; }
	CCDK_FORCEINLINE size_type max_size() const noexcept { return size_type(-1) / sizeof(T); }

	/* index */
	CCDK_FORCEINLINE reference_type operator[](size_type index) noexcept {
		ccdk_assert(index < len);
		return { content[index / kStoreBits], cshl<value_type>(1, index % kStoreBits) };
	}

	/* const index, return bool */
	CCDK_FORCEINLINE constexpr const_reference_type operator[](size_type index) const noexcept {
		ccdk_assert(index < len);
		return content[index / kStoreBits] & cshl<value_type>(1, index % kStoreBits);
	}
	CCDK_FORCEINLINE reference_type at(size_type index) noexcept {
		ccdk_assert(index < len);
		return { content[index / kStoreBits], cshl<value_type>(1,index%kStoreBits) };
	}
	CCDK_FORCEINLINE constexpr const_reference_type at(size_type index) const noexcept {
		ccdk_assert(index < len);
		return content[index / kStoreBits] & cshl<value_type>(1, (index % kStoreBits));
	}

	/* last bit reference */
	CCDK_FORCEINLINE reference_type back()  noexcept {
		return { content[len / kStoreBits], cshl<value_type>(1, index % kStoreBits) };
	}

	/* last bit value */
	CCDK_FORCEINLINE constexpr const_reference_type back() const noexcept {
		return { content[len / kStoreBits], cshl<value_type>(1, NBit % kStoreBits) };
	}

	CCDK_FORCEINLINE this_type& pop_back() noexcept {
		if (len > 0) {
			back() = false;
			--len;
		}
	}

	CCDK_FORCEINLINE this_type& push_back(bool v) noexcept {
		if (len == cap) {
			this->deallocate();
			local_reallocate_copy(len);
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
		cap = 0;
		content = nullptr;
	}

	CCDK_FORCEINLINE void rvalue_set(T* inContent, size_type n, size_type c) noexcept {
		content = inContent;
		len = n;
		cap = c;
	}

	CCDK_FORCEINLINE void deallocate() {
		allocator_type::deallocate(content, count());
	}

	/* count of used elements */
	CCDK_FORCEINLINE size_type count() const noexcept { 
		return shr_type<T, size_type>(len); 
	}

	/* bytes of used elements */
	CCDK_FORCEINLINE size_type bytes() const noexcept { 
		return shr_type<uint8, size_type>(len); 
	}

	/* not used bytes of storage */
	CCDK_FORCEINLINE size_type ext_bytes() const noexcept { 
		return shr_type<uint8, size_type>(cap) - bytes(); 
	}

	/* clear last used element's not used bits */
	CCDK_FORCEINLINE size_type clear_ext_bits() noexcept {
		if (content) {
			size_type ext_bits = shl_type<T, size_type>(count()) - len;
			T mask{ 0 };
			while (ext_bits--) mask |= cshl<T>(1, ext_bits);
			content[count() - 1] & = ~mask;
		}
	}

	CCDK_FORCEINLINE void local_allocate_n(size_type n) {
		ccdk_increase_allocate3(count_store(n), content, cap, len);
		cap = shl_type<T, size_type>(cap);
		len = n;
	}

	CCDK_FORCEINLINE void clear_ext_bit_and_storage() noexcept {
		clear_ext_bits();                           /* clear last T' ext bits */
		memset(content + bytes(), 0, ext_bytes());  /* clear ext storage */
	}

	CCDK_FORCEINLINE void allocate_fill(size_type n, bool val) {
		if (n > 0) {
			local_allocate_n(n);
			memset(content, val ? 0b11111111 : 0b00000000, bytes());
			clear_ext_bit_and_storage();
		}
	}

	template<typename T2>
	CCDK_FORCEINLINE void local_reallocate_copy(size_type n) {
		if (n > 0) {
			T* memory{ nullptr };
			size_type new_len, new_cap;
			ccdk_increase_allocate3( shr_type<T,size_type>(n), memory, new_cap, new_len);
			new_len = n;
			new_cap = shl_type<T, size_type>(new_cap);
			memcpy(memory, content, bytes());
			this->deallocate();
			len = new_len;
			cap = new_cap;
		}
	}


	template<typename InputIt>
	CCDK_FORCEINLINE void allocate_copy(size_type n, InputIt beginIt) {
		if (n > 0) {
			local_allocate_n(b);
			util::copy_n(begin(), beginIt, n);
			clear_ext_bit_and_storage();
		}
	}

	template<typename Char>
	CCDK_FORCEINLINE void allocate_init_from_string(Char const* str, ptr::size_t n) {
		if (!str || n == 0) return;
		local_allocate_n(n);
		for (ptr::size_t i = 0; i < n; ++i) {
			ccdk_assert(str[i] == Char('0') || str[i] == Char('1'));
			this->at(i) =  bool(str[i]-Char('0'));
		}
		clear_ext_bit_and_storage();
	}

	template<typename T2>
	CCDK_FORCEINLINE void assign_copy(T2* memory, size_type n) {
		if (n > count()) {
			// reallocate 
			this->deallocate();
			local_allocate_n(n);
		}
		else { len = n; }
		memcpy(content, memory, bytes());               /* copy content */
		memset(content + bytes(), 0, ext_bytes());      /* clear other */
	}

};

ccdk_namespace_ct_end