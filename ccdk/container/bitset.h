#pragma once

#include<ccdk/container/container_mudule.h>
#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/base/logic_.h>
#include<ccdk/mpl/base/compatible_op.h>
#include<ccdk/mpl/type_traits/is_same.h>
#include<ccdk/mpl/iterator/iterator_traits.h>
#include<ccdk/mpl/iterator/bit_iterator.h>
#include<ccdk/mpl/iterator/reverse_iterator.h>
#include<ccdk/mpl/iterator/algorithm/distance.h>
#include<ccdk/mpl/util/copy.h>
#include<ccdk/mpl/util/swap.h>
#include<ccdk/mpl/units/ratio.h>

#include<ccdk/text/char_traits.h>
#include<ccdk/memory/simple_new_allocator.h>
#include<ccdk/memory/allocator_traits.h>

ccdk_namespace_ct_start

using namespace ccdk::mpl;

template<
	typename T = uint8,
	typename Size = uint32,
	typename InceaseRatio = units::ratio<1,1>,   /* pre-allocate x times more space */
	typename Alloc = mem::simple_new_allocator<T>
>
class bitset : protected Alloc
{
public:
	/* common */
	using this_type      = bitset;
	using increase_ratio = InceaseRatio;

	/* container */
	using value_type      = T;
	using pointer         = T*;
	using const_pointer   = T const*;
	using reference       = it::bit_access<T>;
	using const_reference = bool;
	using size_type       = Size;
	using difference_type = ptr::diff_t;
	using allocator_type  = mem::allocator_traits<Alloc>;

	/* iterator */
	using iterator               = it::iterator<bit_random_category, value_type, size_type>;
	using const_iterator         = it::iterator<bit_random_category, const value_type, size_type>;
	using reverse_iterator       = it::reverse_iterator<iterator>;
	using const_reverse_iterator = it::reverse_iterator<const_iterator>;

	static constexpr uint32 kStoreBits = sizeof(T)*8;
	static constexpr uint32 kComplements = kStoreBits - 1;

	/* friend */
	template<typename T2, typename Size2, typename Ratio2, typename Alloc2>
	friend class bitset;

private:
	pointer   content;
	size_type len;
	size_type cap;

	CCDK_FORCEINLINE size_type total_bytes() const noexcept { return cap >> 3; }
	CCDK_FORCEINLINE size_type used_bytes() const noexcept { return (len + 7) >> 3; }
	CCDK_FORCEINLINE size_type total_count() 
		const noexcept { return shr_type<T, size_type>(cap); }
	CCDK_FORCEINLINE size_type last_pos() 
		const noexcept { ccdk_assert(len > 0);  return shr_type<T, size_type>(len);  }
	CCDK_FORCEINLINE size_type last_offset() 
		const noexcept { return mod_type<value_type, size_type>(len); }
	CCDK_FORCEINLINE value_type last_mask() 
		const noexcept { return cshl<value_type>(1, last_offset()); }

public:

	/* destruct */
	CCDK_FORCEINLINE ~bitset() { deallocate(); len = 0; cap = 0; }

	/* default, lazy */
	CCDK_FORCEINLINE constexpr bitset() 
		:content{ nullptr }, len{ 0 }, cap{ 0 } {}
	CCDK_FORCEINLINE constexpr bitset(ptr::nullptr_t) 
		: content{ nullptr }, len{ 0 }, cap{ 0 } {}

	/* fill byte*/
	CCDK_FORCEINLINE explicit bitset(size_type n, bool value = false) {
		allocate_fill(n, value);
	}

	/* initial from constexpr string */
	template<typename Char, ptr::size_t N>
	CCDK_FORCEINLINE bitset( 
		from_string_literial, Char (&str)[N]) {
		allocate_init_from_string(str, N-1);
	}

	/* initial from constexpr string */
	template<typename Char, ptr::size_t N>
	CCDK_FORCEINLINE explicit bitset(
		from_string_literial, size_type n, Char(&str)[N]) {
		allocate_init_from_string(str, fn::min(n, N - 1));
	}


	/* initial from string pointer, need count string length */
	template<typename Char>
	CCDK_FORCEINLINE explicit bitset(Char const* str) {
		allocate_init_from_string(str, txt::char_traits<Char>::length(str));
	}

	/* range-n copy */
	template<typename InputIt, typename = check_t< is_iterator<InputIt>> >
	CCDK_FORCEINLINE bitset(InputIt beginIt, size_type n) {
		allocate_copy(beginIt,n);
	}

	/* range copy */
	template<typename InputIt, typename = check_t< is_iterator<InputIt>>>
	CCDK_FORCEINLINE bitset(InputIt beginIt, InputIt endIt) {
		allocate_copy(beginIt, alg::distance(beginIt, endIt));
	}

