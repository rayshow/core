#pragma once

#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/util/move.h>
#include<ccdk/mpl/util/fill.h>
#include<ccdk/mpl/util/copy.h>
#include<ccdk/mpl/util/construct.h>
#include<ccdk/mpl/util/swap.h>
#include<ccdk/mpl/fusion/pair.h>
#include<ccdk/mpl/iterator/biward_list_iterator.h>

#include<ccdk/memory/allocator_traits.h>
#include<ccdk/memory/simple_new_allocator.h>
#include<ccdk/memory/list_allocate_adapter.h>

#include<ccdk/mpl/iterator/biward_list_iterator.h>
#include<ccdk/mpl/iterator/reverse_iterator.h>

#include<ccdk/algorithm/advance.h>
#include<ccdk/algorithm/distance.h>
#include<ccdk/container/impl/list_node.h>
#include<ccdk/container/container_mudule.h>

ccdk_namespace_ct_start

using namespace ccdk::mpl;

template<
	typename T,
	typename Size = uint32,
	typename Alloc = mem::simple_new_allocator< T >,
	typename Node = biward_node<T>
>
class blist : protected Alloc::template rebind<Node>
{
public:
	/* common */
	typedef blist                    this_type;
	typedef Node                     node_type;
	
	/* container */
	typedef T           value_type;
	typedef T*          pointer_type;
	typedef T const*    const_pointer_type;
	typedef T&          reference_type;
	typedef T const&    const_reference_type;
	typedef Size        size_type;
	typedef ptr::diff_t difference_type;

	/* allocate/delete multi-list-node */
	typedef mem::list_allocate_adapter< 
		typename Alloc::template rebind<Node>>  allocator_type;

	/* iterator */
	typedef it::iterator<biward_category, Node> iterator_type;
	typedef const iterator_type                 const_iterator_type;
	typedef it::reverse_iterator<iterator_type> reverse_iterator_type;
	typedef const reverse_iterator              const_reverse_iterator_type;

	template<typename T2, typename Size2, typename Alloc2, typename Node2>
	friend class blist;

private:
	node_type* head;
	size_type  len;

public:

	// destruct 
	CCDK_FORCEINLINE ~blist() noexcept {
		destruct_content();
		ccdk_assert(allocator_type::deallocate(
			*this, head, len) == nullptr);
		len = 0;
	}

	// default and nullptr ctor
	CCDK_FORCEINLINE constexpr blist():head{nullptr},len{0}{}
	CCDK_FORCEINLINE constexpr blist(ptr::nullptr_t) : head{ nullptr }, len{ 0 } {}


	// fill ctor
	CCDK_FORCEINLINE explicit blist(size_type n, T const& t = T()) {
		allocate_fill(n, t);
	}

	//  range-n ctor
	template<typename InputIt, typename = check_t< is_iterator<InputIt>>>
	CCDK_FORCEINLINE explicit blist(InputIt beginIt, size_type n) {
		allocate_copy(beginIt, n);
	}

	//  range ctor
	template<typename InputIt, typename = check_t< is_iterator<InputIt>>>
	CCDK_FORCEINLINE explicit blist(InputIt beginIt, InputIt endIt) {
		allocate_copy(beginIt, alg::distance(beginIt, endIt));
	}

	// copy ctor
	CCDK_FORCEINLINE explicit blist(blist const& other) {
		allocate_copy(other.begin(), other.size());
	}

	// template copy ctor, node_type can  be different but T must be same
	template<typename Size2, typename Node2>
	CCDK_FORCEINLINE explicit blist(blist<T,Size2,Alloc,Node2> const& other) {
		allocate_copy(other.begin(), other.size());
	}

	// move ctor
	CCDK_FORCEINLINE explicit blist(blist && other) noexcept
		:head{ other.head }, len{ other.len } {
		other.rvalue_reset();
	}

	// template move, node_type need be same
	template<typename Size2>
	CCDK_FORCEINLINE explicit blist(blist<T, Size2, Alloc, Node> && other) noexcept
		: head{ other.head }, len{ other.len } {
		other.rvalue_reset();
	}

	// swap data
	CCDK_FORCEINLINE void swap(blist& other) noexcept {
		util::swap(head, other.head);
		util::swap(len, other.len);
	}

	// copy assign 
	CCDK_FORCEINLINE this_type& operator=(blist const& other) {
		return copy_range(other.begin(), other.size());
	}

