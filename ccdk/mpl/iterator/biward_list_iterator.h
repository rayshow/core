#pragma once

#include<ccdk/mpl/type_traits/has_attribute.h>
#include<ccdk/mpl/type_traits/remove_const.h>
#include<ccdk/mpl/iterator/iterator_fwd.h>
#include<ccdk/mpl/util/move.h>

ccdk_namespace_mpl_it_start

template<typename Node>
struct iterator< biward_category, Node >
{
	using this_type       = iterator;
	using non_const_node  = remove_const_t<Node>;
	using value_type      = typename non_const_node::value_type ;
	using node_type       = Node;
	using pointer         = value_type*;
	using const_pointer   = value_type const*;
	using reference       = value_type&;
	using const_reference = value_type const&;
	using difference_type = ptr::diff_t;
	using size_type       = ptr::size_t;
	using category        = biward_category;

	static_assert(has_attribute_next_v<non_const_node>, "Node need has next data field!");
	static_assert(has_attribute_prev_v<non_const_node>, "Node need has prev data field!");

	node_type* content;

	CCDK_FORCEINLINE pointer data() noexcept {
		return content;
	}

	CCDK_FORCEINLINE const_pointer data() const noexcept {
		return content;
	}

	/* ++it */
	CCDK_FORCEINLINE this_type& operator++() noexcept {
		content = content->next; return *this;
	}

	/* --it */
	CCDK_FORCEINLINE this_type& operator--() noexcept {
		content = content->prev; return *this;
	}

	/* it++ */
	CCDK_FORCEINLINE constexpr this_type operator++(int) const noexcept {
		return { content->next };
	}

	/* it-- */
	CCDK_FORCEINLINE constexpr this_type operator--(int) const noexcept {
		return { content->prev };
	}

	/* it+=step */
	CCDK_FORCEINLINE this_type& operator+=(size_type step) noexcept {
		while (step--) {
			content = content->next;
			ccdk_assert(content);
		}
		return *this;
	}
	/* it+step */
	CCDK_FORCEINLINE constexpr this_type operator+(size_type step) const noexcept {
		return this_type{ content } += step;
	}

	/* it-=step */
	CCDK_FORCEINLINE this_type& operator-=(size_type step) noexcept {
		while (step--) {
			content = content->prev;
			ccdk_assert(content);
		}
		return *this;
	}
	/* it-step */
	CCDK_FORCEINLINE constexpr this_type operator-(size_type step) const noexcept {
		return this_type{ content } -= step;
	}

	/* dereference */
	CCDK_FORCEINLINE const_reference operator*() const noexcept { return content->value; }
	CCDK_FORCEINLINE reference operator*() noexcept { return content->value; }

	/* member */
	CCDK_FORCEINLINE const_pointer operator->() const noexcept { return content; }
	CCDK_FORCEINLINE pointer operator->() noexcept { return content; }

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

