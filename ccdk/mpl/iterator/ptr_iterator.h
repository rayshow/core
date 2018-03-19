#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/iterator/iterator_fwd.h>

ccdk_namespace_mpl_it_start

template<typename T>
struct iterator<const T*>
{
public:
	typedef iterator         		 this_type;
	typedef const T                  value_type;
	typedef T const*                 pointer_type;
	typedef T const&                 reference_type;
	typedef T const&                 const_reference_type;
	typedef ptr::diff_t              difference_type;
	typedef ptr::size_t              size_type;
	typedef random_iterator_category category;

	T*    pointer;

	/* copy constructor */
	CCDK_FORCEINLINE constexpr iterator(this_type const& other) noexcept : pointer{ other.pointer } {}

	/* copy */
	CCDK_FORCEINLINE constexpr this_type& operator=(this_type const& other) noexcept { pointer = other.pointer; }

	/* ++it */
	CCDK_FORCEINLINE this_type& operator++() noexcept { ++pointer; return *this; }

	/* --it */
	CCDK_FORCEINLINE this_type& operator--() noexcept { --pointer; return *this; }

	/* it++ */
	CCDK_FORCEINLINE constexpr this_type operator++(int) const noexcept { return { pointer++ }; }

	/* it-- */
	CCDK_FORCEINLINE constexpr this_type operator--(int) const noexcept { return { pointer++ }; }

	/* it+step */
	CCDK_FORCEINLINE constexpr this_type operator+(size_type step) const noexcept { return this_type{ pointer + step }; }

	/* it-step */
	CCDK_FORCEINLINE constexpr this_type operator-(size_type step) const noexcept { return this_type{ pointer - step }; }

	/* it1 - it2 */
	CCDK_FORCEINLINE constexpr difference_type operator-(this_type const& other) { return pointer - other.pointer; }

	/* it+=step */
	CCDK_FORCEINLINE this_type& operator+=(size_type step) noexcept { return pointer += step; return *this; }

	/* it+=step */
	CCDK_FORCEINLINE this_type& operator-=(size_type step) noexcept { return pointer -= step; return *this; }

	/* const */
	CCDK_FORCEINLINE reference_type operator*() const noexcept { return *pointer; }

	/* index */
	CCDK_FORCEINLINE reference_type operator[](difference_type index) const noexcept { return pointer[index]; }

	/* cmp */
	CCDK_FORCEINLINE bool operator==(this_type const& other) const noexcept {
		return pointer == other.pointer;
	}

	/* cmp */
	CCDK_FORCEINLINE bool operator!=(this_type const& other) const noexcept {
		return pointer != other.pointer;
	}
};


template<typename T>
struct iterator<T*>
{
public:
	typedef iterator         		 this_type;
	typedef iterator<T const*>       const_this_type;
	typedef T						 value_type;
	typedef T *						 pointer_type;
	typedef T &						 reference_type;
	typedef T const&                 const_reference_type;
	typedef ptr::diff_t              difference_type;
	typedef ptr::size_t              size_type;
	typedef random_iterator_category category;

	T*    pointer;

	/* ++it */
	CCDK_FORCEINLINE this_type& operator++() noexcept { ++pointer; return *this; }

	/* --it */
	CCDK_FORCEINLINE this_type& operator--() noexcept { --pointer; return *this; }

	/* it++ */
	CCDK_FORCEINLINE constexpr this_type operator++(int) const noexcept { return { pointer++ }; }

	/* it-- */
	CCDK_FORCEINLINE constexpr this_type operator--(int) const noexcept { return { pointer++ }; }

	/* it+step */
	CCDK_FORCEINLINE constexpr this_type operator+(size_type step) const noexcept { return this_type{ pointer + step }; }

	/* it-step */
	CCDK_FORCEINLINE constexpr this_type operator-(size_type step) const noexcept { return this_type{ pointer - step }; }

	/* it1 - it2 */
	CCDK_FORCEINLINE constexpr difference_type operator-(this_type const& other) { return pointer - other.pointer; }

	/* it+=step */
	CCDK_FORCEINLINE this_type& operator+=(size_type step) noexcept { return pointer += step; return *this; }

	/* it+=step */
	CCDK_FORCEINLINE this_type& operator-=(size_type step) noexcept { return pointer -= step; return *this; }

	/* const */
	CCDK_FORCEINLINE reference_type operator*()  noexcept { return *pointer; }

	/* index */
	CCDK_FORCEINLINE reference_type operator[](difference_type index)  noexcept { return pointer[index]; }

	/* cmp */
	CCDK_FORCEINLINE bool operator==(this_type const& other) const noexcept {
		return pointer == other.pointer;
	}

