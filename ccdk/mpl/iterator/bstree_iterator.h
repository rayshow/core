#pragma once

#include<ccdk/mpl/type_traits/has_attribute.h>
#include<ccdk/mpl/iterator/iterator_fwd.h>
#include<ccdk/mpl/util/move.h>

ccdk_namespace_mpl_it_start

struct bstree_tag;

/* binary search tree node */
template<typename T>
struct is_bstree_node : and_< has_attribute_left<T>,
	has_attribute_right<T>, has_attribute_parent<T>> {};

template<typename T>
constexpr is_bstree_node<T> is_bstree_node_c{};


template<typename Node>
struct iterator< bstree_tag, Node>
{
	using this_type       = iterator;
	using raw_node        = remove_const_t<Node>;
	using value_type      = typename raw_node::value_type;
	using node_type       = Node;
	using node_pointer    = Node *;
	using pointer         = value_type*;
	using const_pointer   = value_type const*;
	using reference       = value_type&;
	using const_reference = value_type const&;
	using difference_type = ptr::diff_t;
	using size_type       = ptr::size_t;
	using category        = biward_category;

	static_assert(is_bstree_node<Node>::value, "Node need binary search node!");

	node_pointer content;

	static node_pointer right_most(node_pointer node) {
		while (node->right) node = node->right;
		return node;
	}

	static node_pointer left_most(node_pointer node) {
		while (node->left) node = node->left;
		return left;
	}
	
	CCDK_FORCEINLINE void operator++() noexcept {
		ccdk_assert(content);
		if (content->right) {
			content = left_most(content->right);
		}
		else {
			content = content->parent;
		}
	}

	CCDK_FORCEINLINE void operator--() noexcept {
		ccdk_assert(content);
		if (content->left) {
			content = right_most(content->left);
		}
		else {
			content = content->parent;
		}
	}

	CCDK_FORCEINLINE this_type operator++(int) const noexcept {
		return ++this_type{ reinterpret_cast<node_pointer>(content) };
	}
	
	CCDK_FORCEINLINE this_type operator--(int) const noexcept {
		return --this_type{ reinterpret_cast<node_pointer>(content) };
	}

	CCDK_FORCEINLINE reference operator*() noexcept {
		return content->value;
	}

	CCDK_FORCEINLINE const_reference operator*() const noexcept {
		return content->value;
	}
};
ccdk_namespace_mpl_it_end

