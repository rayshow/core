#pragma once

#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/base/if_.h>
#include<ccdk/mpl/util/destruct.h>
#include<ccdk/mpl/util/construct.h>
#include<ccdk/mpl/util/swap.h>
#include<ccdk/mpl/util/move.h>
#include<ccdk/mpl/fusion/local_obj.h>
#include<ccdk/mpl/iterator/forward_list_iterator.h>
#include<ccdk/mpl/iterator/algorithm/distance.h>
#include<ccdk/memory/allocator/simple_new_allocator.h>
#include<ccdk/memory/allocator/local_cached_allocator.h>
#include<ccdk/memory/adapter/list_allocate_adapter.h>
#include<ccdk/container/container_mudule.h>
#include<ccdk/container/impl/link_node.h>

ccdk_namespace_ct_start

using namespace mpl;

//TODO add insert / erase
/*
	T:         element type
	Size:      size type, store length
	Alloc:     base allocator type
	Node:      list node type, need have next and data field
*/
template<
	typename T,
	typename Size = uint32,
	typename Alloc = mem::simple_new_allocator<T,Size>,
	typename Node = forward_node<T>
>
class slist : protected Alloc::template rebind<Node>
{
public:
	using this_type = slist;
	using node_type = Node;
	using link_type = Node *;

	/* container type */
	using value_type      = T;
	using pointer         = T * ;
	using const_pointer   = T const*;
	using reference       = T & ;
	using const_reference = T const&;
	using size_type       = Size;
	using difference_type = ptr::diff_t;
	using base_allocator  = typename Alloc::template rebind<Node>;
	using allocator_type  = mem::list_allocate_adapter< base_allocator >;

	/* iterator */
	using iterator       = it::iterator< forward_category, node_type >;
	using const_iterator = it::iterator< forward_category, const node_type >;

	template<typename T2, typename Size2, typename Alloc2, typename Node2>
	friend class slist;

private:
	fs::local_obj<Node> head;  //a empty node 
	link_type           tlink; //pointer to tail node
	size_type           len;   //length
public:

///////////////////////////////////////////////////////////////////////
//// ctor / de-ctor

	// de-ctor
	CCDK_FORCEINLINE ~slist() { destroy(); }

	// default / nullptr ctor
	CCDK_FORCEINLINE constexpr slist()
		noexcept : head{}, tlink{ head.address() }, len{ 0 } {}
	CCDK_FORCEINLINE constexpr slist(ptr::nullptr_t)
		noexcept : head{}, tlink{ head.address() }, len{ 0 } {}

	// fill 
	CCDK_FORCEINLINE slist(size_type n, T const& t = T()) {
		allocate_fill(n, t);
	}

	// copy range-n
	template<typename InputIt, typename = check_t< is_iterator<InputIt>>>
	CCDK_FORCEINLINE slist(InputIt begin, size_type n) {
		allocate_copy(begin, n);
	}

	// copy range 
	template<typename InputIt, typename = check_t< is_iterator<InputIt>>>
	CCDK_FORCEINLINE slist(InputIt begin, InputIt end)
		: slist{ begin, it::distance(begin, end)} {}

	// template copy
	template<typename Size2, typename Alloc2,typename Node2>
	CCDK_FORCEINLINE slist(slist<T,Size2,Alloc2,Node2> const& other) 
		: slist{ other.begin(), other.size() } {}

	// copy
	CCDK_FORCEINLINE slist(slist const& other)
		: slist{ other.begin(), other.size() } {}

	// array initialize
	template<uint32 N>
	CCDK_FORCEINLINE slist(T const(&arr)[N])
		: slist{ arr, N } {}

	// template move 
	template<typename Size2, typename Alloc2, typename Node2>
	CCDK_FORCEINLINE slist(slist<T, Size2, Alloc, Node> && other)
		noexcept : head{}, tlink{ other.tlink }, len { other.len } {
		head->next = other.head->next;
		other.rvalue_reset();
	}

	// move
	CCDK_FORCEINLINE slist(slist && other)
		noexcept : head{}, tlink{ other.tlink }, len{ other.len } {
		head->next = other.head->next;
		other.rvalue_reset();
	}

///////////////////////////////////////////////////////////////////////////////
//// swap / assign

	//swap with same type
	CCDK_FORCEINLINE void swap(slist & other) noexcept {
		link_type tmp = other.head->next;
		other.head->next = head->next;
		head->next = tmp;
		util::swap(tlink, other.tlink);
		util::swap(len, other.len);
	}

