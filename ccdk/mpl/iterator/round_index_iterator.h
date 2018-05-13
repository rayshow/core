#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/base/identity.h>
#include<ccdk/mpl/base/if_.h>
#include<ccdk/mpl/type_traits/is_const.h>
#include<ccdk/mpl/type_traits/remove_const.h>
#include<ccdk/mpl/iterator/iterator_fwd.h>
#include<ccdk/mpl/util/equals.h>


ccdk_namespace_mpl_it_start

struct round_index_category {};

template<typename T, typename Size>
struct iterator<round_index_category,T,Size>
{
	using this_type       = iterator;
	using const_this_type = iterator<round_index_category, const T, Size>;
	using raw_value_type  = remove_const_t<T>;
	using value_type      = if_t< is_const<T>, T, raw_value_type >;
	using pointer         = T*;
	using const_pointer   = T const*;
	using reference       = T&;
	using const_reference = T const&;
	using difference_type = ptr::diff_t;
	using size_type       = Size;
	using category        = random_category;

	pointer      base;
	size_type    cap;    // actual cap - 1
	size_type    offset;

	/* ++it */
	CCDK_FORCEINLINE this_type& operator++() noexcept {
		offset = (offset + 1) & cap;
		return *this;
	}

	/* --it */
	CCDK_FORCEINLINE this_type& operator--() noexcept {
		offset = (offset + cap) & cap;
		return *this;
	}

	/* it++ */
	CCDK_FORCEINLINE constexpr this_type operator++(int) const noexcept {
		return { base, cap, (offset + 1) & cap };
	}

	/* it-- */
	CCDK_FORCEINLINE constexpr this_type operator--(int) const noexcept {
		return { base, cap, (offset + cap) & cap };
	}


	/* it+step */
	CCDK_FORCEINLINE constexpr this_type operator+(size_type step) const noexcept {
		return { base, cap, (offset + step) & cap };
	}

	/* it+=step */
	CCDK_FORCEINLINE this_type& operator+=(size_type step) noexcept {
		offset = (offset + step) & cap;
		return *this;
	}

	/* it-step */
	CCDK_FORCEINLINE constexpr this_type operator-(size_type step) const noexcept {
		ccdk_assert(step < cap);
		return { base, cap, (offset - step + cap+1) & cap };
	}

	/* it-=step */
	CCDK_FORCEINLINE this_type& operator-=(size_type step) noexcept {
		ccdk_assert(step <= cap);
		offset = (offset - step + cap+1) & cap;
		return *this;
	}

	/* deref */
	CCDK_FORCEINLINE reference operator*() noexcept {
		ccdk_assert(offset >= 0 && offset <= cap);
		return base[offset];
	}

	CCDK_FORCEINLINE const_reference operator*() const noexcept {
		ccdk_assert(offset >= 0 && offset <= cap);
		return base[offset];
	}

	/* index */
	CCDK_FORCEINLINE reference operator[](difference_type index) noexcept {
		ccdk_assert(index<=cap || index >= rneg<difference_type>(cap));
		return base[(offset + index + cap+1) & cap];
	}

	/* index */
	CCDK_FORCEINLINE const_reference operator[](difference_type index) const noexcept {
		ccdk_assert(index<=cap || index >= rneg<difference_type>(cap));
		return base[(offset + index + cap+1) & cap];
	}

	// to const_iterator
	CCDK_FORCEINLINE operator const_this_type() const noexcept { return { base, cap, offset }; }

	/* cmp */
	CCDK_FORCEINLINE bool operator==(this_type const& other) const noexcept {
		return base == other.base && offset == other.offset && cap == other.cap;
	}
};

// const_iterator == iterator
template<typename T, typename Size>
CCDK_FORCEINLINE bool operator==(iterator<round_index_category, const T, Size> const& left,
	iterator<round_index_category, T, Size> const& right) {
	return left.base == right.base && left.offset == right.offset && left.cap == right.cap;
}

// iterator == const_iterator
template<typename T, typename Size>
CCDK_FORCEINLINE bool operator==(iterator<round_index_category,T, Size> const& left,
	iterator<round_index_category, const T, Size> const& right) {
	return left.base == right.base && left.offset == right.offset && left.cap == right.cap;
}

ccdk_namespace_mpl_it_end