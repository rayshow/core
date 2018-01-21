#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/iterator/iterator_fwd.h>

ccdk_namespace_mpl_it_start

template<typename T, typename Container>
class const_iterator<T*, Container>
{
public:
	typedef Container                                 container_type;
	typedef const_iterator                            this_type;
	typedef const_iterator &                          this_reference_type;
	typedef const_iterator const&                     this_const_reference_type;
	typedef typename Container::size_type             size_type;
	typedef typename Container::diff_type             different_type;
	typedef typename Container::value_type            value_type;
	typedef typename Container::pointer_type          pointer_type;
	typedef typename Container::reference_type        reference_type;
	typedef typename Container::const_reference_type  const_reference_type;
private:
	T* pointer;
public:

	/* constructor */
	const_iterator(T const* inPointer) : pointer{ inPointer }{}
	/* copy */
	this_reference_type operator=(this_type const& other) noexcept { pointer = other.pointer;}

	/* ++it */
	this_reference_type operator++() noexcept { ++pointer; return *this; }

	/* --it */
	this_reference_type operator--() noexcept { --pointer; return *this; }

	/* it++ */
	this_type operator++(int) const noexcept { return this_type{ pointer++ }; }

	/* it-- */
	this_type operator--(int) const noexcept { return this_type{ pointer-- }; }

	/* it+step */
	this_type operator+(size_type step) const noexcept { return this_type{ pointer + step }; }

	/* it-step */
	this_type operator-(size_type step) const noexcept { return this_type{ pointer - step }; }

	/* it1 - it2 */
	different_type operator-(this_type const& other) { return pointer - other.pointer; }

	/* it+=step */
	this_reference_type operator+=(size_type step) noexcept { return pointer += step; return *this; }

	/* it+=step */
	this_reference_type operator-=(size_type step) noexcept { return pointer -= step; return *this; }

	/* const */
	value_type const&  operator*() const noexcept { return *pointer; }

	/* index */
	value_type const& operator[](different_type index) const noexcept { pointer + index; return pointer[index]; }
};


template<typename T,typename Container>
struct iterator<T*,Container> : const_iterator<T*, Container>
{
public:
	typedef const_iterator<T, Container> base_type;
	using base_type::operator+;
	using base_type::operator-;
	using base_type::operator++;
	using base_type::operator--;
	using base_type::operator+=;
	using base_type::operator-=;
	using base_type::operator*;  /* const version */
	using base_type::operator[]; /* const version */

								 /* non-const version */
	value_type& operator[](different_type index) noexcept { return pointer[index]; }

	/* non-const version */
	value_type& operator*() noexcept { return *pointer; }
};

template<typename T, typename Container>
struct const_reverse_iterator<T*,Container>
{
public:
	typedef Container                                         container_type;
	typedef const_reverse_iterator                            this_type;
	typedef const_reverse_iterator &                          this_reference_type;
	typedef const_reverse_iterator const&                     this_const_reference_type;
	typedef typename Container::size_type                     size_type;
	typedef typename Container::diff_type                     different_type;
	typedef typename Container::value_type                    value_type;
	typedef typename Container::pointer_type                  pointer_type;
	typedef typename Container::reference_type                reference_type;
	typedef typename Container::const_reference_type          const_reference_type;
private:
	pointer_type    pointer;
public:
	/* constructor */
	const_reverse_iterator(pointer_type const* inPointer) noexcept : pointer{ inPointer } {}

	/* copy */
	this_reference_type operator=(this_type const& other) noexcept { pointer = other.pointer; }

	/* ++it */
	this_reference_type operator++() noexcept { --pointer; return *this; }

	/* --it */
	this_reference_type operator--() noexcept { ++pointer; return *this; }

	/* it++ */
	this_type operator++(int) const noexcept { return this_type{ pointer-- }; }

	/* it-- */
	this_type operator--(int) const noexcept { return this_type{ pointer++ }; }

	/* it+step */
	this_type operator+(size_type step) const noexcept { return this_type{ pointer - step }; }

	/* it-step */
	this_type operator-(size_type step) const noexcept { return this_type{ pointer + step }; }

	/* it1 - it2 */
	different_type operator-(this_type const& other) { return pointer - other.pointer; }

	/* it+=step */
	this_reference_type operator+=(size_type step) noexcept { return pointer -= step; return *this; }

	/* it+=step */
	this_reference_type operator-=(size_type step) noexcept { return pointer += step; return *this; }

	/* const */
	value_type const&  operator*() const noexcept { return *pointer; }

	/* index */
	value_type const& operator[](different_type index) const noexcept { return pointer[index]; }
};


template<typename T,typename Container>
struct reverse_iterator<T*,Container>
	: const_reverse_iterator<T*, Container>
{
public:
	typedef const_reverse_iterator<T*, Container> base_type;
	using base_type::operator+;
	using base_type::operator-;
	using base_type::operator++;
	using base_type::operator--;
	using base_type::operator+=;
	using base_type::operator-=;
	using base_type::operator*;  /* const version */
	using base_type::operator[]; /* const version */

								 /* non-const version */
	value_type& operator[](different_type index) noexcept { return pointer[index]; }

	/* non-const version */
	value_type& operator*() noexcept { return *pointer; }
};


ccdk_namespace_mpl_it_end