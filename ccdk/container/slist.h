#pragma once

#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/base/if_.h>
#include<ccdk/mpl/util/destruct.h>
#include<ccdk/mpl/util/construct.h>
#include<ccdk/mpl/util/swap.h>
#include<ccdk/mpl/util/move.h>
#include<ccdk/mpl/iterator/forward_list_iterator.h>
#include<ccdk/mpl/iterator/reverse_iterator.h>
#include<ccdk/mpl/iterator/algorithm/distance.h>
#include<ccdk/memory/allocator_traits.h>
#include<ccdk/memory/allocator/simple_new_allocator.h>
#include<ccdk/memory/allocator/local_cached_allocator.h>
#include<ccdk/memory/adapter/list_allocate_adapter.h>
#include<ccdk/container/container_mudule.h>
#include<ccdk/container/impl/link_node.h>

ccdk_namespace_ct_start

using namespace mpl;

/*
	T:         element type
	Size:      size type, store length
	Alloc:     base allocator type
	Node:      list node type, need have next and data field
*/
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
	using link_type = Node * ;

	/* container type */
	using value_type      = T;
	using pointer         = T*;
	using const_pointer   = T const*;
	using reference       = T&;
	using const_reference = T const&;
	using size_type       = Size;
	using difference_type = ptr::diff_t;
	using base_allocator  = typename Alloc::template rebind<Node>;
	using allocator_type  = mem::list_allocate_adapter< base_allocator > ;

	/* iterator */
	using iterator       = it::iterator< forward_category, node_type >;
	using const_iterator = it::iterator< forward_category, const node_type >;

	template<typename T2, typename Size2, typename Alloc2, typename Node2>
	friend class slist;

private:
	link_type  head;
	link_type  tail;
	size_type  len;
public:

///////////////////////////////////////////////////////////////////////
//// ctor / de-ctor

	// de-ctor
	CCDK_FORCEINLINE ~slist() {
		if (len > 0) { 
			destroy();
			rvalue_set(); 
		}
	}

	// default / nullptr ctor
	CCDK_FORCEINLINE constexpr slist() 
		noexcept : head{ nullptr }, tail{ nullptr }, len{ 0 } {}
	CCDK_FORCEINLINE constexpr slist(ptr::nullptr_t) 
		noexcept : head{ nullptr }, tail{ nullptr }, len{ 0 } {}

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
		: slist(begin, end) {}

	// template copy
	template<typename Size2, typename Alloc2,typename Node2>
	CCDK_FORCEINLINE slist(slist<T,Size2,Alloc2,Node2> const& other) 
		: slist(other.begin(), other.size()) {}

	// copy
	CCDK_FORCEINLINE slist(slist const& other)
		: slist(other.begin(), other.size()) {}

	// array initialize
	template<uint32 N>
	CCDK_FORCEINLINE slist(T const(&arr)[N])
		: slist(arr, N) {}


	// template move 
	template<typename Size2, typename Alloc2, typename Node2>
	CCDK_FORCEINLINE slist(slist<T, Size2, Alloc, Node> && other)
		noexcept : head{ other.head }, tail{ other.tail }, len { other.len } {
		other.rvalue_reset();
	}

	/* move */
	CCDK_FORCEINLINE slist(slist && other)
		noexcept : head{ other.head }, tail{ other.tail }, len{ other.len } {
		other.rvalue_reset();
	}

