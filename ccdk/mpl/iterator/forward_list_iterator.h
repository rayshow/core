#pragma once

#include<ccdk/mpl/base/if_.h>
#include<ccdk/mpl/base/identity.h>
#include<ccdk/mpl/type_traits/is_const.h>
#include<ccdk/mpl/type_traits/add_const.h>
#include<ccdk/mpl/type_traits/has_attribute.h>
#include<ccdk/mpl/iterator/iterator_fwd.h>
#include<ccdk/mpl/util/move.h>
#include<ccdk/mpl/util/equals.h>

ccdk_namespace_mpl_it_start

template<typename T>
struct is_forward_node : has_attribute_next<T> {};

template<typename T>
constexpr is_forward_node<T> is_forward_node_c{};

template<typename Node>
struct iterator< forward_category, Node >
{
	using this_type       = iterator;
	using const_this_type = iterator< forward_category, const Node >;
	using raw_node        = remove_const_t<Node>;
	using raw_value_type  = typename raw_node::value_type;
	using value_type      = typename derive_if_t< is_const<Node>,
			                add_const<raw_value_type>, identity<raw_value_type> >;
	using node_type       = Node;
	using link_type       = Node * ;
	using pointer         = value_type * ;
	using const_pointer   = value_type const*;
	using reference       = value_type & ;
	using const_reference = value_type const&;
	using difference_type = ptr::diff_t;
	using size_type       = ptr::size_t;
	using category        = forward_category;

	static_assert(is_forward_node<raw_node>::value, "Node need has next field!");

	link_type content;

	/* ++it */
	CCDK_FORCEINLINE this_type& operator++() noexcept {
		ccdk_assert(content);
		content = content->next; return *this;
	}

	/* it++ */
	CCDK_FORCEINLINE constexpr this_type operator++(int) const noexcept { 
		ccdk_assert(content);
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

	//deference
	CCDK_FORCEINLINE const_reference operator*() const noexcept { return content->data; }
	CCDK_FORCEINLINE reference operator*() noexcept { return content->data; }

	//member
	CCDK_FORCEINLINE pointer operator->() noexcept { return &(content->data); }
	CCDK_FORCEINLINE const_pointer operator->() const noexcept { return &(content->data); }

	// iterator to const_iterator
	CCDK_FORCEINLINE operator const_this_type() const noexcept { ccdk_assert(content); return { content }; }


	// iterator == iterator
	CCDK_FORCEINLINE bool operator==(this_type const& other) const noexcept {
		return content == other.content;
	}
};

//  iterator == const_iterator
template<typename Node>
CCDK_FORCEINLINE bool operator==(iterator< forward_category, Node > const& left,
	iterator< forward_category, const Node > const& right) noexcept {
	return left.content == right.content;
}

// const_iterator == iterator
template<typename Node>
CCDK_FORCEINLINE bool operator==(iterator< forward_category, const Node > const& left,
	iterator< forward_category, Node > const& right) noexcept {
	return left.content == right.content;
}

ccdk_namespace_mpl_it_end

