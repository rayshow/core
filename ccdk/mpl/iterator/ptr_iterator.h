#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/iterator/iterator_fwd.h>

ccdk_namespace_mpl_it_start

template<typename T>
struct iterator<T*>
{
	using this_type = iterator;

	using value_type      = T;
	using pointer         = T*;
	using const_pointer   = T const*;
	using reference       = T&;
	using const_reference = T const&;
	using difference_type = ptr::diff_t;
	using size_type       = ptr::size_t ;
	using category        = random_category;

	T*    content;

	/* ++it */
	CCDK_FORCEINLINE this_type& operator++() noexcept { ++content; return *this; }
	/* --it */
	CCDK_FORCEINLINE this_type& operator--() noexcept { --content; return *this; }

	/* it++ */
	CCDK_FORCEINLINE constexpr this_type operator++(int) const noexcept { 
		return { content +1 }; 
	}

	/* it-- */
	CCDK_FORCEINLINE constexpr this_type operator--(int) const noexcept {
		return { content -1 };
	}

	/* it+step */
	CCDK_FORCEINLINE constexpr this_type operator+(size_type n) const noexcept {
		return this_type{ content + n }; 
	}

	/* it-step */
	CCDK_FORCEINLINE constexpr this_type operator-(size_type n) const noexcept {
		return this_type{ content - n };
	}

	/* it1 - it2 */
	CCDK_FORCEINLINE constexpr difference_type operator-(this_type const& other) {
		return content - other.content;
	}

	/* it+=step */
	CCDK_FORCEINLINE this_type& operator+=(size_type n) noexcept { 
		return content += n; return *this;
	}

	/* it+=step */
	CCDK_FORCEINLINE this_type& operator-=(size_type n) noexcept {
		return content -= n; return *this;
	}

	/* deref */
	CCDK_FORCEINLINE reference operator*()  noexcept { return *content; }
	CCDK_FORCEINLINE const_reference operator*() const noexcept { return *content; }

	/* index */
	CCDK_FORCEINLINE reference operator[](difference_type index) noexcept {
		return content[index];
	}
	CCDK_FORCEINLINE const_reference operator[](difference_type index) const noexcept {
		return content[index];
	}

	/* cmp */
	CCDK_FORCEINLINE bool operator==(this_type const& other) const noexcept {
		return content == other.content;
	}

	/* cmp */
	CCDK_FORCEINLINE bool operator!=(this_type const& other) const noexcept {
		return content != other.content;
	}
};


ccdk_namespace_mpl_it_end