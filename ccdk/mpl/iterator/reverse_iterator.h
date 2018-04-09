#pragma once

#include<ccdk/mpl/iterator/iterator_fwd.h>

ccdk_namespace_mpl_it_start

template<typename It>
struct reverse_iterator 
{
	using this_type       = reverse_iterator;
	using value_type      = typename It::value_type;
	using pointer         = typename It::pointer;
	using const_pointer   = typename It::const_pointer;
	using reference       = typename It::reference;
	using const_reference = typename It::const_reference;
	using size_type       = typename It::size_type;
	using difference_type = typename It::difference_type;
	using category        = typename It::category;

	It it;

	CCDK_FORCEINLINE decltype(auto) data() noexcept { return it->data(); }
	CCDK_FORCEINLINE decltype(auto) data() const noexcept { return it->data(); }

	/* move */
	CCDK_FORCEINLINE this_type& operator++() noexcept { --it; return *this; }
	CCDK_FORCEINLINE this_type  operator++(int) const noexcept { return --this_type{ it }; }
	CCDK_FORCEINLINE this_type  operator+(size_type n) const noexcept { return this_type{ it - n }; }
	CCDK_FORCEINLINE this_type& operator+=(size_type n) noexcept { it -= n; return *this; }

	/* deref */
	CCDK_FORCEINLINE decltype(auto) operator*() const noexcept { return *it; }
	CCDK_FORCEINLINE decltype(auto) operator*() noexcept { return *it; }

	CCDK_FORCEINLINE bool operator==(this_type const& other) {
		return it == other.it;
	}

	CCDK_FORCEINLINE bool operator!=(this_type const& other) {
		return it != other.it;
	}
};


template<typename It>
CCDK_FORCEINLINE bool operator==(reverse_iterator<It> const& lhs, It const& rhs) noexcept {
	return lhs.it == rhs;
}

template<typename It>
CCDK_FORCEINLINE bool operator==(It const& lhs, reverse_iterator<It> const& rhs) noexcept {
	return rhs.it == lhs;
}

ccdk_namespace_mpl_it_end