	/* copy */
	CCDK_FORCEINLINE bitset(bitset const& other){
		local_allocate_n(other.len);
		memcpy(content, other.content, other.total_bytes());
	}

	/* template copy */
	template<typename Size2, typename T2, typename Alloc2>
	CCDK_FORCEINLINE bitset(bitset<T2, Size2, Alloc2> const& other) {
		local_allocate_n(other.len);
		clear_exbyte();
		memcpy(content, other.content,
			fn::min( used_bytes(), other.used_bytes()) );
	}

	/* move */
	CCDK_FORCEINLINE bitset(bitset && other)  noexcept
		: content{ other.content }, len{ other.size() },
		cap(other.capcity()) {
		other.rvalue_reset();
	}

	/* template move */
	template<typename Size2, typename Ratio2>
	CCDK_FORCEINLINE bitset(
		bitset<T,Size2, Ratio2, Alloc> && other)  noexcept
		: content{ other.content }, len{ other.size() },
		cap(other.capcity()) {
		other.rvalue_reset();
	}

	/* swap */
	CCDK_FORCEINLINE void swap(bitset& other) {
		util::swap(content, other.content);
		util::swap(len, other.len);
		util::swap(cap, other.cap);
	}

	/*  assign copy */
	CCDK_FORCEINLINE this_type& operator=(bitset const& other) {
		assign_copy_content(other.content, other.len);
		return *this;
	}

	/* template assign copy   */
	template<typename T2, typename Size2, typename Ratio2, typename Alloc2>
	CCDK_FORCEINLINE this_type& operator=(
		bitset<T2,Size2,Ratio2,Alloc2> const& other) {
		assign_copy_content(other.content, other.len);
		return *this;
	}

	/* move assign */
	CCDK_FORCEINLINE this_type& operator=(bitset && other) noexcept {
		this->deallocate();
		rvalue_set(other.content, other.len, other.cap);
		other.rvalue_reset();
	}

	/* move assign */
	template<typename Size2, typename Ratio2>
	CCDK_FORCEINLINE this_type& operator=(
		bitset<T,Size2,Ratio2,Alloc> && other) noexcept {
		this->deallocate();
		rvalue_set(other.content, other.len, other.cap);
		other.rvalue_reset();
	}

	/* assign fill */
	CCDK_FORCEINLINE this_type& operator=(bool v) noexcept {
		if (content && len > 0) {
			memset(content, v ? 0b11111111 : 0b00000000, used_bytes());
			if (v) set_ext_bits();
			return *this;
		}
	}

	/* fit size n and fill */
	CCDK_FORCEINLINE this_type& assign(size_type n, bool v) {
		if ( !content || n > cap) {
			this->deallocate();
			local_allocate_n(n);
		}
		else { len = n; }
		memset(content, v ? 0b11111111 : 0b00000000, used_bytes());
		if (v) set_ext_bits();
		return *this;
	}

	template<typename InputIt, typename = check_t< is_iterator<InputIt>> >
	CCDK_FORCEINLINE this_type& assign(InputIt beginIt, ptr::size_t n) {
		assign_copy_range(beginIt, n);
		return *this;
	}

	template<typename InputIt, typename = check_t< is_iterator<InputIt>> >
	CCDK_FORCEINLINE this_type& assign(InputIt beginIt, InputIt endIt) {
		assign_copy_range(beginIt, alg::distance(beginIt, endIt));
		return *this;
	}

	/* iterator */
	CCDK_FORCEINLINE iterator begin() noexcept {
		return { content, 0, 1, total_count() };
	}
	CCDK_FORCEINLINE iterator end() noexcept {
		return { content, last_pos(), last_mask(), total_count() };
	}
	CCDK_FORCEINLINE const_iterator cbegin() const noexcept {
		return { content, 0, 1, total_count() };
	}
	CCDK_FORCEINLINE const_iterator cend() const noexcept {
		return { content, last_pos(), last_mask(), total_count() };
	}
	CCDK_FORCEINLINE reverse_iterator rbegin() noexcept {
		return { --end() };
	}
	CCDK_FORCEINLINE reverse_iterator rend() noexcept {
		return { --begin() };
	}
	CCDK_FORCEINLINE const_reverse_iterator crbegin() const noexcept {
		return { cend()-1 };
	}
	CCDK_FORCEINLINE const_reverse_iterator crend() const noexcept {
		return { cbegin()-1 };
	}

	/* attribute */
	CCDK_FORCEINLINE size_type size() const noexcept { return len; }
	CCDK_FORCEINLINE size_type capcity() const noexcept { return cap; }
	CCDK_FORCEINLINE size_type max_size() 
		const noexcept { return size_type(-1) / sizeof(T); }

