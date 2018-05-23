#pragma once

#include<ccdk/mpl/iterator/iterator_fwd.h>
#include<ccdk/mpl/iterator/iterator_traits.h>

ccdk_namespace_mpl_it_start



template<typename It>
struct reverse_iterator 
{
	using this_type       = reverse_iterator;
	using traits_type     = iterator_traits<It>;
	using value_type      = typename traits_type::value_type;
	using pointer         = typename traits_type::pointer;
	using const_pointer   = typename traits_type::const_pointer;
	using reference       = typename traits_type::reference;
	using const_reference = typename traits_type::const_reference;
	using size_type       = typename traits_type::size_type;
	using difference_type = typename traits_type::difference_type;
	using category        = typename traits_type::category;

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


// test weather T is a reverse_iterator
template<typename T> struct is_reverse_iterator :false_ {};
template<typename It> struct is_reverse_iterator<reverse_iterator<It>> :true_ {};


template<typename It, typename RawIt = remove_const_t<It>>
CCDK_FORCEINLINE bool operator==(reverse_iterator<It> const& lhs, RawIt const& rhs) noexcept {
	return lhs.it == rhs;
}

template<typename It, typename RawIt = remove_const_t<It>>
CCDK_FORCEINLINE bool operator==(RawIt const& lhs, reverse_iterator<It> const& rhs) noexcept {
	return rhs.it == lhs;
}

ccdk_namespace_mpl_it_end