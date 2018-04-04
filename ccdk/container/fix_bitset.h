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
template<uint32 Bytes, typename T> struct fit_bytes<Bytes, T, true> { typedef T type[ (Bytes+ sizeof(T)-1) / sizeof(T)];  };
template<uint32 Bytes> struct fit_bytes<Bytes, uint8, false>  { RECURSIVE_TYPEDEF(uint16); };
template<uint32 Bytes> struct fit_bytes<Bytes, uint16, false> { RECURSIVE_TYPEDEF(uint32); };
template<uint32 Bytes> struct fit_bytes<Bytes, uint32, false> { RECURSIVE_TYPEDEF(uint64); };
template<uint32 Bytes> struct fit_bytes<Bytes, uint64, false>:fit_bytes<Bytes, uint8, true> {  };

#undef RECURSIVE_TYPEDEF

template<typename T, typename Char>
constexpr T parse_int(const Char* str, uint32 pos, uint32 max) {
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
	static constexpr uint32 kStoreBits = sizeof(single_type) * 8;
	static constexpr single_type kTopMask = 1 << (kStoreBits - 1);
	static constexpr single_type kLastOffset = NBit / kStoreBits;
	static constexpr single_type kLastMask = 1 << (NBit % kStoreBits);
	static constexpr single_type kReverseOffst = (NBit - 1) / kStoreBits;
	static constexpr single_type kReverseMask = 1 <<((NBit - 1) % kStoreBits);

	/* container */
	using value_type           = single_type;
	using pointer_type         = value_type*;
	using const_pointer_type   = value_type const*;
	using reference_type       = it::bit_access<value_type>;
	using const_reference_type = bool;
	using size_type            = uint32;
	using difference_type      = ptr::diff_t;
	

	/* typedef iterator */
	using iterator_type               = iterator<bit_random_category, value_type, size_type>;
	using const_iterator_type         = iterator<bit_random_category, const value_type, size_type>;
	using reverse_iterator_type       = reverse_iterator<iterator_type>;
	using const_reverse_iterator_type = reverse_iterator<const_iterator_type>;

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
	CCDK_FORCEINLINE constexpr fix_bitset(const Char* str, uint32 L,
		mpl::val_pack<uint32, Args...>)
		noexcept : content{ parse_int<value_type,Char>(str, Args, L)... } { }

	/* aux compile-time copy */
	template<value_type... Args>
	CCDK_FORCEINLINE constexpr fix_bitset(const value_type* other, mpl::val_pack<uint32, Args...>)
		noexcept : content{ other[Args]... } {
	}

public:
	/* constexpr default */
	CCDK_FORCEINLINE constexpr fix_bitset() noexcept : content{ 0 } {}

	/* compile time fill */
	template<bool v>
	CCDK_FORCEINLINE constexpr fix_bitset(mpl::bool_<v>) noexcept
		:fix_bitset(mpl::dup_to_val_pack_c<N,value_type,v?-1:0>) {}

	/* compile time fill from constexpr string, exclude end '\0' */
	template<typename Char, ptr::size_t L>
	CCDK_FORCEINLINE constexpr explicit fix_bitset(
		constexpr_string_ctor, Char(&str)[L]) noexcept
		: fix_bitset(str, mpl::min_val<uint32, NBit,L-1>, mpl::make_indice_c<N> ){}

	/* runtime fill */
	CCDK_FORCEINLINE explicit fix_bitset(bool v) noexcept {
		memset(content, v ? 0b11111111 : 0b00000000, Bytes);
	} 

	/* runtime string */
	CCDK_FORCEINLINE explicit fix_bitset(char const* str) noexcept :content{ 0 } {
		ptr::size_t len = fn::min(NBit, strlen(str));
		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < kStoreBits; ++j) {
				uint32 pos = ( i << shift_bit<kStoreBits>::value ) + j;
				if (pos >= len) return;
				ccdk_assert(str[pos] - '0' == 1 || str[pos] - '0' == 0);
				content[i] |= (str[pos] - '0') << j;
			}
		}
	}

	/* range-n copy */
	template<typename InputIt, typename = check_t< is_iterator<InputIt>> >
	CCDK_FORCEINLINE fix_bitset(InputIt beginIt, size_type n) noexcept:content { 0 } {
		util::copy_n(begin(), beginIt, fn::min(NBit, n) );
	}

	/* range copy */
	template<typename InputIt, typename = check_t< is_iterator<InputIt>> >
	CCDK_FORCEINLINE fix_bitset(InputIt beginIt, InputIt endIt) noexcept {
		util::copy_n(begin(), beginIt, alg::distance(beginIt, endIt));
	}

	/* compile copy */
	CCDK_FORCEINLINE constexpr fix_bitset(fix_bitset const& other)
		noexcept:fix_bitset{ other.content, mpl::make_indice_c<N> } { }

	/* template copy */
	template<uint32 NBit2>
	CCDK_FORCEINLINE fix_bitset(fix_bitset<NBit2> const& other) noexcept : content{} {
		memcpy(content, other.content, fn::min(Bytes, other.Bytes));
	}

	/* swap */
	CCDK_FORCEINLINE void swap(fix_bitset& other) {
		util::swap(content, other.content);
	}

	/* copy assign */
	CCDK_FORCEINLINE this_type& operator=(fix_bitset const& other) {
		for (uint32 i = 0; i < N; ++i) { content[i] = other.content[i]; }
		return *this;
	}

	/* template copy assign */
	template<uint32 NBit2>
	CCDK_FORCEINLINE this_type& operator=(fix_bitset<NBit2> const& other) {
		memcpy(content, other.content, fn::min(Bytes, other.Bytes));
		return *this;
	}

	/* fill assign */
	CCDK_FORCEINLINE this_type& operator=(bool v) {
		memset(content, v ? 0x11111111 : 0x00000000, Bytes);
		return *this;
	}

	/* fill range */
	CCDK_FORCEINLINE this_type& assign(size_type start, size_type end, bool v) {
		ccdk_assert(start < end && end <= NBit);
		for (size_type i = start; i < end; ++i) at(i) = v;
		return *this;
	}

	template<typename InputIt, typename = check_t< is_iterator<InputIt>> >
	CCDK_FORCEINLINE this_type& assign(InputIt beginIt, ptr::size_t n) {
		ccdk_assert(n <= NBit);
		util::copy_n(begin(), beginIt, n);
	}

	template<typename InputIt, typename = check_t< is_iterator<InputIt>> >
	CCDK_FORCEINLINE this_type& assign(InputIt beginIt, InputIt endIt) {
		util::copy_range(begin(), beginIt, endIt);
	}

	/* iterator */
	CCDK_FORCEINLINE iterator_type begin()
		noexcept { return { content, 0,1 }; }
	CCDK_FORCEINLINE iterator_type end()
		noexcept { return { content, kLastOffset, kLastMask }; }
	CCDK_FORCEINLINE constexpr const_iterator_type cbegin()
		const noexcept { return { content, 0, 1 }; }
	CCDK_FORCEINLINE constexpr const_iterator_type cend()
		const noexcept { return { content, kLastOffset, kLastMask }; }
	CCDK_FORCEINLINE reverse_iterator_type rbegin()
		noexcept { return { { content, kReverseOffst, kReverseMask } }; }
	CCDK_FORCEINLINE reverse_iterator_type rend()
		noexcept { return { {content, size_type(-1),kTopMask} }; }
	CCDK_FORCEINLINE constexpr const_reverse_iterator_type crbegin()
		const noexcept { return { { content, kReverseOffst, kReverseMask} }; }
	CCDK_FORCEINLINE constexpr const_reverse_iterator_type crend()
		const noexcept { return { { content, size_type(-1), kTopMask } }; }

	/* attribute */
	CCDK_FORCEINLINE constexpr size_type size() { return NBit; }
	CCDK_FORCEINLINE constexpr size_type capcity() { return Bytes * 8; }
	CCDK_FORCEINLINE constexpr size_type max_size() { return size_type(-1) / sizeof(value_type); }

	/* index */
	CCDK_FORCEINLINE reference_type operator[](size_type index) noexcept {
		ccdk_assert(index < NBit);
		return { content[index / kStoreBits], value_type(1 << (index % kStoreBits)) };
	}
	CCDK_FORCEINLINE constexpr const_reference_type operator[](size_type index) const noexcept {
		ccdk_assert(index < len);
		return content[index / kStoreBits] & (1 << (index % kStoreBits));
	}
	CCDK_FORCEINLINE reference_type at(size_type index) noexcept {
		ccdk_assert(index < NBit );
		return { content[index / kStoreBits], value_type(1 << (index % kStoreBits)) };
	}
	CCDK_FORCEINLINE constexpr const_reference_type at(size_type index) const noexcept {
		ccdk_assert(index < NBit);
		return content[index / kStoreBits] & (1 << (index % kStoreBits));
	}

	CCDK_FORCEINLINE reference_type back()  noexcept {
		return { content[len / kStoreBits], 1 << (len % kStoreBits) };
	}

	CCDK_FORCEINLINE constexpr const_reference_type back() const noexcept {
		return { content[len / kStoreBits], 1 << (len % kStoreBits) };
	}

	CCDK_FORCEINLINE constexpr size_type count_one() {
		auto e = end();
		auto count = 0;
		for (auto it = begin(); it != e; ++it)
			if (*it) ++count;
		return count;
	}

	CCDK_FORCEINLINE constexpr size_type count_zero() {
		return NBit - count_one();
	}

	/* weather all bit is set */
	CCDK_FORCEINLINE constexpr bool all() {
		for (int i = 0; i < N; ++i) {
			if (content[i] != value_type(-1)) return false;
		}
		return true;
	}

	/* weather at least 1bit is set */
	CCDK_FORCEINLINE constexpr bool any() {
		for (int i = 0; i < N; ++i) {
			if (content[i] != 0) return true;
		}
		return false;
	}

	/* none bit is set */
	CCDK_FORCEINLINE constexpr bool none() { return !any(); }

	/* operation */
	CCDK_FORCEINLINE this_type& operator&=(this_type const& other) noexcept {
		for (int i = 0; i < N; ++i) { content[i] &= other.content[i]; }
	}
	CCDK_FORCEINLINE this_type& operator|=(this_type const& other) noexcept {
		for (int i = 0; i < N; ++i) { content[i] |= other.content[i]; }
	}
	CCDK_FORCEINLINE this_type& operator^=(this_type const& other) noexcept {
		for (int i = 0; i < N; ++i) { content[i] ^= other.content[i]; }
	}
	CCDK_FORCEINLINE this_type& flip() noexcept {
		for (int i = 0; i < N; ++i) { content[i] = ~content[i]; }
	}
	/* not circler shift right */
	CCDK_FORCEINLINE this_type& operator>>=(size_type shr) noexcept {
		if (shr > 0) {
			uint32 wsft = shr / kStoreBits;
			uint32 lsft = shr - wsft*kStoreBits;
			uint32 hsft = kStoreBits - lsft;
			for (uint32 i = N - 1; i > wsft; ++i) {
				content[i] = (content[i-wsft] >> lsft) | (content[i -wsft- 1] << hsft);
			}
			content[wsft] = content[0] >> lsft;
			memset(content, 0, wsft*Bytes);
			clear_external();
		}
	}

	CCDK_FORCEINLINE this_type& clear_external() noexcept {
		uint32 ext = N * kStoreBits - NBit;
		value_type mask = 0;
		for (int i = 0; i < ext; ++i) mask |= (1 << i);
		content[N - 1] &= mask;
	}

	CCDK_FORCEINLINE this_type& operator<<=(size_type shl) noexcept {
		uint32 lbit = shl % kStoreBits;
		uint32 rbit = kStoreBits - lbit;
		if (sft > 0) {
			for (uint32 i = 0; i<N-1; ++i) {
				content[i] = (content[i] << lsft) | (content[i + 1] >> hsft);
			}
			content[N-1] = content[N-1] << lsft;
		}

	}
	CCDK_FORCEINLINE this_type operator&(this_type const& other) const noexcept {
		return this_type{ *this } &= other;
	}
	CCDK_FORCEINLINE this_type operator|(this_type const& other) const noexcept {
		return this_type{ *this } |= other;
	}
	CCDK_FORCEINLINE this_type operator^(this_type const& other) const noexcept {
		return this_type{ *this } ^= other;
	}
	CCDK_FORCEINLINE this_type operator>>(size_type pos) const noexcept {
		return this_type{ *this } >>= pos;
	}
	CCDK_FORCEINLINE this_type operator<<(size_type pos) const noexcept {
		return this_type{ *this } <<= pos;
	}
	CCDK_FORCEINLINE this_type operator~() noexcept {
		return this_type{ *this }.flip();
	}