	// template copy assign 
	template<typename Size2, typename Alloc2, typename Node2>
	CCDK_FORCEINLINE this_type& operator=(slist<T, Size2, Alloc2, Node2> const& other) {
		return assign_copy(other.begin(), other.len);
	}

	// copy assign 
	CCDK_FORCEINLINE this_type& operator=(slist const& other) {
		return assign_copy(other.begin(), other.len);
	}

	// template copy assign  
	template<typename Size2, typename Alloc2, typename Node2>
	CCDK_FORCEINLINE this_type& operator=(slist<T, Size2, Alloc, Node> && other) {
		destroy();
		rvalue_set(other.head, other.tlink, other.size());
		other.rvalue_reset();
		return *this;
	}

	/* move assign */
	CCDK_FORCEINLINE this_type& operator=(slist && other) {
		destroy();
		rvalue_set(other.head, other.tlink, other.size());
		other.rvalue_reset();
		return *this;
	}

	//assign array
	CCDK_FORCEINLINE this_type& operator=(std::initializer_list<T> const& lst) {
		return assign_copy(lst.begin(), lst.size());
	}

	// range-n assign
	CCDK_FORCEINLINE this_type& assign(size_type n, T const& t = T()) {
		return assign_fill(n, t);
	}

	// range-n assign
	template<typename InputIt, typename = check_t< is_iterator<InputIt>>>
	CCDK_FORCEINLINE this_type& assign(InputIt beginIt, InputIt endIt) {
		return assign_copy(beginIt, it::distance(beginIt, endIt));
	}

	// range-n assign 
	template<typename InputIt, typename = check_t< is_iterator<InputIt>>>
	CCDK_FORCEINLINE this_type& assign(InputIt beginIt, size_type n) {
		return assign_copy(beginIt, n);
	}

	//iterator
	CCDK_FORCEINLINE iterator begin() noexcept { return { head->next };}
	CCDK_FORCEINLINE iterator end() noexcept { return { nullptr }; }
	CCDK_FORCEINLINE const_iterator begin() const noexcept { return { head->next }; }
	CCDK_FORCEINLINE const_iterator end() const noexcept { return { nullptr }; }
	CCDK_FORCEINLINE const_iterator cbegin() const noexcept { return{ head->next }; }
	CCDK_FORCEINLINE const_iterator cend() const noexcept { return{ nullptr }; }

	//readonly attribute
	CCDK_FORCEINLINE size_type size() const noexcept { return len;}
	CCDK_FORCEINLINE bool empty() const noexcept { return len==0; }
	CCDK_FORCEINLINE constexpr size_type max_size() const noexcept { return size_type(-1); }

	//element
	CCDK_FORCEINLINE reference front() noexcept { ccdk_assert(head->next); return head->next->data; }
	CCDK_FORCEINLINE const_reference front() const noexcept { ccdk_assert(head->next); return head->next->data; }
	CCDK_FORCEINLINE reference back() noexcept { ccdk_assert( tlink!=head.address()); return tlink->data; }
	CCDK_FORCEINLINE const_reference back() const noexcept { ccdk_assert(tlink != head.address()); return tlink->data; }

/////////////////////////////////////////////////////////////////////
//// pop/push front / push back

	//pop front
	CCDK_FORCEINLINE this_type& pop_front() noexcept {
		link_type node = head->next;
		if (node) {
			head->next = node->next;
			util::destruct<T>(node);
			allocator_type::deallocate(*this, node, 1);
			--len;
		}
		return *this;
	}

	// emplace at front
	template<typename... Args>
	CCDK_FORCEINLINE this_type& emplace_front(Args&&... args) noexcept {
		link_type node = new_node(util::forward<Args>(args)...);
		node->next = head->next;
		head->next = node;
		++len;
		return *this;
	}

	// push at head
	CCDK_FORCEINLINE this_type& push_front(T && t) noexcept { return emplace_front(util::move(t)); }
	CCDK_FORCEINLINE this_type& push_front(T const& t) noexcept { return emplace_front(t); }

	// emplace at tail
	template<typename... Args>
	CCDK_FORCEINLINE this_type& emplace_back(Args&&... args) noexcept {
		link_type node = new_node(util::forward<Args>(args)...);
		tlink->next = node;
		tlink = node;
		++len;
		return *this;
	}

	// push tail
	CCDK_FORCEINLINE this_type& push_back(T const& t) noexcept { return emplace_back(t); }
	CCDK_FORCEINLINE this_type& push_back(T && t) noexcept { return emplace_back(util::move(t));}

///////////////////////////////////////////////////////////////////
//// transform / inner loop

