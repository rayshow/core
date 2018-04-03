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

#define RECURSIVE_TYPEDEF(Type) using type =  typename fit_bytes<Bytes,Type, Bytes <=sizeof(Type)>::type

/* fit bytes with a inner basic type  */
template<uint32 Bytes, typename T = uint8 , bool Fit = (Bytes<= sizeof(T)) > struct fit_bytes;
template<uint32 Bytes, typename T> struct fit_bytes<Bytes, T, true> { typedef T type[Bytes / sizeof(T)];  };
template<uint32 Bytes> struct fit_bytes<Bytes, uint8, false>  { RECURSIVE_TYPEDEF(uint16); };
template<uint32 Bytes> struct fit_bytes<Bytes, uint16, false> { RECURSIVE_TYPEDEF(uint32); };
template<uint32 Bytes> struct fit_bytes<Bytes, uint32, false> { RECURSIVE_TYPEDEF(uint64); };
template<uint32 Bytes> struct fit_bytes<Bytes, uint64, false>:fit_bytes<Bytes, uint8, true> {  };


template<typename T, typename Char>
constexpr T parse_int(const Char* str, uint32 pos, uint32 max)
{
	uint32 begin = pos * 16;
	if (begin >= max) return 0;
	uint32 end =  begin + 16;
	end = max < end ? max : end;
	T val = 0;
	for (uint32 i = begin; i < end; ++i) {
		val = (val << 1) | (str[i] - '0');
	}
	return val;
}

struct constexpr_string_ctor {};
constexpr constexpr_string_ctor constexpr_string_ctor_c{};

template< uint32 NBit >
class fix_bitset 
{
public:
	/* common */
	static constexpr uint32 Bytes = (NBit + 7) / 8;
	using this_type   = fix_bitset;
	using array_type  = typename fit_bytes<Bytes>::type;
	using single_type = remove_dim_t<array_type>;
	static constexpr uint32 N = sizeof(array_type) / sizeof(single_type);


	/* container */
	using value_type           = single_type;
	using pointer_type         = value_type*;
	using const_pointer_type   = value_type const*;
	using reference_type       = it::bit_access<value_type>;
	using const_reference_type = bool;
	using size_type            = uint32;
	using difference_type      = ptr::diff_t;
	

	/* typedef iterator */
	using iterator_type               = iterator<it::bit_random_category, value_type, size_type>;
	using const_iterator_type         = const iterator_type;
	using reverse_iterator_type       = reverse_iterator<iterator_type>;
	using const_reverse_iterator_type = const reverse_iterator_type;

	/* friend */
	template<uint32 NBit2> friend class fix_bitset;

private:
	value_type content[N];

	/* aux compile-time fill-ctor */
	template<value_type... Args>
	CCDK_FORCEINLINE constexpr fix_bitset(mpl::val_pack<value_type, Args...>) 
		noexcept : content{ Args... } {}

	/* aux compile-time fill from constexpr-string */
	template<typename Char, value_type... Args>
	CCDK_FORCEINLINE constexpr fix_bitset(const Char* str, uint32 L, mpl::val_pack<uint32, Args...>)
		noexcept : content{ parse_int<value_type,Char>(str, Args, L)... } {
	}

public:
	/* constexpr default */
	CCDK_FORCEINLINE constexpr fix_bitset() noexcept : content{ 0 } {}

	/* compile time fill */
	template<bool v>
	CCDK_FORCEINLINE constexpr fix_bitset(mpl::bool_<v>) noexcept
		:fix_bitset(mpl::dup_to_val_pack_c<N,value_type,v?0:-1>) {}

	/* compile time fill from constexpr string, exclude end '\0' */
	template<typename Char, ptr::size_t L>
	CCDK_FORCEINLINE constexpr explicit fix_bitset(constexpr_string_ctor,Char(&str)[L]) noexcept
		: fix_bitset(str, mpl::min_val<uint32, NBit,L-1>, mpl::make_indice_c<N> ){}

	/* runtime fill */
	CCDK_FORCEINLINE explicit fix_bitset(bool v) noexcept {
		memset(util::addressof(content), v ? 0b11111111 : 0b00000000, Bytes);
	} 

	/* runtime string */
	template<typename Char>
	CCDK_FORCEINLINE explicit fix_bitset(Char const* str) noexcept {
		memset(util::addressof(content), 0 ? 0b11111111 : 0b00000000, Bytes);
	}

	/* range-n copy */
	template<typename InputIt, typename = check_t< is_iterator<InputIt>> >
	CCDK_FORCEINLINE constexpr fix_bitset(InputIt beginIt, size_type n) noexcept {
		if (n > 0) { util::copy_n(begin(), beginIt, n); }
	}

	/* range copy */
	template<typename InputIt, typename = check_t< is_iterator<InputIt>> >
	CCDK_FORCEINLINE constexpr fix_bitset(InputIt beginIt, InputIt endIt) noexcept {
		util::copy_n(begin(), beginIt, alg::distance(beginIt, endIt));
	}

	/* copy */
	CCDK_FORCEINLINE constexpr fix_bitset(fix_bitset const& other) noexcept{
		for (uint32 i = 0; i < Size; ++i) {
			content[i] = other.content[i];
		}
	}

	/* template copy */
	template<uint32 N2>
	CCDK_FORCEINLINE constexpr fix_bitset(fix_bitset<N2> const& other) noexcept {
		
	}

	/* swap */
	CCDK_FORCEINLINE void swap(fix_bitset& other) {
		util::swap(content, other.content);
	}

	/* copy assign */
	CCDK_FORCEINLINE this_type& operator=(fix_bitset const& other) {
		for (uint32 i = 0; i < Size; ++i) {
			content[i] = other.content[i];
		}
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
	CCDK_FORCEINLINE iterator_type begin() noexcept { return { content, 0 }; }
	CCDK_FORCEINLINE iterator_type end() noexcept { return { content, len }; }
	CCDK_FORCEINLINE const_iterator_type cbegin() const noexcept { return { content, 0 }; }
	CCDK_FORCEINLINE const_iterator_type cend() const noexcept { return { content, N }; }
	CCDK_FORCEINLINE reverse_iterator_type rbegin() noexcept { return { content, len - 1 }; }
	CCDK_FORCEINLINE reverse_iterator_type rend() noexcept { return { content, -1 }; }
	CCDK_FORCEINLINE const_reverse_iterator_type crbegin() const noexcept { return { content, len - 1 }; }
	CCDK_FORCEINLINE const_reverse_iterator_type crend() const noexcept { return { content, -1 }; }

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

private:

	CCDK_FORCEINLINE void rvalue_reset() noexcept {
		len = 0;
		content = nullptr;
	}

	CCDK_FORCEINLINE void rvalue_set(value_type* inContent, size_type n) noexcept {
		content = inContent;
		len = n;
	}

	CCDK_FORCEINLINE size_type store_size(size_type n) const noexcept {
		return (n + sizeof(T) - 1) / sizeof(T);
	}

	CCDK_FORCEINLINE value_type* allocate_n(size_type n) {
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