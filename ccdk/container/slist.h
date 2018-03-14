#pragma once

#include<ccdk/container/container_mudule.h>
#include<ccdk/mpl/base/compile_check.h>

ccdk_namespace_ct_start

using namespace ccdk::mpl;

template<typename T>
struct slist_node
{
	T              data;
	slist_node<T>* next;
};

/* single list, only length and head */
template<typename T, typename Size = uint32>
class slist
{
public:
	typedef slist                this_type;
	typedef slist                type;

	/* common */
	typedef T                    value_type;
	typedef T*                   pointer_type;
	typedef T const*             const_pointer_type;
	typedef T&                   reference_type;
	typedef T const&             const_reference_type;
	typedef Size                 size_type;
	typedef ptr::diff_t          difference_type;

	/* node */
	typedef slist_node<T>        node_type;
	typedef slist_node<T>*       node_pointer_type;
	typedef slist_node<T> const* const_node_pointer_type;
	typedef slist_node<T> &      node_reference_type;
	typedef slist_node<T> const& const_node_reference_type;

private:
	node_pointer_type head;
	node_pointer_type tail;
	size_type         len;
public:
	/*default */
	constexpr slist() noexcept : head{ nullptr }, tail{ nullptr }, len{ 0 } {}
	constexpr slist(ptr::nullptr_t) noexcept : head{ nullptr }, tail{ nullptr }, len{ 0 } {}

	/*value */
	constexpr slist()
};

ccdk_namespace_ct_end