	// move assign 
	CCDK_FORCEINLINE this_type& operator=(blist && other) {
		this->~blist();
		this->rvalue_set(other.head, other.len);
		other.rvalue_reset();
		return *this;
	}

	// template copy assign 
	template<typename Size2, typename Node2>
	CCDK_FORCEINLINE this_type& operator=(blist<T, Size2, Alloc, Node2> const& other) {
		return copy_range(other.begin(), other.size());
	}

	// template move assign 
	template<typename Size2>
	CCDK_FORCEINLINE this_type& operator=(blist<T, Size2, Alloc, Node> && other) {
		this->~blist();
		this->rvalue_set(other.head, other.len);
		other.rvalue_reset();
		return *this;
	}

	// fill assign 
	CCDK_FORCEINLINE this_type& assign(size_type n, T const& t = T()) {
		return fill_range(t, n); 
	}

	// range-n assign 
	template<typename InputIt, typename = check_t< is_iterator<InputIt>>>
	CCDK_FORCEINLINE this_type& assign(InputIt beginIt, size_type n) {
		return copy_range(beginIt, n);
	}

	// range assign 
	template<typename InputIt, typename = check_t< is_iterator<InputIt>>>
	CCDK_FORCEINLINE this_type& assign(InputIt beginIt, InputIt endIt) {
		return copy_range(beginIt, alg::distance(beginIt,endIt) );
	}


	/* attribute */
	CCDK_FORCEINLINE size_type size() const noexcept { return len;  }
	CCDK_FORCEINLINE size_type max_size() const noexcept { 
		return allocate_type::max_allocate_size();
	}

	/* iterator */
	CCDK_FORCEINLINE iterator_type begin() noexcept { return { head }; }
	CCDK_FORCEINLINE iterator_type end() noexcept { return { head }; }
	CCDK_FORCEINLINE const_iterator_type cbegin() const noexcept { return { head }; }
	CCDK_FORCEINLINE const_iterator_type cend() const noexcept { return { head }; }

	// pop back
	CCDK_FORCEINLINE this_type& pop_back() noexcept {
		if (len > 1) {
			node_type* back = head->prev;
			back->prev->next = head;
			head->prev = back->prev;
			destroy_node(back);
		} else if (len == 1) {
			destroy_node(head);
			head = nullptr;
		}
		--len;
	}

	CCDK_FORCEINLINE void insert_after(node_type* pos, node_type* node) {
		node->prev = pos;
		node->next = pos->next;
		pos->next->prev = node;
		pos->next = node;
	}

	// emplace back elements 
	template<typename T, typename... Args>
	CCDK_FORCEINLINE this_type& emplace_back(T&& t, Args&&... args) {
		node_type* node = new_node(util::forward<T>(t), util::forward<Args>(args)...);
		if (len == 0) { set_first_node(node); } 
		else { insert_after(head->prev, node); }
		++len;
	}

	CCDK_FORCEINLINE this_type& push_back(T const& t) {
		return emplace_back(t);
	}

	CCDK_FORCEINLINE this_type& push_back(T && t) {
		return emplace_back(util::move(t) );
	}

	// pop back
	CCDK_FORCEINLINE this_type& pop_front() noexcept {
		if (len > 1) {
			node_type* next = head->next;
			head->prev->next = next;
			next->prev = head->prev;
			head = next;
			destroy_node(head);
		}
		else if (len == 1) {
			destroy_node(head);
			head = nullptr;
		}
		--len;
	}


	// emplace back elements 
	template<typename T, typename... Args>
	CCDK_FORCEINLINE this_type& emplace_front(T&& t, Args&&... args) {
		return emplace_back(util::forward<T>(t), util::forward<Args>(args)...);
	}

	CCDK_FORCEINLINE this_type& push_back(T const& t) {
		return emplace_back(t);
	}

	CCDK_FORCEINLINE this_type& push_back(T && t) {
		return emplace_back(util::move(t));
	}

	template<typename T, typename... Args>
	CCDK_FORCEINLINE this_type& emplace(const_iterator_type pos, T&& t, Args&&... args) {
		node_type* node = new_node(util::forward<T>(t), util::forward<Args>(args)...);
		if (len == 0) { set_first_node(node); }
		else { insert_after(pos.pointer->prev, node); }
		++len;
	}

	CCDK_FORCEINLINE this_type& insert(const_iterator_type pos,T const& t) {
		return emplace(pos, t);
	}