	/* cmp */
	CCDK_FORCEINLINE bool operator!=(this_type const& other) const noexcept {
		return pointer != other.pointer;
	}
};

template<typename T>
struct reverse_iterator<const T*>
{
public:
	typedef reverse_iterator		 this_type;
	typedef const T                  value_type;
	typedef T const*                 pointer_type;
	typedef T const&                 reference_type;
	typedef T const&                 const_reference_type;
	typedef ptr::diff_t              difference_type;
	typedef ptr::size_t              size_type;
	typedef random_iterator_category category;

	 T*    pointer;

	

	/* ++it */
	CCDK_FORCEINLINE this_type& operator++() noexcept { --pointer; return *this; }

	/* --it */
	CCDK_FORCEINLINE this_type& operator--() noexcept { ++pointer; return *this; }

	/* it++ */
	CCDK_FORCEINLINE constexpr this_type operator++(int) const noexcept { return this_type{ pointer-- }; }

	/* it-- */
	CCDK_FORCEINLINE constexpr this_type operator--(int) const noexcept { return this_type{ pointer++ }; }

	/* it+step */
	CCDK_FORCEINLINE constexpr this_type operator+(size_type step) const noexcept { return this_type{ pointer - step }; }

	/* it-step */
	CCDK_FORCEINLINE constexpr this_type operator-(size_type step) const noexcept { return this_type{ pointer + step }; }

	/* it1 - it2 */
	CCDK_FORCEINLINE constexpr difference_type operator-(this_type const& other) { return pointer - other.pointer; }

	/* it+=step */
	CCDK_FORCEINLINE this_type& operator+=(size_type step) noexcept { return pointer -= step; return *this; }

	/* it+=step */
	CCDK_FORCEINLINE this_type& operator-=(size_type step) noexcept { return pointer += step; return *this; }

	/* const */
	CCDK_FORCEINLINE reference_type operator*() const noexcept { return *pointer; }

	/* index */
	CCDK_FORCEINLINE reference_type operator[](difference_type index) const noexcept { return pointer[index]; }

	/* cmp */
	CCDK_FORCEINLINE bool operator==(this_type const& other) const noexcept {
		return pointer == other.pointer;
	}

	/* cmp */
	CCDK_FORCEINLINE bool operator!=(this_type const& other) const noexcept {
		return pointer != other.pointer;
	}
};


template<typename T>
struct reverse_iterator<T*>
{
public:
	typedef reverse_iterator		 this_type;
	typedef const T                  value_type;
	typedef T *						 pointer_type;
	typedef T &					     reference_type;
	typedef T const&                 const_reference_type;
	typedef ptr::diff_t              difference_type;
	typedef ptr::size_t              size_type;
	typedef random_iterator_category category;

	T*    pointer;


	/* ++it */
	CCDK_FORCEINLINE this_type& operator++() noexcept { --pointer; return *this; }

	/* --it */
	CCDK_FORCEINLINE this_type& operator--() noexcept { ++pointer; return *this; }

	/* it++ */
	CCDK_FORCEINLINE constexpr this_type operator++(int) const noexcept { return this_type{ pointer-- }; }

	/* it-- */
	CCDK_FORCEINLINE constexpr this_type operator--(int) const noexcept { return this_type{ pointer++ }; }

	/* it+step */
	CCDK_FORCEINLINE constexpr this_type operator+(size_type step) const noexcept { return this_type{ pointer - step }; }

	/* it-step */
	CCDK_FORCEINLINE constexpr this_type operator-(size_type step) const noexcept { return this_type{ pointer + step }; }

	/* it1 - it2 */
	CCDK_FORCEINLINE constexpr difference_type operator-(this_type const& other) { return pointer - other.pointer; }

	/* it+=step */
	CCDK_FORCEINLINE this_type& operator+=(size_type step) noexcept { return pointer -= step; return *this; }

	/* it+=step */
	CCDK_FORCEINLINE this_type& operator-=(size_type step) noexcept { return pointer += step; return *this; }

	/* const */
	CCDK_FORCEINLINE reference_type operator*()  noexcept { return *pointer; }

	/* index */
	CCDK_FORCEINLINE reference_type operator[](difference_type index)  noexcept { return pointer[index]; }

	/* cmp */
	CCDK_FORCEINLINE bool operator==(this_type const& other) const noexcept {
		return pointer == other.pointer;
	}

	/* cmp */
	CCDK_FORCEINLINE bool operator!=(this_type const& other) const noexcept {
		return pointer != other.pointer;
	}
};


ccdk_namespace_mpl_it_end