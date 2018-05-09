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
	using node_type        = Node;
	using link_type        = Node * ;
	using raw_node         = remove_const_t<Node>;
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

	//access content
	CCDK_FORCEINLINE node_type* data() noexcept {return content;}
	CCDK_FORCEINLINE node_type const* data() const noexcept {return content;}
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

	/* cast to const version */
	CCDK_FORCEINLINE operator iterator<biward_category, const Node>() const noexcept {
		return { content };
	}

	/* cmp */
	CCDK_FORCEINLINE bool operator==(this_type const& other) const noexcept {
		return content == other.content;
	}

	/* cmp */
	CCDK_FORCEINLINE bool operator!=(this_type const& other) const noexcept {
		return content != other.content;
	}
};
	template<typename Node>
	bool operator==(iterator<biward_category, const Node> const& lhs,
			iterator<biward_category, Node> const& rhs) noexcept {
		return lhs.data() == rhs.data();
	}
	template<typename Node>
	bool operator==(iterator<biward_category, Node> const& lhs,
		iterator<biward_category, const Node> const& rhs) noexcept {
		return lhs.data() == rhs.data();
	}

ccdk_namespace_mpl_it_end

