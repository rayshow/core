#pragma once

#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/util/destruct.h>
#include<ccdk/mpl/util/construct.h>
#include<ccdk/mpl/util/swap.h>
#include<ccdk/mpl/util/move.h>
#include<ccdk/mpl/iterator/forward_list_iterator.h>
#include<ccdk/mpl/iterator/reverse_iterator.h>
#include<ccdk/mpl/iterator/algorithm/distance.h>
#include<ccdk/memory/simple_new_allocator.h>
#include<ccdk/memory/allocator_traits.h>
#include<ccdk/memory/list_allocate_adapter.h>
#include<ccdk/container/container_mudule.h>
#include<ccdk/container/impl/link_node.h>

ccdk_namespace_ct_start

using namespace mpl;

template<
	typename T,
	typename Size = uint32,
	typename Alloc = mem::simple_new_allocator<T>,
	typename Node = forward_node<T>
>
class slist : protected Alloc::template rebind<Node>
{
public:
	using this_type = slist;
	using node_type = Node;

	/* container type */
	using value_type      = T;
	using pointer         = T*;
	using const_pointer   = T const*;
	using reference       = T&;
	using const_reference = T const&;
	using size_type       = Size;
	using difference_type = ptr::diff_t ;
	using allocator_type  = mem::list_allocate_adapter< typename Alloc::template rebind<Node> > ;  /* list node block allcate */

	/* iterator */
	using iterator       = it::iterator< forward_category, node_type >;
	using const_iterator = it::iterator< forward_category, const node_type >;

	template<typename T2, typename Size2, typename Alloc2, typename Node2>
	friend class slist;

private:
	node_type* head;
	size_type  len;
public:

	/* destruct */
	CCDK_FORCEINLINE ~slist() {
		util::destruct_n(begin(), len);
		head = nullptr;
		len = 0;
	}

	/* default */
	CCDK_FORCEINLINE constexpr slist() 
		noexcept : head{ nullptr }, len{0}{}
	CCDK_FORCEINLINE constexpr slist(ptr::nullptr_t) 
		noexcept : head{ nullptr }, len{ 0 } {}


	/* fill */
	CCDK_FORCEINLINE slist(size_type n, T const& t = T()) {
		allocate_fill(n, t);
	}

	/* copy range-n */
	template<typename InputIt, typename = check_t< is_iterator<InputIt>>>
	CCDK_FORCEINLINE slist(InputIt beginIt, size_type n) {
		allocate_copy(beginIt, n);
	}

	/* copy range */
	template<typename InputIt, typename = check_t< is_iterator<InputIt>>>
	CCDK_FORCEINLINE slist(InputIt beginIt, InputIt endIt) {
		allocate_copy(beginIt, alg::distance(beginIt, endIt));
	}

	/* template copy */
	template<typename Size2, typename Alloc2, typename Node2>
	CCDK_FORCEINLINE slist(slist<T,Size2,Alloc2,Node2> const& other) {
		allocate_copy(other.begin(), other,len);
	}

	/* copy */
	CCDK_FORCEINLINE slist(slist const& other) {
		allocate_copy(other.begin(), other, len);
	}

	/* template move */
	template<typename Size2, typename Alloc2, typename Node2>
	CCDK_FORCEINLINE slist(slist<T, Size2, Alloc, Node> && other)
		noexcept : head{ other.head }, len{ other.len } {
		other.rvalue_reset();
	}

	/* move */
	CCDK_FORCEINLINE slist(slist && other)
		noexcept : head{ other.head }, len{ other.len } {
		other.rvalue_reset();
	}

	CCDK_FORCEINLINE void swap(slist & other) noexcept {
		util::swap(head, other.head);
		util::swap(len, other.len);
	}

	/* template copy assign  */
	template<typename Size2, typename Alloc2, typename Node2>
	CCDK_FORCEINLINE this_type& operator=(slist<T, Size2, Alloc2, Node2> const& other) {
		assign_copy(other.begin(), other.len);
	}

	/* copy assign */
	CCDK_FORCEINLINE this_type& operator=(slist const& other) {
		assign_copy(other.begin(), other.len);
	}

	/* template copy assign  */
	template<typename Size2, typename Alloc2, typename Node2>
	CCDK_FORCEINLINE this_type& operator=(slist<T, Size2, Alloc, Node> && other) {
		rvalue_set(other.head, other.len);
		other.rvalue_reset();
	}

