#pragma once

#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/util/move.h>
#include<ccdk/mpl/util/fill.h>
#include<ccdk/mpl/util/copy.h>
#include<ccdk/mpl/util/construct.h>
#include<ccdk/mpl/util/swap.h>
#include<ccdk/mpl/fusion/pair.h>
#include<ccdk/mpl/fusion/local_obj.h>
#include<ccdk/mpl/iterator/biward_list_iterator.h>
#include<ccdk/mpl/iterator/reverse_iterator.h>
#include<ccdk/mpl/iterator/algorithm/advance.h>
#include<ccdk/mpl/iterator/algorithm/distance.h>
#include<ccdk/memory/allocator_traits.h>
#include<ccdk/memory/simple_new_allocator.h>
#include<ccdk/memory/list_allocate_adapter.h>
#include<ccdk/container/impl/link_node.h>
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
	using this_type = blist;
	using node_type = Node;

	/* container */
	using value_type      = T;
	using pointer         = T *;
	using const_ponter    = T const*;
	using reference       = T &;
	using const_reference = T const&;
	using size_type       = Size;
	using difference      = ptr::diff_t;
	using allocator_type  = mem::list_allocate_adapter<typename Alloc::template rebind<Node>>;

	/* iterator */
	using iterator               = it::iterator<biward_category, Node>;
	using const_iterator         = it::iterator<biward_category, const Node>;
	using reverse_iterator       = it::reverse_iterator<iterator>;
	using const_reverse_iterator = it::reverse_iterator<const_iterator>;

	template<typename T2, typename Size2, typename Alloc2, typename Node2>
	friend class blist;

private:
	fs::local_obj<Node> head;  // locally head, no need alloc
	size_type           len;