	CCDK_FORCEINLINE this_type& insert(const_iterator_type pos, T && t) {
		return emplace(pos, util::move(t));
	}

	CCDK_FORCEINLINE this_type& erase(const_iterator_type pos) {
		return erase(pos, ++pos);
	}

	CCDK_FORCEINLINE this_type& erase(const_iterator_type beginIt,
		const_iterator_type endIt) {
		node_type* beginPtr = beginIt.pointer;
		node_type* endPtr = endIt.pointer;
		size_type n = alg::distance(beginIt, endIt);
		ccdk_assert(n <= len);
		if (n == len) { this->~blist(); }
		else {
			node_type* before = beginPtr->prev;
			before->next = endPtr;
			endPtr->prev = before;
			util::destruct_range<T>(beginIt, endIt);
			allocator_type::deallocate(*this, beginPtr, n);
		}
		len -= n;
	}


private:
	// reset r-value
	CCDK_FORCEINLINE void rvalue_reset() noexcept {
		head = nullptr;
		len = 0;
	}

	// set to another list
	CCDK_FORCEINLINE void rvalue_set(node_type* other_head, size_type n) noexcept {
		head = other_head;
		len = n;
	}

	// destruct objects in list
	CCDK_FORCEINLINE void destruct_content() noexcept {
		if (len > 0) util::destruct_n(begin(), len);
	}

	// destruct and deallocate
	CCDK_FORCEINLINE void destroy_node(node_type* node) {
		util::destruct<T>(node);
		allocator_type::deallocate(*this, node, 1);
	}

	// construct one node
	template<typename T, typename... Args>
	CCDK_FORCEINLINE node_type* new_node(T&& t, Args&&... args) {
		node_type* node = allocate_type::allocate(*this, 1);
		util::construct<T>(node, util::forward<T>(t), util::forward<Args>(args)...);
		return node;
	}

	// 
	CCDK_FORCEINLINE void set_first_node(node_type* node) {
		head = node;
		node->next = node;
		node->prev = node;
	}

	CCDK_FORCEINLINE node_type* allocate_n(size_type n) {
		node_type *new_head, *new_tail;
		fs::tie(new_head, new_tail) = allocator_type::allocate(*this, n);
		new_tail->next = new_head;
		new_head->prev = new_tail;
		return new_head;
	}

	// allocate n elements and fill with t
	CCDK_FORCEINLINE void allocate_fill(size_type n, T const& t) {
		if (n == 0) return;
		content = allocate_n(n);
		ccdk_safe_cleanup_if_exception(
			util::construct_fill_n(iterator_type{ content }, t, n),//may throw if copy-ctor 
			allocator_type::deallocate(*this, content, n));        //delete content 
		len = n;
	}

	// allocate n elements and copy from [beginIt, beginIt+len)
	template<typename InputIt>
	CCDK_FORCEINLINE void allocate_copy(InputIt beginIt, size_type n) {
		if (n == 0) return;
		content = allocate_n(n);
		ccdk_safe_cleanup_if_exception(
			util::construct_copy_n(iterator_type{ content }, beginIt, n),   //may throw if copy-ctor 
			allocator_type::deallocate(*this, content, n));        //delete content 
		len = n;
	}


	CCDK_FORCEINLINE void append_n(size_type n) {
		auto pair = allocator_type::allocate(*this, n);
		if (len == 0) {
			head = pair.first;
			head->prev = pair.second;
			pair.second->next = head;
		} else {
			node_type* tail = head->prev;
			tail->next = pair.first;
			pair.first->prev = next;
			pair.second->next = head;
			head->prev = pair.second;
		}
	}

	// copy range
	template<typename InputIt>
	CCDK_FORCEINLINE this_type& copy_range(InputIt beginIt, size_type n) {
		if (n > 0) {
			destruct_content();
			if (n > len) append_n(len - n);
			util::construct_copy_n(iterator_type{ head }, beginIt, n);   //may throw if copy-ctor 
			len = n;
		}
		return *this;
	}

	// copy range
	template<typename InputIt>
	CCDK_FORCEINLINE this_type& fill_range(T const& t, size_type n) {
		if (n > 0) {
			destruct_content();
			if (n > len) append_n(len - n);
			util::construct_fill_n(iterator_type{ head }, t, n);   //may throw if copy-ctor 
			len = n;
		}
		return *this;
	}
};
ccdk_namespace_ct_end