	/* move assign */
	CCDK_FORCEINLINE this_type& operator=(slist && other) {
		rvalue_set(other.head, other.len);
		other.rvalue_reset();
	}

	/* range-n assign */
	CCDK_FORCEINLINE this_type& assign(size_type n, T const& t = T()) {
		assign_fill(n, t);
	}

	/* range-n assign */
	template<typename InputIt, typename = check_t< is_iterator<InputIt>>>
	CCDK_FORCEINLINE this_type& assign(InputIt beginIt, InputIt endIt) {
		assign_copy(beginIt, alg::distance(beginIt, endIt));
	}

	/* range-n assign */
	template<typename InputIt, typename = check_t< is_iterator<InputIt>>>
	CCDK_FORCEINLINE this_type& assign(InputIt beginIt, size_type n) {
		assign_copy(beginIt, n);
	}

	/* iterator */
	CCDK_FORCEINLINE iterator begin() noexcept { return { head };}
	CCDK_FORCEINLINE iterator end() noexcept { return { nullptr }; }

	CCDK_FORCEINLINE const_iterator cbegin() const noexcept { return{ head }; }
	CCDK_FORCEINLINE const_iterator cend() const noexcept { return{ nullptr }; }

	/* attribute */
	CCDK_FORCEINLINE size_type size() const noexcept { return len;}
	CCDK_FORCEINLINE size_type max_size() const noexcept { 
		return allocator_type::max_allocate_size();  
	}

	/* access */
	CCDK_FORCEINLINE reference front() noexcept { return *head; }
	CCDK_FORCEINLINE const_reference front() const noexcept { return *head; }
	
	/* operation */
	CCDK_FORCEINLINE this_type& pop_front() noexcept {
		util::destruct<T>(head);
		allocator_type::deallocate(*this, head, 1);
		head = head->next;
		--len;
		return *this;
	}

	/* remove head */
	CCDK_FORCEINLINE this_type& push_front(T const& t) noexcept {
		node_type* memory = allocator_type::allocate(*this, 1);
		util::construct<T>(memory, t);
		memory->next = head;
		head = memory;
		++len;
		return *this;
	}

	/* push to head */
	CCDK_FORCEINLINE this_type& push_front(T && t) noexcept {
		node_type* memory = allocator_type::allocate(*this, 1);
		util::construct<T>(memory, util::move(t));
		memory->next = head;
		head = memory;
		++len;
		return *this;
	}

private:

	CCDK_FORCEINLINE void rvalue_reset() noexcept { head = nullptr; len = 0; }
	CCDK_FORCEINLINE void rvalue_set(node_type* newHead, size_type newSize) noexcept {
		util::destruct_n(begin(), len);
		head = newHead; len = newSize;
	}

	CCDK_FORCEINLINE void allocate_fill(size_type n, T const& t) {
		if (n == 0) return;
		auto p = allocator_type::allocate(*this, n);
		util::construct_fill_n(iterator{ p.first }, t, n);
		len = n;
	}

	template<typename InputIt>
	CCDK_FORCEINLINE void allocate_copy(InputIt beginIt, size_type n) {
		if (n == 0) return;
		node_type* head = allocator_type::allocate(*this, n);
		util::construct_copy_n(iterator{ head }, beginIt, n);
		len = n;
	}

	template<typename InputIt>
	CCDK_FORCEINLINE void assign_copy(InputIt beginIt, size_type n) {
		if (n == 0) return;
		util::destruct_n(begin(), len);
		if (n > len) {
			auto last_its = util::construct_copy_n(begin(), beginIt, len - 1);
			ccdk_assert(last_its.first.pointer->next == nullptr);
			last_its.first.pointer->next = allocator_type::allocate(n - len);
			util::construct_copy_n(last_its.first, last_its.second, n - len + 1);
		} else {
			util::construct_copy_n(begin(), beginIt, n);
		}
		len = n;
	}

	CCDK_FORCEINLINE void assign_fill(size_type n, T const& t) {
		if (n == 0) return;
		util::destruct_n(begin(), len);
		if (n > len) {
			auto last_it = util::construct_fill_n(begin(), t, len-1);
			ccdk_assert(last_it.pointer != nullptr);
			last_it.pointer->next = allocator_type::allocate(n - len);
			util::construct_fill_n(last_it, t, n - len + 1);
		} else {
			util::construct_fill_n(begin(), beginIt, n);
		}
	}
};

ccdk_namespace_ct_end