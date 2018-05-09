#pragma once

#include<ccdk/mpl/base/if_.h>
#include<ccdk/mpl/base/identity.h>
#include<ccdk/mpl/base/logic_.h>
#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/type_traits/has_attribute.h>
#include<ccdk/mpl/type_traits/remove_const.h>
#include<ccdk/mpl/type_traits/is_const.h>
#include<ccdk/mpl/type_traits/add_const.h>
#include<ccdk/mpl/type_traits/remove_const.h>

#include<ccdk/mpl/iterator/iterator_fwd.h>
#include<ccdk/mpl/util/move.h>

ccdk_namespace_mpl_it_start


template<typename T>
struct is_biward_node : and_< has_attribute_next<T>, has_attribute_prev<T>> {};

template<typename T>
constexpr is_biward_node<T> is_biward_node_c{};

template<typename Node>
struct iterator< biward_category, Node >
{
	using this_type        = iterator;
	using const_this_type  = iterator< biward_category, const Node >;
	using node_type        = Node;
	using link_type        = Node * ;
	using raw_node         = remove_const_t<Node>;
	using non_const_this   = iterator< biward_category, raw_node >;
	using raw_value_type   = typename raw_node::value_type ;
	using value_type       = typename derive_if_t< is_const<Node>,
							add_const<raw_value_type>, identity<raw_value_type> >;
	using pointer          = value_type *;
	using const_pointer    = value_type const*;
	using reference        = value_type &;
	using const_reference  = value_type const&;
	using difference_type  = ptr::diff_t;
	using size_type        = ptr::size_t;
	using category         = biward_category;

	static_assert(is_biward_node<raw_node>::value, "Node need has next/front field!");

	link_type content;

	// access content
	CCDK_FORCEINLINE node_type* data() noexcept {return content;}
	CCDK_FORCEINLINE node_type const* data() const noexcept {return content;}

/////////////////////////////////////////////////////////////////////////////////////
//// inc / dec 

	CCDK_FORCEINLINE this_type& operator++() noexcept { ccdk_assert(content); content = content->next; return *this; }
	CCDK_FORCEINLINE this_type& operator--() noexcept { ccdk_assert(content); content = content->prev; return *this; }
	CCDK_FORCEINLINE constexpr this_type operator++(int) const noexcept { ccdk_assert(content); return { content->next }; }
	CCDK_FORCEINLINE constexpr this_type operator--(int) const noexcept { ccdk_assert(content); return { content->prev }; }

	// self add multi-step
	CCDK_FORCEINLINE this_type& operator+=(size_type step) noexcept { 
		while (step--) { 
			ccdk_assert(content); 
			content = content->next;
		} 
		return *this;
	}
	// add multi-step
	CCDK_FORCEINLINE this_type operator+(size_type step) const noexcept {
		ccdk_assert(content);
		return this_type{ content } += step;
	}

	// self sub multi-step
	CCDK_FORCEINLINE this_type& operator-=(size_type step) noexcept {
		while (step--) {
			ccdk_assert(content);
			content = content->prev;
		}
		return *this;
	}

	// sub multi-step
	CCDK_FORCEINLINE this_type operator-(size_type step) const noexcept {
		ccdk_assert(content);
		return this_type{ content } -= step;
	}

//////////////////////////////////////////////////////////////////////////
//// member / deference

	CCDK_FORCEINLINE const_reference operator*() const noexcept { ccdk_assert(content); return content->data; }
	CCDK_FORCEINLINE reference operator*() noexcept { ccdk_assert(content); return content->data; }
	CCDK_FORCEINLINE const_pointer operator->() const noexcept { ccdk_assert(content); return content; }
	CCDK_FORCEINLINE pointer operator->() noexcept { ccdk_assert(content); return content; }

	// const_iterator to iterator
	CCDK_FORCEINLINE operator const_this_type() const noexcept { return { content }; }

	// iterator == iterator
	CCDK_FORCEINLINE bool operator==(this_type const& other) const noexcept {
		return content == other.content;
	}

};

// const_iterator == iterator
template<typename Node>
CCDK_FORCEINLINE bool operator==(iterator<biward_category, const Node> const& lhs,
		iterator<biward_category, Node> const& rhs) noexcept {
	return lhs.content == rhs.content;
}

// iterator == const_iterator
template<typename Node>
CCDK_FORCEINLINE bool operator==(iterator<biward_category, Node> const& lhs,
	iterator<biward_category, const Node> const& rhs) noexcept {
	return lhs.content == rhs.content;
}

ccdk_namespace_mpl_it_end