public:

	// destruct this
	CCDK_FORCEINLINE ~blist() noexcept {
		if (len > 0) {
			destruct_content();
			deallocate();
			len = 0;
		}
	}

	// default and nullptr ctor
	CCDK_FORCEINLINE constexpr blist() 
		:head{}, len{ 0 } { init_head(); }
	CCDK_FORCEINLINE constexpr blist(ptr::nullptr_t) 
		: head{}, len{ 0 } { init_head(); }

	// fill ctor
	CCDK_FORCEINLINE explicit blist(size_type n, T const& t = T()) {
		allocate_fill(n, t);
	}

	//  range-n ctor
	template<typename InputIt, typename = check_t< is_iterator<InputIt>>>
	CCDK_FORCEINLINE blist(InputIt beginIt, size_type n) {
		allocate_copy(beginIt, n);
	}

	//  range ctor
	template<typename InputIt, typename = check_t< is_iterator<InputIt>>>
	CCDK_FORCEINLINE blist(InputIt beginIt, InputIt endIt) {
		allocate_copy(beginIt, alg::distance(beginIt, endIt));
	}

	// copy ctor
	CCDK_FORCEINLINE blist(blist const& other) {
		allocate_copy(other.cbegin(), other.size());
	}

	// template copy ctor, node_type can  be different but T must be same
	template<typename Size2, typename Node2>
	CCDK_FORCEINLINE blist(blist<T, Size2, Alloc, Node2> const& other) {
		allocate_copy(other.begin(), other.size());
	}

	// sim initialize_list 
	template<uint32 N>
	CCDK_FORCEINLINE explicit blist(T const (&arr)[N]) {
		allocate_copy(arr, N);
	}

	// move ctor
	CCDK_FORCEINLINE blist(blist && other) noexcept: head{}, len { other.len } {
		if (len > 0) {
			head->next = other.head->next;
			head->prev = other.head->prev;
			head->next->prev = head.address();
			head->prev->next = head.address();
			other.rvalue_reset();
		}
	}

	// template move, node_type need be same
	template<typename Size2>
	CCDK_FORCEINLINE blist(blist<T, Size2, Alloc, Node> && other)
		noexcept: head{}, len{ other.len } {
		head->next = other.head->next;
		head->prev = other.head->prev;
		other.rvalue_reset();
	}

	// swap data
	CCDK_FORCEINLINE void swap(blist& other) noexcept {
		node_type* tmp;
		tmp = other.head->next;
		other.head->next = head->next;
		tmp->prev = head.address();
		head->next->prev = other.head.address();
		head->next = tmp;
		tmp = other.head->prev;
		other.head->prev = head->prev;
		tmp->next = head.address();
		head->prev->next = other.head.address();
		head->prev = tmp;
		util::swap(len, other.len);
	}

	// copy assign 
	CCDK_FORCEINLINE this_type& operator=(blist const& other) {
		return copy_range(other.cbegin(), other.size());
	}


	// template copy assign 
	template<typename Size2, typename Node2>
	CCDK_FORCEINLINE this_type& operator=(
		blist<T, Size2, Alloc, Node2> const& other) {
		return copy_range(other.cbegin(), other.size());
	}

	// move assign 
	CCDK_FORCEINLINE this_type& operator=(blist && other) {
		this->~blist();
		this->rvalue_set(other.head.address(), other.len);
		other.rvalue_reset();
		return *this;
	}


	// template move assign 
	template<typename Size2>
	CCDK_FORCEINLINE this_type& operator=(
		blist<T, Size2, Alloc, Node> && other) {
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
	template<
		typename InputIt,
		typename = check_t< is_iterator<InputIt>> >
	CCDK_FORCEINLINE this_type& assign(InputIt beginIt, size_type n) {
		return copy_range(beginIt, n);
	}

	// range assign 
	template<
		typename InputIt,
		typename = check_t< is_iterator<InputIt>>>
	CCDK_FORCEINLINE this_type& assign(InputIt beginIt, InputIt endIt) {
		return copy_range(beginIt, alg::distance(beginIt, endIt) );
	}

	/* attribute */
	CCDK_FORCEINLINE size_type size() const noexcept { return len;  }
	CCDK_FORCEINLINE size_type empty() const noexcept { return len==0; }
	CCDK_FORCEINLINE size_type max_size() const noexcept { 
		return allocate_type::max_allocate_size();
	}

	// access front
	CCDK_FORCEINLINE reference front() noexcept 
	{ ccdk_assert(len > 0); return head->next->value; }
	CCDK_FORCEINLINE const_reference front() const noexcept
	{ ccdk_assert(len > 0); return head->next->value; }

	// access back
	CCDK_FORCEINLINE reference back() noexcept 
	{ ccdk_assert(len > 0); return head->prev->value; }
	CCDK_FORCEINLINE const_reference back() const noexcept 
	{ ccdk_assert(len > 0); return head->prev->value; }


	/* iterator */
	CCDK_FORCEINLINE iterator begin() noexcept { return { head->next }; }
	CCDK_FORCEINLINE iterator end() noexcept { return { head.address() }; }
	CCDK_FORCEINLINE const_iterator cbegin() const noexcept { return { head->next }; }
	CCDK_FORCEINLINE const_iterator cend() const noexcept { return { head.address() }; }
	CCDK_FORCEINLINE reverse_iterator rbegin() noexcept { return { head->prev }; }
	CCDK_FORCEINLINE reverse_iterator rend() noexcept { return { head.address() }; }
	CCDK_FORCEINLINE const_reverse_iterator crbegin() const noexcept { return { head->prev }; }
	CCDK_FORCEINLINE const_reverse_iterator crend() const noexcept { return { head.address() }; }

	// pop back
	CCDK_FORCEINLINE this_type& pop_back() noexcept {
		if (len > 0) {
			node_type* last = head->prev;
			last->prev->next = head.address();
			head->prev = last->prev;
			allocator_type::deallocate(*this, last, 1);
			--len;
		}
		return *this;
	}

	// pop back
	CCDK_FORCEINLINE this_type& pop_front() noexcept {
		if (len > 0) {
			node_type* first = head->next;
			first->next->prev = head.address();
			head->next = first->next;
			allocator_type::deallocate(*this, first, 1);
			--len;
		}
		return *this;
	}

	// emplace back elements aq2	
	template<typename... Args>
	CCDK_FORCEINLINE this_type& emplace_back(Args&&... args) {
		node_type* node = new_node(util::forward<Args>(args)...);
		head->prev->next = node;
		node->prev = head->prev;
		head->prev = node;
		node->next = head.address();
		++len;
		return *this;
	}

	CCDK_FORCEINLINE this_type& push_back(T const& t) {
		return emplace_back(t);
	}

	CCDK_FORCEINLINE this_type& push_back(T && t) {
		return emplace_back(util::move(t) );
	}

	// emplace back elements 
	template<typename... Args>
	CCDK_FORCEINLINE this_type& emplace_front(Args&&... args) {
		node_type* node = new_node(util::forward<Args>(args)...);
		head->next->prev = node;
		node->next = head->next;
		head->next = node;
		node->prev = head.address();
		++len;
		return *this;
	}

	CCDK_FORCEINLINE this_type& push_front(T const& t) {
		return emplace_front(t);
	}

	CCDK_FORCEINLINE this_type& push_front(T && t) {
		return emplace_front(util::move(t));
	}

	template<typename... Args>
	CCDK_FORCEINLINE this_type& emplace(const_iterator it, Args&&... args) {
	    node_type* next_to_insert = const_cast<node_type*>(it.data());
		node_type* node = new_node(util::forward<Args>(args)...);
		next_to_insert->prev->next = node;
		node->prev = next_to_insert->prev;
		node->next = next_to_insert;
		next_to_insert->prev = node;
		++len;
		return *this;
	}

	CCDK_FORCEINLINE this_type& insert(const_iterator it,T const& t) {
		return emplace(pos, t);
	}

	CCDK_FORCEINLINE this_type& insert(const_iterator it, T && t) {
		return emplace(pos, util::move(t));
	}

	CCDK_FORCEINLINE iterator erase(const_iterator it) noexcept {
		ccdk_assert(it != end());
		if (len > 0) {
		    node_type* to_erase = const_cast<node_type*>(it.data());
			node_type* next = to_erase->next;
			to_erase->prev->next = to_erase->next;
			next->prev = to_erase->prev;
			util::destruct<T>(to_erase);
			allocator_type::deallocate(*this, to_erase,1);
			--len;
			return { next };
		}
		return { head.address() };
	}

	CCDK_FORCEINLINE iterator erase(const_iterator beginIt, 
		const_iterator endIt) noexcept {
		node_type* beginPtr = const_cast<node_type*>(beginIt.data());
		node_type* endPtr = const_cast<node_type*>(endIt.data());
		size_type n = alg::distance(beginIt, endIt);
		ccdk_assert(n <= len);
		node_type* before = beginPtr->prev;
		before->next = endPtr;
		endPtr->prev = before;
		util::destruct_range(beginIt, endIt);
		allocator_type::deallocate(*this, beginPtr, n);
		len -= n;
		return { endPtr };
	}

	CCDK_FORCEINLINE this_type& clear() noexcept { 
		erase(begin(), end()); return *this; 
	}

private:
	// reset r-value
	CCDK_FORCEINLINE void rvalue_reset() noexcept {
		head->next = nullptr;
		head->prev = nullptr;
		len = 0;
	}

	// set to another list
	CCDK_FORCEINLINE void rvalue_set(node_type* other_head, size_type n) noexcept {
		head->next = other_head->next;
		head->prev = other_head->prev;
		head->next->prev = head.address();
		head->prev->next = head.address();
		len = n;
	}

	//init head
	CCDK_FORCEINLINE void init_head() noexcept {
		head->next = head.address();
		head->prev = head.address();
	}

	// destruct objects in list
	CCDK_FORCEINLINE void destruct_content() noexcept {
		if (len > 0) util::destruct_n(begin(), len);
	}

	// deallocate memory
	CCDK_FORCEINLINE void deallocate() {
		ccdk_assert(allocator_type::deallocate( *this,
			head->next, len) == head.address() );
	}

	// destruct and deallocate
	CCDK_FORCEINLINE void destroy_node(node_type* node) {
		util::destruct<T>(node);
		allocator_type::deallocate(*this, node, 1);
	}

	// construct one node
	template<typename... Args>
	CCDK_FORCEINLINE node_type* new_node(Args&&... args) {
		auto range = allocator_type::allocate(*this, 1);
		util::construct<T>(range.first, util::forward<Args>(args)...);
		return range.first;
	}

	// allocate n elements and fill with t
	CCDK_FORCEINLINE void allocate_fill(size_type n, T const& t) {
		if (n == 0) return;
		node_type *new_head, *new_tail;
		//TODO , check throw
		fs::tie(new_head, new_tail) = allocator_type::allocate(*this, n);  
		head->next = new_head;
		new_head->prev = head.address();
		new_tail->next = head.address();
		head->prev = new_tail;
		ccdk_safe_cleanup_if_exception(
			//may throw if copy-ctor
			util::construct_fill_n( begin(), t, n),    
			//delete content 
			allocator_type::deallocate(*this, head->next, n));        
		len = n;
	}

	// allocate n elements and copy from [beginIt, beginIt+len)
	template<typename InputIt>
	CCDK_FORCEINLINE void allocate_copy(InputIt beginIt, size_type n) {
		if (n == 0) return;
		node_type *new_head, *new_tail;
		fs::tie(new_head, new_tail) = allocator_type::allocate(*this, n);  //TODO , check throw
		head->next = new_head;
		new_head->prev = head.address();
		new_tail->next = head.address();
		head->prev = new_tail;
		ccdk_safe_cleanup_if_exception(
			util::construct_copy_n( begin(), beginIt, n),         //may throw if copy-ctor 
			allocator_type::deallocate(*this, head->next, n));    //delete content 
		len = n;
	}


	// append n node to tail
	CCDK_FORCEINLINE void append_n(size_type n) {
		auto pair = allocator_type::allocate(*this, n);  //TODO. allocate memory
		node_type* tail = head->prev;
		tail->next = pair.first;
		pair.first->prev = tail;
		pair.second->next = head.address();
		head->prev = pair.second;
	}

	// release n-tail node
	CCDK_FORCEINLINE void release_n(size_type n) {
		node_type* it = head.address();
		size_type count = n;
		while (count--) {
			it = it->prev;
			ccdk_assert(it != head.address());
		}
		node_type* rls_head = it->next;
		it->prev->next = head.address();
		head->prev = it->prev;
		ccdk_assert(allocator_type::deallocate(*this, it, n) == head.address());
	}

	// copy range
	template<typename InputIt>
	CCDK_FORCEINLINE this_type& copy_range(InputIt beginIt, size_type n) {
		if (n > 0) {
			destruct_content();
			if (n > len) append_n(n - len);
			else if (n < len) release_n(len - n);
			util::construct_copy_n( begin(), beginIt, n);   //may throw if copy-ctor 
			len = n;
		}
		return *this;
	}

	// copy range
	CCDK_FORCEINLINE this_type& fill_range(T const& t, size_type n) {
		if (n > 0) {
			destruct_content();
			if (n > len) append_n(n - len);
			if (n < len) release_n(len - n);
			util::construct_fill_n( begin(), t, n);   //may throw if copy-ctor 
			len = n;
		}
		return *this;
	}

///////////////////////////////////////////////////////////////////////////////////////////////
/////
public:
	void debug(char const* title) {
		std::cout << title << " :";
		for (auto it = this->begin(); it != end(); ++it) {
			std::cout << *it << " ";
		}
		std::cout << std::endl;
	}
	void cdebug(char const* title) const {
		std::cout << title << " :";
		for (auto it = this->cbegin(); it != cend(); ++it) {
			std::cout << *it << " ";
		}
		std::cout << std::endl;
	}
	void crdebug(char const* title) const {
		std::cout << title << " :";
		for (auto it = this->crbegin(); it != crend(); ++it) {
			std::cout << *it << " ";
		}
		std::cout << std::endl;
	}
};
ccdk_namespace_ct_end