#pragma once

#include<ccdk/mpl/iterator/iterator_fwd.h>

ccdk_namespace_mpl_it_start

template<typename It>
struct reverse_iterator 
{
	typedef reverse_iterator this_type;
	typedef reverse_iterator type;

	typedef typename It::value_type	          value_type;
	typedef typename It::pointer_type	      pointer_type;
	typedef typename It::reference_type	      reference_type;
	typedef typename It::const_reference_type const_reference_type;
	typedef typename It::size_type	          size_type;
	typedef typename It::difference_type      difference_type;
	typedef typename It::category	          category;

	It it;

	/* move */
	CCDK_FORCEINLINE this_type& operator++() noexcept { --it; return *this; }
	CCDK_FORCEINLINE this_type  operator++(int) const noexcept { return --this_type{ it }; }
	CCDK_FORCEINLINE this_type  operator+(size_type n) const noexcept { return this_type{ it - n }; }
	CCDK_FORCEINLINE this_type& operator+=(size_type n) noexcept { it -= n; return *this; }

	/* deref */
	CCDK_FORCEINLINE const_reference_type operator*() const noexcept { return *it; }
	CCDK_FORCEINLINE reference_type operator*() noexcept { return *it; }

	CCDK_FORCEINLINE bool operator==(this_type const& other) {
		return it == other.it;
	}

	CCDK_FORCEINLINE bool operator!=(this_type const& other) {
		return it == other.it;
	}
};


ccdk_namespace_mpl_it_end