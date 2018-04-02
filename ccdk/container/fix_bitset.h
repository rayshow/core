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

#include<ccdk/algorithm/distance.h>
#include<ccdk/memory/simple_new_allocator.h>
#include<ccdk/memory/allocator_traits.h>


ccdk_namespace_ct_start

using namespace ccdk::mpl;

namespace ct_impl {
	template<uint32 NBit, typename T, bool Fit>
	struct fit_size_impl { typedef uint8 type; };
	template<uint32 NBit, bool Fit>
	struct fit_size_impl<NBit, uint8, true> { typedef uint8 type; };
}


template<uint32 NBit, uint32 Bytes, bool = (NBit>sizeof(uint64)) >
struct fit_size_t { typedef uint8 type[ Bytes ]; };
template<uint32 NBit, uint32 Bytes >
struct fit_size_t<NBit, Bytes, false>:ct_impl::fit_size_impl<Bytes,uint8, (Bytes>1)> {};


template< uint32 NBit >
class fix_bitset 
{
public:

	/* common */
	typedef fix_bitset this_type;

	/* container */
	typedef bool                          value_type;
	typedef T*                            pointer_type;
	typedef T const*                      const_reference;
	typedef it::bit_access<T>             reference_type;
	typedef bool                          const_reference_type;
	typedef Size                          size_type;
	typedef int32                         difference_type;
	typedef mem::allocator_traits<Alloc>  allocator_type;

	/* typedef iterator */
	typedef iterator<it::bit_random_category, T, Size>    iterator_type;
	typedef const iterator_type                         const_iterator_type;
	typedef reverse_iterator<iterator_type>             reverse_iterator_type;
	typedef const reverse_iterator_type                 const_reverse_iterator_type;

	static constexpr uint32 kStoreBits = sizeof(T) * 8;

	/* friend */
	template<typename T2, typename Size2, typename Alloc2>
	friend class bitset;

private:
	pointer_type  content;
	size_type     len;

public:

	/* destruct */
	CCDK_FORCEINLINE ~bitset() { destruct(); len = 0; }

	/* default, lazy */
	CCDK_FORCEINLINE constexpr bitset() :content{ nullptr }, len{ 0 } {}
	CCDK_FORCEINLINE constexpr bitset(ptr::nullptr_t) : content{ nullptr }, len{ 0 } {}

	/* fill byte*/
	CCDK_FORCEINLINE explicit bitset(size_type n, bool value = false) {
		allocate_fill(n, value);
	}

	/* initial from constexpr string */
	template<typename Char, ptr::size_t N>
	CCDK_FORCEINLINE explicit bitset(Char(&str)[N]) {
		allocate_init_from_string(str, N - 1);
	}

	/* range-n copy */
	template<typename InputIt,
		typename = check< is_iterator<InputIt>> >
		CCDK_FORCEINLINE bitset(InputIt beginIt, size_type n) {
		allocate_copy(n, beginIt);
	}

	/* range copy */
	template<typename InputIt,
		typename = check< is_iterator<InputIt>> >
		CCDK_FORCEINLINE bitset(InputIt beginIt, InputIt endIt) {
		ptr::size_t n = alg::distance(beginIt, endIt);
		allocate_copy(n, beginIt);
	}

	/* copy */
	CCDK_FORCEINLINE bitset(bitset const& other) :
		content{ allocator_type::allocate(*this, count_store(other.len)) },
		len{ other.len } {
		memcpy(content, other.content, count_store(other.len));
	}

	/* move */
	CCDK_FORCEINLINE bitset(bitset && other)  noexcept
		: content{ other.content }, len{ other.len } {
		other.rvalue_reset();
	}

	/* swap */
	CCDK_FORCEINLINE void swap(bitset& other) {
		util::swap(content, other.content);
		util::swap(len, other.len);
	}

	/* copy assign */
	CCDK_FORCEINLINE this_type& operator=(bitset const& other) {
		ptr::size_t other_store = store_size(other.len);
		if (store_size(len) < other_store) {
			this->destruct();
			content = allocate_n(other.len);
			len = other.len;
		}
		memcpy(content, other.content, other_store);
	}

	/* move assign */
	CCDK_FORCEINLINE this_type& operator=(bitset && other) noexcept {
		this->destruct();
		rvalue_set(other);
		other.rvalue_reset();
	}

	CCDK_FORCEINLINE this_type& operator=(bool v) {
		uint8 mask = v ? 0x11111111 : 0x00000000;
		memset(content, mask, store_size(len));
	}


	CCDK_FORCEINLINE this_type& assign(size_type n, bool v) {
		if (n > capcity()) {
			this->destruct();
			allocator_type::allocate(*this, store_size(n));
		}
		uint8 mask = v ? 0b11111111 : 0b00000000;
		memset(content, mask, store_size(len));
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
		noexcept {
		return { content, 0 };
	}
	CCDK_FORCEINLINE iterator_type end()
		noexcept {
		return { content, len };
	}
	CCDK_FORCEINLINE const_iterator_type cbegin()
		const noexcept {
		return { content, 0 };
	}
	CCDK_FORCEINLINE const_iterator_type cend()
		const noexcept {
		return { content, len };
	}
	CCDK_FORCEINLINE reverse_iterator_type rbegin()
		noexcept {
		return { content, len - 1 };
	}
	CCDK_FORCEINLINE reverse_iterator_type rend()
		noexcept {
		return { content, -1 };
	}
	CCDK_FORCEINLINE const_reverse_iterator_type crbegin()
		const noexcept {
		return { content, len - 1 };
	}
	CCDK_FORCEINLINE const_reverse_iterator_type crend()
		const noexcept {
		return { content, -1 };
	}

	/* attribute */
	CCDK_FORCEINLINE size_type size() { return len; }
	CCDK_FORCEINLINE size_type capcity() { return store_size() * kStoreBits; }
	CCDK_FORCEINLINE size_type max_size() { return size_type(-1) / sizeof(T); }

	/* access */
	CCDK_FORCEINLINE reference_type operator[](size_type index) noexcept {
		ccdk_assert(index < len);
		return { content[index / kStoreBits], 1 << (index % kStoreBits) };
	}
	CCDK_FORCEINLINE const_reference_type operator[](size_type index) const noexcept {
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
		for (int i = len - 1; i > pos; ++i) {
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
		return erase(pos, pos + 1);
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

	CCDK_FORCEINLINE size_type store_size(size_type n) const noexcept {
		return (n + sizeof(T) - 1) / sizeof(T);
	}

	CCDK_FORCEINLINE T* allocate_n(size_type n) {
		return allocator_type::allocate(*this, count_store(n));
	}

	CCDK_FORCEINLINE void destruct() {
		allocator_type::deallocate(content, count_store(len));
	}

	CCDK_FORCEINLINE void allocate_fill(size_type n, bool val) {
		if (n == 0) return;
		T* memory = allocate_n(n);
		uint8 mask = val ? 0b11111111 : 0b00000000;
		memset(memory, mask, sizeof(T)*count);
		len = n;
		content = memory;
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
			this->at(i) = str[i];
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