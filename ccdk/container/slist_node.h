#pragma once

#include<ccdk/container/container_mudule.h>
#include<ccdk/mpl/iterator/iterator_fwd.h>
#include<ccdk/mpl/util/move.h>

ccdk_namespace_ct_start

using namespace ccdk::mpl;

template<typename T>
struct slist_node
{
	T              data;
	slist_node<T>* next;

	explicit slist_node(T const& t) :data{ t }, next{ nullptr } {}

	explicit slist_node(T&& t) :data{ util::move(t) }, next{ nullptr } {}
};
ccdk_namespace_ct_end

ccdk_namespace_mpl_it_start

template< typename T>
struct iterator< const ccdk::ct::slist_node<T> >
{
	typedef ccdk::ct::slist_node<T> node_type;
};

template< typename T>
struct iterator< ccdk::ct::slist_node<T> > 
{
	typedef ccdk::ct::slist_node<T>      node_type;
	typedef iterator					 this_type;
	typedef T							 value_type;
	typedef T*							 pointer_type;
	typedef T&							 reference_type;
	typedef T const&					 const_reference_type;
	typedef ptr::diff_t					 difference_type;
	typedef ptr::size_t					 size_type;
	typedef forward_category			 category;

	node_type* pointer;

	CCDK_FORCEINLINE  constexpr iterator(node_type* pointer) :pointer{ pointer } {}

	/* copy constructor */
	CCDK_FORCEINLINE  constexpr iterator(this_type const& other) noexcept : pointer{ other.pointer } {}

	/* ++it */
	CCDK_FORCEINLINE this_type& operator++() noexcept {
		pointer = pointer->next; return *this; 
	}

	/* it++ */
	CCDK_FORCEINLINE constexpr this_type operator++(int) const noexcept { 
		return this_type{ pointer->next }; 
	}

	/* it+step */
	CCDK_FORCEINLINE constexpr this_type operator+(size_type step) const noexcept {
		return this_type{ pointer } +=step; 
	}

	/* const */
	CCDK_FORCEINLINE reference_type operator*() const noexcept { return pointer->data; }

	/* index */
	CCDK_FORCEINLINE reference_type operator[](size_type index) const noexcept {
		return *(this_type{ pointer } += step);
	}

	/* it+=step */
	CCDK_FORCEINLINE this_type& operator+=(size_type step) noexcept {
		for (ptr::size_t i = 0; i < step && pointer; ++i, pointer = pointer->next);
		return *this;
	}

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

