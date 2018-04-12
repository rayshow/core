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
		val = (val & ~ mask) | (bit? mask:0);
	}

	CCDK_FORCEINLINE void flip() noexcept { val = val ^ mask;	}

	CCDK_FORCEINLINE void set() noexcept { val = val | mask;   }

	CCDK_FORCEINLINE void reset() noexcept { val = val & ~mask; }

	CCDK_FORCEINLINE T masked_value() const noexcept { return val & mask; }

	CCDK_FORCEINLINE operator bool() { return val & mask; }

	CCDK_FORCEINLINE T* addressof() noexcept { return &val; }

	CCDK_FORCEINLINE T const * addressof() const noexcept { return &val; }
};



template<typename T,typename  Size>
struct iterator< bit_random_category, T, Size >
{
	using this_type       = iterator;
	using value_type      = bool;
	using pointer         = T*;
	using const_pointer   = T const*;
	using reference       = bit_access<T>;
	using const_reference = bool;
	using size_type       = Size;
	using difference_type = ptr::diff_t;
	using category        = random_category;
	static constexpr uint32 kStoreBits = sizeof(T)*8;
	static constexpr uint32 kShiftCount = kStoreBits -1;
	static constexpr T kTopMask = T(1) << kShiftCount;

	pointer           base;
	size_type         pos;
	remove_const_t<T> mask;
	size_type         max;

	CCDK_FORCEINLINE this_type& operator++() noexcept {
		T acc = cshr<T>( kTopMask & mask, kShiftCount);
		pos += acc;
		mask = cshl<T>(mask,1) | acc;
		return *this;
	}

	CCDK_FORCEINLINE this_type& operator--() noexcept {
		pos -= 1 & mask;
		mask = (mask >> T(1)) | cshl<T>(T(1) & mask, kShiftCount);
		return *this;
	}


	/* it++ */
	CCDK_FORCEINLINE constexpr this_type operator++(int) noexcept {
		this_type ret { base, pos, mask };
		this->operator++();
		return ret;
	}

	/* it-- */
	CCDK_FORCEINLINE constexpr this_type operator--(int) noexcept {
		this_type ret{ base, pos, mask };
		this->operator--();
		return ret;
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
		return this_type{ base, pos , mask, max }+=step;
	}

	/* it+step */
	CCDK_FORCEINLINE constexpr this_type operator-(difference_type step) const noexcept {
		return this_type{ base, pos , mask ,max }+=-step;
	}

	CCDK_FORCEINLINE constexpr difference_type operator-(this_type const& other) const noexcept {
		ccdk_assert(base == other.base);
		difference_type diff = csub<difference_type>(pos, other.pos) * kStoreBits;
		T cp_mask = mask;
		if (cp_mask < other.mask) while (cp_mask != other.mask) {
			cp_mask <<= T(1); --diff;
		}else if(cp_mask > other.mask) while(cp_mask!=other.mask){ 
			cp_mask >>= T(1); ++diff;
		}
		return diff;
	}

	/* const dereference */
	CCDK_FORCEINLINE const_reference operator*() const noexcept {
		ccdk_assert(pos < max);
		return base[pos] & mask;
	}

	/* dereference */
	CCDK_FORCEINLINE reference operator*() noexcept {
		ccdk_assert(pos < max);
		return { base[pos], mask };
	}

	/* const index */
	CCDK_FORCEINLINE const_reference operator[](difference_type index) const noexcept {
		return *(*this+index);
	}

	/* index */
	CCDK_FORCEINLINE reference operator[](difference_type index) noexcept {
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