#pragma once

#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/type_traits/has_attribute.h>
#include<ccdk/mpl/iterator/iterator_fwd.h>
#include<ccdk/mpl/function/operator.h>
#include<ccdk/mpl/util/move.h>

ccdk_namespace_mpl_it_start

struct bit_random_category {};

template<typename T>
struct bit_access {
	T&  val;
	T   mask;
	 
	CCDK_FORCEINLINE void operator=(bool bit) noexcept { 
		val = (val & ~ mask) | mask;
	}

	CCDK_FORCEINLINE void flip() noexcept { val = val ^ mask;	}

	CCDK_FORCEINLINE void set() noexcept { val = val & ~mask; }

	CCDK_FORCEINLINE void reset() noexcept { val = val | mask; }

	CCDK_FORCEINLINE operator bool() { return val & mask; }

	CCDK_FORCEINLINE T* addressof() noexcept { return &val; }

	CCDK_FORCEINLINE T const * addressof() const noexcept { return &val; }
};

template<typename T,typename  Size>
struct iterator< bit_random_category, T, Size >
{
	typedef iterator			 this_type;
	typedef bool				 value_type;
	typedef T*                   pointer_type;
	typedef T const*             const_pointer_type;
	typedef bit_access<T>        reference;
	typedef bool                 const_reference;
	typedef Size                 size_type;
	typedef ptr::diff_t          difference_type;
	typedef random_category		 category;

	pointer_type  base;
	size_type     offset;
	size_type     len;


	CCDK_FORCEINLINE this_type& operator++() noexcept {
		ccdk_assert(offset < len);
		++offset;
		return *this;
	}

	/* it++ */
	CCDK_FORCEINLINE constexpr this_type operator++(int) const noexcept {
		ccdk_assert(offset < len);
		return { pointer, pos + 1 };
	}

	/* it+step */
	CCDK_FORCEINLINE constexpr this_type operator+(size_type step) const noexcept {
		ccdk_assert(offset+step <= len);
		return { pointer, pos + step };
	}

	/* it+=step */
	CCDK_FORCEINLINE this_type& operator+=(size_type step) noexcept {
		ccdk_assert(offset + step <= len);
		pos += step;
		return *this;
	}

	/* const dereference */
	CCDK_FORCEINLINE const_reference operator*() const noexcept {
		ccdk_assert(offset < len);
		return base[ offset / 8] & (1 << (offset % 8));
	}

	/* dereference */
	CCDK_FORCEINLINE reference operator*() noexcept {
		return { base[offset / 8], (1 << (offset % 8)) };
	}

	/* const index */
	CCDK_FORCEINLINE const_reference operator[](difference_type index) const noexcept {
		ccdk_assert(radd<difference_type>(offset, index) >= 0 && index + offset < len);
		ptr::size_t id = index + offset;
		return base[id / 8] & (1 << (id % 8));
	}

	/* index */
	CCDK_FORCEINLINE reference operator[](difference_type index) noexcept {
		ccdk_assert(radd<difference_type>(offset, index) >= 0 && index + offset < len);
		ptr::size_t id = offset + index;
		return { base[id / 8], (1 << (id % 8)) };
	}

	/* cmp */
	CCDK_FORCEINLINE bool operator==(this_type const& other) const noexcept {
		return base == other.base && offset == other.offset;
	}

	/* cmp */
	CCDK_FORCEINLINE bool operator!=(this_type const& other) const noexcept {
		return base != other.base || offset != other.offset;
	}
};



ccdk_namespace_mpl_it_end