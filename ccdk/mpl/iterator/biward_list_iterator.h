#pragma once

#include<ccdk/mpl/type_traits/has_attribute.h>
#include<ccdk/mpl/iterator/iterator_fwd.h>
#include<ccdk/mpl/util/move.h>

ccdk_namespace_mpl_it_start

template<typename Node>
struct iterator< biward_category, Node >
{
	typedef iterator                  this_type;
	typedef typename Node::value_type value_type;
	typedef Node                      node_type;
	typedef value_type*               pointer_type;
	typedef value_type const*         const_pointer_type;
	typedef value_type&               reference_type;
	typedef value_type const&         const_reference_type;
	typedef ptr::diff_t			      difference_type;
	typedef ptr::size_t			      size_type;
	typedef biward_category           category;

	static_assert(has_attribute_next_v<Node>, "Node need has next data field!");
	static_assert(has_attribute_prev_v<Node>, "Node need has prev data field!");

	node_type* pointer;

	/* ++it */
	CCDK_FORCEINLINE this_type& operator++() noexcept {
		pointer = pointer->next; return *this;
	}

	/* --it */
	CCDK_FORCEINLINE this_type& operator--() noexcept {
		pointer = pointer->prev; return *this;
	}

	/* it++ */
	CCDK_FORCEINLINE constexpr this_type operator++(int) const noexcept {
		return { pointer->next };
	}

	/* it-- */
	CCDK_FORCEINLINE constexpr this_type operator--(int) const noexcept {
		return { pointer->prev };
	}

	/* it+=step */
	CCDK_FORCEINLINE this_type& operator+=(size_type step) noexcept {
		while (step--) {
			pointer = pointer->next;
			ccdk_assert(pointer);
		}
		return *this;
	}
	/* it+step */
	CCDK_FORCEINLINE constexpr this_type operator+(size_type step) const noexcept {
		return this_type{ pointer } += step;
	}

	/* it-=step */
	CCDK_FORCEINLINE this_type& operator-=(size_type step) noexcept {
		while (step--) {
			pointer = pointer->prev;
			ccdk_assert(pointer);
		}
		return *this;
	}
	/* it-step */
	CCDK_FORCEINLINE constexpr this_type operator-(size_type step) const noexcept {
		return this_type{ pointer } -= step;
	}

	/* dereference */
	CCDK_FORCEINLINE const_reference_type operator*() const noexcept { return pointer->value; }
	CCDK_FORCEINLINE reference_type operator*() noexcept { return pointer->value; }

	/* member */
	CCDK_FORCEINLINE const_pointer_type operator->() const noexcept { return pointer; }
	CCDK_FORCEINLINE pointer_type operator->() noexcept { return pointer; }

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

