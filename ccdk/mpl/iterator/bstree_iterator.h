#pragma once

#include<ccdk/mpl/type_traits/has_attribute.h>
#include<ccdk/mpl/iterator/iterator_fwd.h>
#include<ccdk/mpl/util/move.h>

ccdk_namespace_mpl_it_start

struct bstree_category;

/* binary search tree node */
template<typename T>
struct is_bstree_node : and_< has_attribute_left<T>, has_attribute_right<T>, has_attribute_parent<T>> {};

template<typename T>
constexpr is_bstree_node<T> is_bstree_node_c{};


template<typename Node>
struct iterator< bstree_category, Node>
{
	using this_type       = iterator;
	using value_type      = typename Node::value_type;
	using node_type       = Node;
	using node_pointer    = Node *;
	using pointer         = value_type*;
	using const_pointer   = value_type const*;
	using reference       = value_type&;
	using const_reference = value_type const&;
	using difference_type = ptr::diff_t;
	using size_type       = ptr::size_t;
	using category        = forward_category;

	static_assert(is_bstree_node<Node>::value, "Node need has next data field!");

	node_pointer content;

	static node_pointer right_most(node_pointer node) {
		while (node->right) node = node->right;
		return node;
	}
	
	CCDK_FORCEINLINE void operator++() noexcept {
		ccdk_assert(content);
		if (content->right) {
			content = right_most(content->right);
		}
		else {
			ccdk_assert(content->parent);
			while (content == content->parent->right) {
				content = content->parent;
			}
		}
	}
	

};
ccdk_namespace_mpl_it_end