///////////////////////////////////////////////////////////////////////////////
//// swap / assign

	//swap with same type
	CCDK_FORCEINLINE void swap(slist & other) noexcept {
		util::swap(head, other.head);
		util::swap(len, other.len);
	}

	// template copy assign 
	template<typename Size2, typename Alloc2, typename Node2>
	CCDK_FORCEINLINE this_type& operator=(slist<T, Size2, Alloc2, Node2> const& other) {
		assign_copy(other.begin(), other.len);
		return *this;
	}

	/* copy assign */
	CCDK_FORCEINLINE this_type& operator=(slist const& other) {
		assign_copy(other.begin(), other.len);
		return *this;
	}

	/* template copy assign  */
	template<typename Size2, typename Alloc2, typename Node2>
	CCDK_FORCEINLINE this_type& operator=(slist<T, Size2, Alloc, Node> && other) {
		rvalue_set(other.head, other.len);
		other.rvalue_reset();
		return *this;
	}

	/* move assign */
	CCDK_FORCEINLINE this_type& operator=(slist && other) {
		rvalue_set(other.head, other.len);
		other.rvalue_reset();
		return *this;
	}

	/* range-n assign */
	CCDK_FORCEINLINE this_type& assign(size_type n, T const& t = T()) {
		assign_fill(n, t);
		return *this;
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
	CCDK_FORCEINLINE const_iterator begin() const noexcept { return { head }; }
	CCDK_FORCEINLINE const_iterator end() const noexcept { return { nullptr }; }

	CCDK_FORCEINLINE const_iterator cbegin() const noexcept { return{ head }; }
	CCDK_FORCEINLINE const_iterator cend() const noexcept { return{ nullptr }; }

	/* attribute */
	CCDK_FORCEINLINE size_type size() const noexcept { return len;}
	CCDK_FORCEINLINE size_type max_size() const noexcept { 
		return allocator_type::max_allocate_size();  
	}

	/* access */
	CCDK_FORCEINLINE reference front() noexcept { return head->data; }
	CCDK_FORCEINLINE const_reference front() const noexcept { return head->data; }
	
	/* operation */
	CCDK_FORCEINLINE this_type& pop_front() noexcept {
		link_type other = head->next;
		util::destruct<T>(head);
		allocator_type::deallocate(*this, head, 1);
		head = other;
		--len;
		return *this;
	}

	/* remove head */
	CCDK_FORCEINLINE this_type& push_front(T const& t) noexcept {
		node_type* memory = allocator_type::allocate(*this, 1).first;
		util::construct<T>(memory, t);
		memory->next = head;
		head = memory;
		++len;
		return *this;
	}

	/* push to head */
	CCDK_FORCEINLINE this_type& push_front(T && t) noexcept {
		link_type memory = allocator_type::allocate(*this, 1).first;
		util::construct<T>(memory, util::move(t));
		memory->next = head;
		head = memory;
		++len;
		return *this;
	}

private:
	CCDK_FORCEINLINE void destruct_content() noexcept {
		util::destruct_n(begin(), len);
	}

	CCDK_FORCEINLINE void deallocate() noexcept {
		allocator_type::deallocate(*this, head, len);
	}

	CCDK_FORCEINLINE void destroy() noexcept {
		destruct_content();
		deallocate();
	}

	CCDK_FORCEINLINE void rvalue_reset() noexcept { 
		head = nullptr; 
		tail = nullptr;
		len = 0; 
	}
	CCDK_FORCEINLINE void rvalue_set(link_type newHead,
		link_type newTail, size_type newSize) noexcept {
		destroy();
		head = newHead; 
		tail = newTail;
		len = newSize;
	}


	CCDK_FORCEINLINE void allocate_fill(size_type n, T const& t) {
		if (n == 0) return;
		fs::tie(head,tail) = allocator_type::allocate(*this, n);
		util::construct_fill_n(iterator{ head }, t, n);
		len = n;
	}

	template<typename InputIt>
	CCDK_FORCEINLINE void allocate_copy(InputIt beginIt, size_type n) {
		if (n == 0) return;
		fs::tie(head, tail) = allocator_type::allocate(*this, n);
		util::construct_copy_n(iterator{ head }, beginIt, n);
		len = n;
	}

	template<typename InputIt>
	CCDK_FORCEINLINE void assign_copy(InputIt beginIt, size_type n) {
		if (n == 0) return;
		destruct_content();
		iterator tail1{nullptr}, tail2{nullptr};
		fs::tie(tail1, tail2) = util::construct_copy_n(begin(), beginIt, fn::min(len,n) - 1);
		if (n > len) {
			ccdk_assert( ++tail1 == end() );
			tail1.content->next = allocator_type::allocate(*this,n - len).first;
			util::construct_copy_n(tails.first, tails.second, n - len + 1);
		} else {
			//assign front n-1 elements
			auto tails = util::construct_copy_n(begin(), beginIt, n - 1);
			link_type last = tails.first.content;
			ccdk_assert(last);
			link_type to_delete = last->next;
			//assign last element
			last->data = *(tails.second);
			last->next = nullptr;
			if (n < len) {
				ccdk_assert(to_delete);
				allocator_type::deallocate(*this, to_delete, len-n);
			}
		}
		len = n;
	}

	CCDK_FORCEINLINE void assign_fill(size_type n, T const& t) {
		if (n == 0) return;
		destruct_content();
		if (n > len) {
			auto last = util::construct_fill_n(begin(), t, len-1);
			ccdk_assert(last.content != nullptr);
			last.content->next = allocator_type::allocate(*this, n - len).first;
			util::construct_fill_n(last, t, n - len + 1);
		} else {
			auto last = util::construct_fill_n(begin(), t, n-1);
			*last = t;
			link_type to_delete = last.content->next;
			last.content->next = nullptr;
			if (n < len) {
				ccdk_assert(to_delete);
				allocator_type::deallocate(*this, to_delete, len - n);
			}
		}
		len = n;
	}

///////////////////////////////////////////////////////////////////
//// transform 

	// loop over
	template<typename Pred>
	CCDK_FORCEINLINE void foreach(Pred pred) const {
		link_type node = head;
		while (node) {
			pred(node->data);
			node = node->next;
		}
	}

	// mapping to a new list
	template<typename Transform>
	CCDK_FORCEINLINE this_type map(Transform transform) {
		this_type ret{};
		link_type node = head;
		while (node) {
			ret.push_back( transform(node->data));
			node = node->next;
		}
		return ret;
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
	uint32 CacheSize = 30,
	typename Size = uint32,
	typename Alloc = mem::simple_new_allocator<T>,
	typename Node = forward_node<T>
>
using cached_slist = slist<T, Size,
	mem::local_cached_allocator<CacheSize,Alloc>, Node>;


ccdk_namespace_ct_end