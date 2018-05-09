#pragma once

#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/util/move.h>
#include<ccdk/mpl/util/fill.h>
#include<ccdk/mpl/util/copy.h>
#include<ccdk/mpl/util/construct.h>
#include<ccdk/mpl/util/swap.h>
#include<ccdk/mpl/function/operator.h>
#include<ccdk/mpl/iterator/iterator_fwd.h>
#include<ccdk/mpl/units/ratio.h>
#include<ccdk/mpl/iterator/algorithm/advance.h>
#include<ccdk/mpl/iterator/algorithm/distance.h>
#include<ccdk/memory/allocator/simple_new_allocator.h>
#include<ccdk/memory/adapter/list_allocate_adapter.h>

#include<ccdk/container/impl/link_node.h>
#include<ccdk/container/container_mudule.h>

ccdk_namespace_ct_start

using namespace ccdk::mpl;


/* lazy single list:
    delete/pop node will not free/delete its memory, but link to tail->next to keep a cache
	and avoid frequency new/delete
   push/insert node will first utilize cached memory after tail
*/
template<
	typename T, 
	typename Size = uint32,
	typename InceaseRatio = units::ratio<15, 10>,     /* preallocate 0.5  more times node space */
	typename Alloc = mem::simple_new_allocator< T >,
	typename Node = forward_node<T>
>
class circular_slist : protected Alloc::template rebind<Node>
{
public:
	typedef lazy_slist           this_type;
	typedef Node                 node_type;

	/* common */
	typedef T                    value_type;
	typedef T*                   pointer_type;
	typedef T const*             const_pointer_type;
	typedef T&                   reference_type;
	typedef T const&             const_reference_type;
	typedef Size                 size_type;
	typedef ptr::diff_t          difference_type;
	typedef mem::list_allocate_adapter< typename Alloc::template rebind<Node>> allocate_type;  /* allocate multi node*/
	typedef mem::allocator_traits< typename Alloc::template rebind<Node> > _1_allocate_type;   /* allocate 1 node */

	/*iterator*/
	typedef iterator< node_type>       iterator_type;
	typedef iterator< const node_type> const_iterator_type;

	template<typename T2, typename Size2, typename Alloc2, typename Node2>
	friend class lazy_slist;

private:
	node_type* head; /* first useful node of list, use for push_front/ pop_front */
	node_type* tail; /* last  useful node of list, use for push_back  */
	size_type  len;  /* length of [head, tail] */
	size_type  cap;  /* keep cap - len nodes as cache to avoid allocate  */
	
public:
	/*default */
	CCDK_FORCEINLINE constexpr lazy_slist() noexcept 
		: head{ nullptr }, tail{ nullptr }, len{ 0 }, cap{ 0 } {}
	CCDK_FORCEINLINE constexpr lazy_slist(ptr::nullptr_t) noexcept
		: head{ nullptr }, tail{ nullptr }, len{ 0 }, cap{ 0 } {}

	/* fill n */
	CCDK_FORCEINLINE lazy_slist(size_type n, T const& t = T()) { 
		ccdk_assert(n > 0);
		allocate_fill(n, t);
	}

	/* copy range [begin, begin+n)*/
	template<
		typename InputIt,
		typename = check_t< is_iterator<InputIt>>
	>
	CCDK_FORCEINLINE lazy_slist(InputIt begin, ptr::size_t n){
		ccdk_assert(n > 0);
		allocate_copy(n, begin);
	}

	/* copy range [begin, end)*/
	template<
		typename InputIt,
		typename = check_t< is_iterator<InputIt>>
	>
	CCDK_FORCEINLINE lazy_slist(InputIt begin, InputIt end){
		ptr::size_t n = alg::distance(begin, end);
		ccdk_assert(n > 0);
		allocate_copy(n, begin);
	}

	CCDK_FORCEINLINE lazy_slist(std::initializer_list<T> const& lst){
		allocate_copy(lst.size() , lst.begin());
	}

	/* copy contructor*/
	CCDK_FORCEINLINE lazy_slist(lazy_slist const& other) {
		allocate_copy(other.len, other.begin() );
	}

	/* move constructor */
	CCDK_FORCEINLINE lazy_slist(lazy_slist && other)  noexcept
		: head{ other.head }, tail{ other.tail }, len{ other.len } { 
		other.rvalue_reset(); 
	}