	/* index */
	CCDK_FORCEINLINE reference operator[](size_type index) noexcept {
		ccdk_assert(index < len);
		return { content[shr_type<value_type, size_type>(index)],
			cshl<value_type>(1, mod_type<value_type, value_type>(index)) };
	}

	/* const index, return bool */
	CCDK_FORCEINLINE const_reference operator[](size_type index) const noexcept {
		ccdk_assert(index < len);
		return content[shr_type<value_type, size_type>(index)] &
			cshl<value_type>(1, mod_type<value_type, value_type>(index));
	}

	/* quick index */
	CCDK_FORCEINLINE reference at(size_type index) noexcept {
		ccdk_assert(index < len);
		return { content[shr_type<value_type, size_type>(index)],
			cshl<value_type>(1, mod_type<value_type, value_type>(index)) };
	}

	/* quick const index */
	CCDK_FORCEINLINE const_reference at(size_type index) const noexcept {
		ccdk_assert(index < len);
		return content[shr_type<value_type, size_type>(index)] &
			cshl<value_type>(1, mod_type<value_type, value_type>(index));
	}

	/* last bit reference */
	CCDK_FORCEINLINE reference back()  noexcept {
		return { content[shr_type<value_type, size_type>(len-1)],
			cshl<value_type>(1, mod_type<value_type, value_type>(len-1)) };
	}

	/* last bit value */
	CCDK_FORCEINLINE const_reference back() const noexcept {
		return content[shr_type<value_type, size_type>(len - 1)] &
			cshl<value_type>(1, mod_type<value_type, value_type>(len - 1));
	}

	CCDK_FORCEINLINE this_type& pop_back() noexcept {
		if (len > 0) {
			back() = false;
			--len;
		}
		return *this;
	}

	CCDK_FORCEINLINE this_type& push_back(bool v) noexcept {
		if (len == cap) { local_reallocate_copy(len+1); }
		++len;
		back() = v;
		return *this;
	}

	CCDK_FORCEINLINE this_type& insert(size_type pos, bool v) noexcept {
		ccdk_assert(pos >= 0 && pos <= len);
		if (len == cap ) {
			this_type old{ util::move(*this) };
			local_allocate_n(old.size() + 1);
			len = old.size();
			clear_exbyte();
			if(len>0) memcpy(content, old.content, used_bytes());
		}
		++len;
		for (int i = len-2; i > pos; --i) {
			this->at(i + 1) = this->at(i);
		}
		this->at(pos) = v;
		return *this;
	}

	CCDK_FORCEINLINE this_type& erase(size_type pos) noexcept {
		return erase(pos, pos + 1);
	}
	
	CCDK_FORCEINLINE this_type& erase(iterator pos) noexcept {
		return erase(pos, pos + 1);
	}

	CCDK_FORCEINLINE this_type& erase(size_type ibegin, size_type iend) noexcept {
		ccdk_assert(ibegin >= 0 && iend > ibegin && iend <= len);
		size_type j = ibegin;
		for(size_type i = iend ; i < len; ++i, ++j)
			this->at(j) = this->at(i);
		size_type n = iend - ibegin;
		for (uint32 i = 1; i < n+1; ++i) {
			this->at(len - i) = false;
		}
		len -= n;
		return *this;
	}

	CCDK_FORCEINLINE this_type& erase(iterator beginIt, iterator endIt) noexcept {
		iterator it2 = beginIt;
		for (const_iterator it = endIt; it != end(); ++it, ++it2)
			*it2 = *it;
		size_type n = alg::distance(beginIt, endIt);
		it2 = end()-1;
		for (uint32 i = 0; i < n; ++i, --it2)
			*it2 = false;
		len -= n;
		return *this;
	}

	CCDK_FORCEINLINE this_type& clear() noexcept {
		return erase(begin(), end());
	}

	

private:
	CCDK_FORCEINLINE void rvalue_reset() noexcept {
		len = 0;
		cap = 0;
		content = nullptr;
	}

	CCDK_FORCEINLINE void rvalue_set(T* inContent,
		size_type n, size_type c) noexcept {
		content = inContent;
		len = n;
		cap = c;
	}

	CCDK_FORCEINLINE void deallocate() {
		if(content) allocator_type::deallocate(
			*this, content, total_count());
	}


	/* clear last used element's not used bits */
	CCDK_FORCEINLINE void set_ext_bits() noexcept {
		if (content && (len & 0x07)) {
			uint32 last_pos = used_bytes() - 1; 
			size_type ext_bits = len - (last_pos << 3);
			ccdk_assert(ext_bits >= 0 && ext_bits < 8);
			uint8& mask = *((uint8*)content+last_pos);
			mask = 0;
			for(uint32 i = 0;i<ext_bits;++i) mask |= cshl<uint8>(1, i);
		}
	}