///////////////////////////////////////////////////////////////////////////////
//// debug function 

	CCDK_FORCEINLINE void debug() const noexcept {
		char info[NBit + 1]{ 0 };
		for (int i = 0; i < NBit; ++i) {
			info[i] = this->at(i) ? '1' : '0';
		}
		DebugValue("fix_bit:", info);
	}

	CCDK_FORCEINLINE constexpr bool operator==(this_type const& other) const noexcept {
		for (int i = 0; i < N; ++i) {
			if (content[i] != other.content[i]) return false;
		}
		return true;
	}


	CCDK_FORCEINLINE void debug_iterator() noexcept{
		char info[N*kStoreBits+1]{ 0 };
		int j = 0;
		
		for (auto i = begin(); i != end(); ++i,++j) {
			bool v = *i;
			info[j] = v ? '1' : '0';
		}
		DebugValue("fix_bit:", info);
	}

	CCDK_FORCEINLINE void debug_reverse_iterator() noexcept {
		char info[N*kStoreBits + 1]{ 0 };
		int j = 0;

		for (auto i = rbegin(); i != rend(); ++i, ++j) {
			bool v = *i;
			info[j] = v ? '1' : '0';
		}
		DebugValue("fix_bit reverse :", info);
	}

	CCDK_FORCEINLINE void debug_const_reverse_iterator() const noexcept {
		char info[N*kStoreBits + 1]{ 0 };
		int j = 0;

		for (auto i = crbegin(); i != crend(); ++i, ++j) {
			bool v = *i;
			info[j] = v ? '1' : '0';
		}
		DebugValue("fix_bit reverse :", info);
	}

	CCDK_FORCEINLINE void debug_const_iterator()  const noexcept {
		char info[N*kStoreBits + 1]{ 0 };
		int j = 0;

		for (auto i = cbegin(); i != cend(); ++i, ++j) {
		
			bool v = *i;
			info[j] = v ? '1' : '0';
		}
		DebugValue("fix_bit:", info);
	}
};

ccdk_namespace_ct_end