	// loop over
	template<typename FN>
	CCDK_FORCEINLINE void foreach(FN Fn) const {
		link_type node = head->next;
		while (node) {
			Fn(node->data);
			node = node->next;
		}
	}

	// mapping to a new list
	template<typename Transform>
	CCDK_FORCEINLINE this_type map(Transform Fn) {
		this_type ret{};
		link_type node = head->next;
		while (node) {
			ret.push_back(Fn(node->data));
			node = node->next;
		}
		return ret;
	}

private:
	//destruct content
	CCDK_FORCEINLINE void destruct_content() noexcept {
		util::destruct_n(begin(), len);
	}

	// recycle memory
	CCDK_FORCEINLINE void deallocate() noexcept {
		allocator_type::deallocate(*this, head->next, len);
	}

	//destruct content and recycle memory
	CCDK_FORCEINLINE void destroy() noexcept {
		destruct_content();
		deallocate();
		rvalue_reset();
	}

	//reset 3
	CCDK_FORCEINLINE void rvalue_reset() noexcept { 
		head->next = nullptr;
		tlink = nullptr;
		len = 0; 
	}

	//set 3
	CCDK_FORCEINLINE void rvalue_set(
		fs::local_obj<Node> const& newHead,
		link_type newTail, size_type newSize) noexcept {
		head = newHead; 
		tlink = newTail;
		len = newSize;
	}

	// emplace construct node with args...
	template<typename... Args>
	CCDK_FORCEINLINE link_type new_node(Args&&... args) {
		link_type node = allocator_type::allocate(*this, 1).first;
		util::construct<T>(node, util::forward<Args>(args)...);
		node->next = nullptr;
		return node;
	}

	CCDK_FORCEINLINE void allocate_fill(size_type n, T const& t) {
		if (n == 0) return;
		link_type hlink;
		fs::tie(hlink,tlink) = allocator_type::allocate(*this, n);
		head->next = hlink;
		util::construct_fill_n(begin(), t, n);
		len = n;
	}

	template<typename InputIt>
	CCDK_FORCEINLINE void allocate_copy(InputIt beginIt, size_type n) {
		if (n == 0) return;
		link_type hlink;
		fs::tie(hlink, tlink) = allocator_type::allocate(*this, n);
		head->next = hlink;
		util::construct_copy_n(begin(), beginIt, n);
		len = n;
	}

	template<typename InputIt>
	CCDK_FORCEINLINE this_type& assign_copy(InputIt beginIt, size_type n) {
		if (n > 0) {
			destruct_content();
			if (n > len) {
				link_type nhlink, ntlink;
				fs::tie(nhlink, ntlink) = allocator_type::allocate(*this, n - len);
				ccdk_assert(!tlink->next);
				tlink->next = nhlink;
				tlink = ntlink;
				util::construct_copy_n(begin(), beginIt, n);
			}
			else {
				auto its = util::construct_copy_n(begin(), beginIt, n - 1);
				tlink = its.first.content; //to last node
				tlink->data = *its.second;
				if (n < len) {
					ccdk_assert(tlink->next);
					allocator_type::deallocate(*this, tlink->next, len - n);
				}
				tlink->next = nullptr;
			}
			len = n;
		}
		return *this;
	}

	//assign move
	CCDK_FORCEINLINE this_type& assign_fill(size_type n, T const& t) {
		if (n > 0) {
			destruct_content();
			if (n > len) {
				link_type nhlink, ntlink;
				fs::tie(nhlink, ntlink) = allocator_type::allocate(*this, n - len);
				ccdk_assert(!tlink->next);
				tlink->next = nhlink;
				tlink = ntlink;
				util::construct_fill_n(begin(), t, n);
			}
			else {
				tlink = util::construct_fill_n(begin(), t, n - 1).content;
				tlink->data = t;
				if (n < len) {
					ccdk_assert(tlink->next);
					allocator_type::deallocate(*this, tlink->next, len - n);
				}
				tlink->next = nullptr;
			}
			len = n;
		}
		return *this;
	}

////////////////////////////////////////////////////////////
//// debug content

public:
	CCDK_FORCEINLINE void debug_value() const noexcept {
		foreach([](T const& t) {
			DebugValueIt(t);
		});
		DebugValueItEnd();
	}
};


// defined a 30-cached-node slist, allocate/deallocate will use this cache
template<
	typename T,
	uint32 CacheSize = 10,
	typename Size = uint32,
	typename Alloc = mem::simple_new_allocator<T,Size>,
	typename Node = forward_node<T>
>
using cached_slist = slist<T, Size,
	mem::local_cached_allocator<CacheSize,Alloc>, Node>;


ccdk_namespace_ct_end