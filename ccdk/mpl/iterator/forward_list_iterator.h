#pragma once

#include<ccdk/mpl/type_traits/has_attribute.h>
#include<ccdk/mpl/iterator/iterator_fwd.h>
#include<ccdk/mpl/util/move.h>

ccdk_namespace_mpl_it_start


template<typename T>
struct is_forward_node : has_attribute_next<T> {};

template<typename T>
constexpr is_forward_node<T> is_forward_node_c{};


template<typename Node>
struct iterator< forward_category, Node >
{
	using this_type       = iterator;
	using value_type      = typename Node::value_type;
	using node_type       = Node;
	using node_pointer    = Node * ;
	using pointer         = value_type * ;
	using const_pointer   = value_type const*;
	using reference       = value_type & ;
	using const_reference = value_type const&;
	using difference_type = ptr::diff_t;
	using size_type       = ptr::size_t;
	using category        = forward_category;

	static_assert(is_forward_node_c<Node>, "Node need has next field!");

	node_pointer content;

	/* ++it */
	CCDK_FORCEINLINE this_type& operator++() noexcept {
		content = content->next; return *this;
	}

	/* it++ */
	CCDK_FORCEINLINE constexpr this_type operator++(int) const noexcept { 
		return { content->next };
	}

	/* it+=step */
	CCDK_FORCEINLINE this_type& operator+=(size_type step) noexcept {
		for (ptr::size_t i = 0; i < step ; ++i, content = content->next)
			ccdk_assert(content);
		return *this;
	}

	/* it+step */
	CCDK_FORCEINLINE constexpr this_type operator+(size_type step) const noexcept {
		return this_type{ content } +=step;
	}

	/* const */
	CCDK_FORCEINLINE const_reference operator*() const noexcept { return content->data; }
	CCDK_FORCEINLINE reference operator*() noexcept { return content->data; }

	/* cmp */
	CCDK_FORCEINLINE bool operator==(this_type const& other) const noexcept {
		return content == content.content;
	}

	/* cmp */
	CCDK_FORCEINLINE bool operator!=(this_type const& other) const noexcept {
		return content != content.content;
	}
};
ccdk_namespace_mpl_it_end

