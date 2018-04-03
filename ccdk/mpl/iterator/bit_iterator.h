#pragma once

#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/type_traits/has_attribute.h>
#include<ccdk/mpl/type_traits/remove_const.h>
#include<ccdk/mpl/iterator/iterator_fwd.h>
#include<ccdk/mpl/function/operator.h>
#include<ccdk/mpl/util/move.h>

ccdk_namespace_mpl_it_start

struct bit_random_category {};

template<typename T>
struct bit_access {
	T&  val;
	T   mask;

	CCDK_FORCEINLINE void operator=(bit_access const& other) {
		val = (val & ~mask) | other.masked_value();
	}
	 
	CCDK_FORCEINLINE void operator=(bool bit) noexcept { 
		val = (val & ~ mask) | mask;
	}

	CCDK_FORCEINLINE void flip() noexcept { val = val ^ mask;	}

	CCDK_FORCEINLINE void set() noexcept { val = val & ~mask; }

	CCDK_FORCEINLINE void reset() noexcept { val = val | mask; }

	CCDK_FORCEINLINE T masked_value() const noexcept { return val & mask; }

	CCDK_FORCEINLINE operator bool() { return val & mask; }

	CCDK_FORCEINLINE T* addressof() noexcept { return &val; }

	CCDK_FORCEINLINE T const * addressof() const noexcept { return &val; }
};

template<uint32 Size> struct shift_bit;
template<> struct shift_bit<8> :mpl::uint32_<3> {};
template<> struct shift_bit<16> :mpl::uint32_<4> {};
template<> struct shift_bit<32> :mpl::uint32_<5> {};
template<> struct shift_bit<64> :mpl::uint32_<6> {};

template<typename T,typename  Size>
struct iterator< bit_random_category, T, Size >
{
	typedef iterator			 this_type;
	typedef bool				 value_type;
	typedef T*                   pointer_type;
	typedef T const*             const_pointer_type;
	typedef bit_access<T>        reference_type;
	typedef bool                 const_reference_type;
	typedef Size                 size_type;
	typedef ptr::diff_t          difference_type;
	typedef random_category		 category;
	static constexpr uint32 kStoreBits = sizeof(T)*8;
	static constexpr uint32 kShiftCount = kStoreBits -1;
	static constexpr T kTopMask = 1 << kShiftCount;

	pointer_type      base;
	size_type         pos;
	remove_const_t<T> mask;

	CCDK_FORCEINLINE this_type& operator++() noexcept {
		T acc = (kTopMask & mask) >> kShiftCount;
		pos += acc;
		mask = (mask << 1) | acc;
		return *this;
	}

	CCDK_FORCEINLINE this_type& operator--() noexcept {
		T dec = (1 & mask) << kShiftCount;
		pos -= 1 & mask;
		mask = (mask >> 1) | dec;
		return *this;
	}


	/* it++ */
	CCDK_FORCEINLINE constexpr this_type operator++(int) const noexcept {
		T acc = (kTopMask & mask) >> kShiftCount;
		return { base, pos + acc, (mask << 1) | acc };
	}

	/* it+=step */
	CCDK_FORCEINLINE this_type& operator+=(difference_type step) noexcept {
		if(step>0) while(step--) this->operator++();
		else if(step < 0) while(step++) this->operator--();
		return *this;
	}

	/* it-=step */
	CCDK_FORCEINLINE this_type& operator-=(difference_type step) noexcept {
		return this->operator+=(-step);
	}

	/* it+step */
	CCDK_FORCEINLINE constexpr this_type operator+(difference_type step) const noexcept {
		return this_type{ base, pos , mask }+=step;
	}

	/* it+step */
	CCDK_FORCEINLINE constexpr this_type operator-(difference_type step) const noexcept {
		return this_type{ base, pos , mask }+=-step;
	}

	CCDK_FORCEINLINE constexpr difference_type operator-(this_type const& other) const noexcept {
		ccdk_assert(base == other.base);
		difference_type diff = rsub<difference_type>(pos, other.pos) * kStoreBits;
		T cp_mask = mask;
		if (cp_mask < other.mask) while (cp_mask != other.mask) {
			cp_mask <<= 1; --diff;
		}else if(cp_mask > other.mask) while(cp_mask!=other.mask){ 
			cp_mask >>= 1; ++diff;
		}
		return diff;
	}

	/* const dereference */
	CCDK_FORCEINLINE const_reference_type operator*() const noexcept {
		ccdk_assert(offset < len);
		return base[pos] & mask;
	}

	/* dereference */
	CCDK_FORCEINLINE reference_type operator*() noexcept {
		return { base[pos], mask };
	}

	/* const index */
	CCDK_FORCEINLINE const_reference_type operator[](difference_type index) const noexcept {
		return *(*this+index);
	}

	/* index */
	CCDK_FORCEINLINE reference_type operator[](difference_type index) noexcept {
		return *(*this+index);
	}

	/* cmp */
	CCDK_FORCEINLINE bool operator==(this_type const& other) const noexcept {
		return base == other.base && mask == other.mask && pos == other.pos;
	}

	/* cmp */
	CCDK_FORCEINLINE bool operator!=(this_type const& other) const noexcept {
		return mask != other.mask || pos!=other.pos || base != other.base;
	}
};



ccdk_namespace_mpl_it_end