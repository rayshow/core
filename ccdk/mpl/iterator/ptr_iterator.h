#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/iterator/iterator_fwd.h>

ccdk_namespace_mpl_it_start

template<typename T>
struct reverse_iterator<const T*>
{
public:
	typedef reverse_iterator   this_type;
	typedef const T                  value_type;
	typedef T const*                 pointer_type;
	typedef T const&                 reference_type;
	typedef T const&                 const_reference_type;
	typedef ptr::diff_t              difference_type;
	typedef ptr::size_t              size_type;
	typedef random_iterator_category categroy;
private:
	const T*    pointer;
public:
	/* constructor */
	constexpr reverse_iterator(pointer_type p) noexcept : pointer{ p } {}

	/* copy constructor */
	constexpr reverse_iterator(this_type const& other) noexcept : pointer{ other.pointer } {}

	/* copy */
	constexpr this_type& operator=(this_type const& other) noexcept { pointer = other.pointer; }

	/* ++it */
	this_type& operator++() noexcept { --pointer; return *this; }

	/* --it */
	this_type& operator--() noexcept { ++pointer; return *this; }

	/* it++ */
	constexpr this_type operator++(int) const noexcept { return this_type{ pointer-- }; }

	/* it-- */
	constexpr this_type operator--(int) const noexcept { return this_type{ pointer++ }; }

	/* it+step */
	constexpr this_type operator+(size_type step) const noexcept { return this_type{ pointer - step }; }

	/* it-step */
	constexpr this_type operator-(size_type step) const noexcept { return this_type{ pointer + step }; }

	/* it1 - it2 */
	constexpr difference_type operator-(this_type const& other) { return pointer - other.pointer; }

	/* it+=step */
	this_type& operator+=(size_type step) noexcept { return pointer -= step; return *this; }

	/* it+=step */
	this_type& operator-=(size_type step) noexcept { return pointer += step; return *this; }

	/* const */
	reference_type operator*() const noexcept { return *pointer; }

	/* index */
	reference_type operator[](different_type index) const noexcept { return pointer[index]; }
};


template<typename T,typename Container>
struct reverse_iterator<T*,Container>: reverse_iterator<T*, Container>
{
public:
	typedef reverse_iterator<const T*, Container> base_type;
	typedef T                                     value_type;
	typedef T*                                    pointer_type;
	typedef T&                                    reference_type;

	using base_type::operator+;
	using base_type::operator-;
	using base_type::operator++;
	using base_type::operator--;
	using base_type::operator+=;
	using base_type::operator-=;
	using base_type::operator*;  /* const version */
	using base_type::operator[]; /* const version */

	 /* non-const version */
	reference_type operator[](different_type index) noexcept { return pointer[index]; }

	/* non-const version */
	reference_type operator*() noexcept { return *pointer; }
};


ccdk_namespace_mpl_it_end