	CCDK_FORCEINLINE void local_allocate_n(size_type n) {
		ccdk_increase_allocate3( (shr_type<T,size_type>(n+ kComplements)),
			content, cap, len);
		cap = shl_type<T, size_type>(cap);
		len = n;
	}

	CCDK_FORCEINLINE void allocate_fill(size_type n, bool val) {
		if (n > 0) {
			local_allocate_n(n);
			memset(content, val ? 0b11111111 : 0b00000000, used_bytes());
			if (val) set_ext_bits();
			if (cap - 8 >= len) {
				uint32 used = used_bytes();
				memset((uint8*)content + used, 0, total_bytes() - used);
			}
		}
	}

	CCDK_FORCEINLINE void local_reallocate_copy(size_type n) {
		if (n > 0) {
			this_type old{ util::move(*this) };
			local_allocate_n(n);
			len = old.size();
			clear_exbyte();
			memcpy(content, old.content, old.used_bytes());               /* copy content */
		}
	}

	CCDK_FORCEINLINE void clear_exbyte() noexcept {
		memset(reinterpret_cast<uint8*>(content) + used_bytes() - 1,
			0, total_bytes() - used_bytes() + 1);
	}

	template<typename InputIt>
	CCDK_FORCEINLINE void allocate_copy(InputIt beginIt, size_type n) {
		if (n > 0) {
			local_allocate_n(n);
			clear_exbyte();
			util::copy_n(begin(), beginIt, n);
		}
	}

	template<typename Char>
	CCDK_FORCEINLINE void allocate_init_from_string(Char const* str, ptr::size_t n) {
		if (!str || n == 0) return;
		local_allocate_n(n);
		clear_exbyte();
		for (ptr::size_t i = 0; i < n; ++i) {
			ccdk_assert(str[i] == Char('0') || str[i] == Char('1'));
			this->at(i) =  bool(str[i]-Char('0'));
		}
	}

	/* if n > cap reallocate content to fit, if not reset len to n and clear ex-byte */
	CCDK_FORCEINLINE void reallocate_or_set_len(size_type n) {
		if ( !content || n > cap) {
			this->deallocate();
			this->local_allocate_n(n);
		}
		else { len = n; }
		clear_exbyte();
	}

	/* memory copy content */
	template<typename T2>
	CCDK_FORCEINLINE void assign_copy_content(T2* memory, size_type n) {
		reallocate_or_set_len(n);
		memcpy(content, memory, used_bytes());               /* copy content */
	}

	/* iterator copy range */
	template<typename It>
	CCDK_FORCEINLINE void assign_copy_range(It beginIt, size_type n) {
		reallocate_or_set_len(n);
		util::copy_n(begin(), beginIt, n);
	}

///////////////////////////////////////////////////////////////////////////////
//// debug
public:
	/* quick index */
	CCDK_FORCEINLINE reference debug_at(size_type index) const noexcept {
		ccdk_assert(index < cap);
		return { content[shr_type<value_type, size_type>(index)],
			cshl<value_type>(1, mod_type<value_type, value_type>(index)) };
	}

	CCDK_FORCEINLINE void debug_all() const noexcept {
		char *info = new char[cap + 1]{ 0 };
		for (uint32 i = 0; i < cap; ++i) {
			info[i] = this->debug_at(i) ? '1' : '0';
		}

		DebugValue("fix_bit:", info);
		delete[] info;
	}

	CCDK_FORCEINLINE void debug() const noexcept {
		char *info = new char[len + 1]{ 0 };
		for (uint32 i = 0; i < len; ++i) {
			info[i] = this->at(i) ? '1' : '0';
		}
		DebugValue("fix_bit:", info);
		delete[] info;
	}

	CCDK_FORCEINLINE void debug_index() const noexcept {
		char *info = new char[len + 1]{ 0 };
		for (uint32 i = 0; i < len; ++i) {
			info[i] = (*this)[i] ? '1' : '0';
		}

		DebugValue("fix_bit:", info);
		delete[] info;
	}

	CCDK_FORCEINLINE void debug_it()  const noexcept {
		char *info = new char[len + 1]{ 0 };
		int j = 0;
		for (auto i = cbegin(); i!=cend(); ++i,++j) {
			info[j] = (*i) ? '1' : '0';
		}
		DebugValue("fix_bit:", info);
		delete[] info;
	}
	CCDK_FORCEINLINE void debug_rit()   noexcept {
		char *info = new char[len + 1]{ 0 };
		int j = 0;
		for (auto i = rbegin(); i != rend(); ++i, ++j) {
			info[j] = (*i) ? '1' : '0';
		}
		DebugValue("fix_bit:", info);
		delete[] info;
	}
};

ccdk_namespace_ct_end