	/* template copy constructor */
	template<typename Size2, typename Alloc2, typename Node2>
	CCDK_FORCEINLINE lazy_slist(lazy_slist<T,Size2,Alloc2,Node2> const& other) {
		allocate_copy(other.len, other.begin());
	}

	/* template move constructor */
	template<typename Size2, typename Alloc2, typename Node2>
	CCDK_FORCEINLINE lazy_slist(lazy_slist<T, Size2, Alloc2, Node2> && other) noexcept
		: head{ other.head }, tail{ other.tail }, len{ other.len }, cap{ other.cap } {
		other.rvalue_reset();
	}

	CCDK_FORCEINLINE void swap(this_type& other) noexcept {
		util::swap(head, other.head);
		util::swap(tail, other.tail);
		util::swap(len, other.len);
		util::swao(cap, other.cap);
	}
	
	CCDK_FORCEINLINE this_type& operator=(ptr::nullptr_t) {
		util::destruct_n(begin(), len);
		tail = head; 
		len = 0;
	}

	CCDK_FORCEINLINE this_type& operator=(this_type const& other) {
		ccdk_if_not_this(other) { assign_range(other.len, other.begin()); }
	}

	CCDK_FORCEINLINE this_type& operator=(this_type && other) {
		ccdk_if_not_this(other) { 
			this->rvalue_set(other.head, other.tail, other.len, other.cap);
			other.rvalue_reset();
		}
	}

	 template<typename Size2, typename Alloc2, typename Node2>
	 CCDK_FORCEINLINE this_type& operator=(lazy_slist<T, Size2, Alloc2, Node2> const& other) {
		assign_range(other.len, other.begin());
	}

	template<typename Size2, typename Alloc2, typename Node2>
	CCDK_FORCEINLINE this_type& operator=(lazy_slist<T, Size2, Alloc2, Node2> && other) {
		this->rvalue_set(other.head, other.tail, other.len, other.cap);
		other.rvalue_reset();
	}

	template<typename Size2, typename Alloc2, typename Node2>
	CCDK_FORCEINLINE this_type& assign(lazy_slist<T, Size2, Alloc2, Node2> const& other) {
		ccdk_if_not_this(other) {
			assign_range(other.len, other.begin());
		}
	}

	template<typename Size2, typename Alloc2, typename Node2>
	this_type& assign(lazy_slist<T, Size2, Alloc2, Node2> && other) {
		ccdk_if_not_this(other) {
			this->rvalue_set(other.head, other.tail, other.len, other.cap);
			other.rvalue_reset();
		}
	}

	this_type& assign(size_type n, T const& t = T()) {
		util::destruct_n(begin(), len);
		tail = util::construct_fill_n(begin(), t, n);
		len = n;
	}

	template<typename ForwardIt>
	this_type& assign(ForwardIt begin, ForwardIt end) {
		assign_range(alg::distance(begin,end), begin);
	}

	/* iterator */
	CCDK_FORCEINLINE iterator_type begin() noexcept {
		return iterator_type{ head };
	}

	CCDK_FORCEINLINE iterator_type end() noexcept {
		return iterator_type{ tail ? tail->next:nullptr };
	}

	CCDK_FORCEINLINE iterator_type cbegin() const noexcept {
		return const_iterator_type{ head };
	}

	CCDK_FORCEINLINE const_iterator_type& cend() const noexcept {
		return iterator_type{ tail ? tail->next:nullptr };
	}

	/* attribute */
	CCDK_FORCEINLINE size_type length() const noexcept { return len; }
	CCDK_FORCEINLINE size_type capcity() const noexcept { return cap; }
	CCDK_FORCEINLINE size_type max_size() const noexcept { 
		return allocate_type::max_allocate_size(); 
	}

	/* access */
	CCDK_FORCEINLINE T& front() noexcept {
		return head->data;
	}
	CCDK_FORCEINLINE T const& front() const noexcept {
		return head->data;
	}
	/* access */
	CCDK_FORCEINLINE T& back() noexcept {
		return tail->data;
	}
	CCDK_FORCEINLINE T const& back() const noexcept {
		return tail->data;
	}

	/* emplace back and push back */
	template<typename T, typename... Args>
	CCDK_FORCEINLINE this_type& emplace_back(T && t, Args&& ... args) {
		if (len + 1 = cap) {
			tail->next = _1_allocate_type::allocate(*this, 1);
		}
		ccdk_assert(tail->next);
		tail = tail->next;
		construct<T>(tail, util::forward<T>(t), util::forward<Args>(args)...);
		++len;
		return *this;
	}

