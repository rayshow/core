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

template<typename T>
class slist
{
public:
	typedef slist                this_type;
	typedef slist                type;
	typedef T                    value_type;
	typedef T*                   pointer_type;
	typedef T const*             const_pointer_type;
	typedef T&                   reference_type;
	typedef T const&             const_reference_type;
	typedef slist_node<T>        node_type;
	typedef slist_node<T>*       node_pointer_type;
	typedef slist_node<T> const* const_node_pointer_type;
	typedef slist_node<T> &      node_reference_type;
	typedef slist_node<T> const& const_node_reference_type;
private:
	slist_node<T>* head;
	slist_node<T>* tail;
	ptr::size_t    length;
public:
	/*default */
	constexpr slist() = default;
	constexpr slist(ptr::nullptr_t) noexcept : head{ nullptr }, tail{ nullptr }, length{ 0 } {}

	/*value */
	constexpr slist()
};

ccdk_namespace_ct_end