	CCDK_FORCEINLINE this_type& push_back(T const& t) {
		return emplace_back(t);
	}

	CCDK_FORCEINLINE this_type& push_back(T && t) {
		return emplace_back(util::move(t));
	}

	/* emplace front and push front */
	template<typename T, typename... Args>
	CCDK_FORCEINLINE this_type& emplace_front(T && t, Args&& ... args) {
		node_type* new_node = get_node();
		construct<T>(new_node, util::forward<T>(t), util::forward<Args>(args)...);
		new_node->next = head;
		head = new_node;
		++len;
		return *this;
	}

	CCDK_FORCEINLINE this_type& push_front(T const& t) {
		return emplace_front(t);
	}

	CCDK_FORCEINLINE this_type& push_front(T&& t) {
		return emplace_front(util::move(t));
	}

	/* just move front node to tail->next */
	CCDK_FORCEINLINE this_type& pop_front() {
		if (len > 0) {
			node_type* node = head;
			head = head->next;
			node->next = tail->next;
			tail->next = node;
			destruct<T>(node);
		}
	}

	/* insert and emplace */
	template<typename P, typename... Args>
	CCDK_FORCEINLINE this_type& emplace(iterator_type before_pos, P&& t, Args&&... args) {
		node_type* node = get_node();
		construct<P>(node, util::forward<P>(t), util::forward<Args>(args)...);
		node->next = pos->next;
		pos->next = node;
	}

	CCDK_FORCEINLINE this_type& insert(iterator_type before_pos, P&& p) {
		return emplace(pos, util::move(p));
	}

	CCDK_FORCEINLINE this_type& insert(iterator_type before_pos, P const& p) {
		return emplace(pos, p);
	}

	/* erase (before, before_end] */
	CCDK_FORCEINLINE this_type& erase(iterator_type before_pos, iterator_type before_end) {
		ccdk_assert(alg::distance(before_pos, before_end) > 0);
		node_type* erase_head = before_pos->next;
		before_pos->next = before_end->next;
		before_end->next = tail->next;
		tail->next = erase_head;
		util::destruct_range(++before_pos, ++before_end);
	}

private:
	CCDK_FORCEINLINE void rvalue_reset() noexcept {
		head = tail = nullptr;
		len = 0;
		cap = 0;
	}

	CCDK_FORCEINLINE void rvalue_set(node_type* head, node_type* tail,
		size_type len, size_type cap) noexcept {
		this->head = head;
		this->tail = tail;
		this->len = len;
		this->cap = cap;
	}

	CCDK_FORCEINLINE void destruct_content() noexcept {
		util::destruct_n(begin(), len);
		len = 0;
		tail = head;
	}

	CCDK_FORCEINLINE node_type* get_node() {
		node_type *node = nullptr;
		if (len + 1 = cap) {
			node = _1_allocate_type::allocate(*this, 1);
		}
		else {
			ccdk_assert(tail->next);
			node = tail->next;
			tail->next = tail->next->next;
		}
		return node;
	}

	CCDK_FORCEINLINE void destruct_this() noexcept {
		destruct_content();
		allocate_type::deallocate(*this, it, cap);
	}

	template<typename InputIt>
	CCDK_FORCEINLINE void allocate_copy(size_type n, InputIt begin) {
		if (n > 0) {
			ccdk_increase_allocate_lst4(n, head, tail, cap, len);
			util::construct_copy_n(iterator_type{ head }, begin, n);
		}
	}

	CCDK_FORCEINLINE void allocate_fill(size_type n, T const& t) {
		if (n > 0) {
			ccdk_increase_allocate_lst4(n, head, tail, cap, len);
			util::construct_fill_n(iterator_type{ head }, t, n);
		}
	}

	template<typename ForwardIt>
	void assign_range(size_type n, ForwardIt begin2) {
		if (n == 0) return;
		destruct_content();
		ptr::size_t copy_len = fn::min(cap, n);
		util::construct_copy_n(begin(), begin2, copy_len);
		if (n == len) return;
		tail = util::addressof( begin() + copy_len );  //new tail
		if (n > cap) {
			node_type* new_head, new_tail;
			ccdk_increase_allocate_lst3(n - cap, new_head,new_tail,
				size_type new_cap);
			tail->next = new_head;
			tail = new_tail;
			util::construct_copy_n(iterator_type{new_head},
				begin2+copy_len, copy_len-cap);
			cap = cap + new_cap;
		}
		len = n;
	}

};

ccdk_namespace_ct_end