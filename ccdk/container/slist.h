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
#include<ccdk/memory/simple_new_allocator.h>
#include<ccdk/memory/list_allocate_adapter.h>

#include<ccdk/algorithm/advance.h>
#include<ccdk/algorithm/distance.h>
#include<ccdk/container/slist_node.h>
#include<ccdk/container/container_mudule.h>

ccdk_namespace_ct_start

using namespace ccdk::mpl;

/* circle lazy single list:
   delete/pop node will not free/delete its memory, but link to tail->next to keep a cache
		and avoid frequency new/delete
   push/insert node will first utilize cached memory after tail
   shrink_to_fit will remain len * InceaseRatio nodes, free cap - len * InceaseRatio nodes
   shrink_to_len will remain len nodes, free the other cap - len nodes
*/
template<
	typename T, 
	typename Size = uint32,
	typename InceaseRatio = units::ratio<15, 10>,     /* preallocate 0.5  more times node space */
	typename Alloc = mem::simple_new_allocator< T >,
	typename Node = slist_node<T>
>
class slist: protected Alloc::template rebind<Node>
{
public:
	typedef slist                this_type;
	typedef slist                type;
	typedef Node                 node_type;

	/* common */
	typedef T                    value_type;
	typedef T*                   pointer_type;
	typedef T const*             const_pointer_type;
	typedef T&                   reference_type;
	typedef T const&             const_reference_type;
	typedef Size                 size_type;
	typedef ptr::diff_t          difference_type;
	typedef mem::list_allocate_adapter< typename Alloc::template rebind<Node>> allocate_type;

	/*iterator*/
	typedef iterator< node_type>       iterator_type;
	typedef iterator< const node_type> const_iterator_type;

	template<typename T2, typename Size2, typename Alloc2, typename Node2>
	friend class slist;

private:
	node_type* head; /* first useful node of list, use for push_front/ pop_front */
	node_type* tail; /* last  useful node of list, use for push_back  */
	size_type  len;  /* length of [head, tail] */
	size_type  cap;  /* keep cap - len nodes as cache to avoid allocate  */
	
public:
	/*default */
	CCDK_FORCEINLINE constexpr slist() noexcept 
		: head{ nullptr }, tail{ nullptr }, len{ 0 }, cap{ 0 } {}
	CCDK_FORCEINLINE constexpr slist(ptr::nullptr_t) noexcept 
		: head{ nullptr }, tail{ nullptr }, len{ 0 }, cap{ 0 } {}

	/* fill n */
	CCDK_FORCEINLINE slist(size_type n, T const& t = T()) { allocate_fill(n, t); }

	/* copy range [begin, begin+n)*/
	template<
		typename InputIt,
		typename = check_t< is_iterator<InputIt>>
	>
	CCDK_FORCEINLINE slist(InputIt begin, ptr::size_t n){
		ccdk_assert(n > 0);
		if (n>0) allocate_copy(n, begin);
	}

	/* copy range [begin, end)*/
	template<
		typename InputIt,
		typename = check_t< is_iterator<InputIt>>
	>
	CCDK_FORCEINLINE slist(InputIt begin, InputIt end){
		ptr::size_t n = alg::distance(begin, end);
		ccdk_assert(n > 0);
		if (n>0) allocate_copy(n, begin);
	}

	CCDK_FORCEINLINE slist(std::initializer_list<T> const& lst){
		if (lst>0) allocate_copy(lst.size() , lst.begin());
	}

	/* copy contructor*/
	CCDK_FORCEINLINE slist(slist const& other) {
		if (other.len > 0)  allocate_copy(other.len, other.begin() );
	}

	/* move constructor */
	CCDK_FORCEINLINE slist(slist && other)  noexcept
		: head{ other.head }, tail{ other.tail }, len{ other.len } { 
		other.rvalue_reset(); 
	}

	/* template copy constructor */
	template<typename Size2, typename Alloc2, typename Node2>
	CCDK_FORCEINLINE slist(slist<T,Size2,Alloc2,Node2> const& other) {
		if (other.len > 0)  allocate_copy(other.len, other.begin());
	}

	/* template move constructor */
	template<typename Size2, typename Alloc2, typename Node2>
	CCDK_FORCEINLINE slist(slist<T, Size2, Alloc2, Node2> && other) noexcept
		: head{ other.head }, tail{ other.tail }, len{ other.len } { 
		other.rvalue_reset();
	}

	CCDK_FORCEINLINE void swap(this_type& other) noexcept {
		util::swap(head, other.head);
		util::swap(tail, other.tail);
		util::swap(len, other.len);
	}
	
	this_type& operator=(ptr::nullptr_t) {
		util::destruct_n(begin(), len);
		tail = head; 
		len = 0;
	}

	this_type& operator=(this_type const& other) {
		ccdk_if_not_this(other) { assign_range(other.len, other.begin()); }
	}

	this_type& operator=(this_type && other) {
		ccdk_if_not_this(other) { this_type{ util::move(other) }.swap(*this); }
	}

	template<typename Size2, typename Alloc2, typename Node2>
	this_type& operator=(slist<T, Size2, Alloc2, Node2> const& other) {
		assign_range(other.len, other.begin());
	}

	template<typename Size2, typename Alloc2, typename Node2>
	this_type& operator=(slist<T, Size2, Alloc2, Node2> && other) {
		this_type{ util::move(other) }.swap(*this);
	}

	template<typename Size2, typename Alloc2, typename Node2>
	this_type& assign(slist<T, Size2, Alloc2, Node2> const& other) {
		ccdk_if_not_this(other) {
			assign_range(other.len, other.begin());
		}
	}

	template<typename Size2, typename Alloc2, typename Node2>
	this_type& assign(slist<T, Size2, Alloc2, Node2> && other) {
		ccdk_if_not_this(other) {
			this_type{ util::move(other) }.swap(*this);
		}
	}

	this_type& assign(size_type n, T const& t = T()) {
		util::destruct_n(begin(), len);
		tail = util::construct_fill_n(begin(), t, n);
		len = n;
	}

	template<typename ForwardIt>
	this_type& assign(ForwardIt begin, ForwardIt end) {
		util::destruct_n(begin(), len);
		tail = util::construct_fill_n(begin(), t, n);
		len = n;
	}

	CCDK_FORCEINLINE iterator_type begin() noexcept {
		return iterator_type{ head };
	}

	CCDK_FORCEINLINE iterator_type end() noexcept {
		return iterator_type{ nullptr };
	}

	CCDK_FORCEINLINE iterator_type cbegin() const noexcept {
		return const_iterator_type{ head };
	}

	CCDK_FORCEINLINE const_iterator_type& cend() const noexcept {
		static constexpr const_iterator_type ce{ nullptr };
		return ce;
	}

private:
	void rvalue_reset() noexcept {
		head = tail = nullptr;
		len = 0;
	}

	void destroy_content() {
		util::destruct_n(begin(), len);
		node_type* it = head;
		node_type* next = nullptr;
		while (it) {
			ptr::size_t count = 1;
			for (; it && it->next - it == sizeof(node_type); it = it->next; ++count);
			next = it->next;
			allocate_type::deallocate(*this, it, count);
			it = next;
		}
		head = tail = nullptr;
		len = 0;
	}

	void link_block_memory(size_type n, node_type* memory) const noexcept {
		for (size_type i = 0; i < n - 1; ++i) (memory + i)->next = (memory + i + 1);
	}

	template<typename InputIt>
	T* allocate_copy_link(size_type n, InputIt begin) {
		ccdk_assert(n > 0);
		node_type* memory = nullptr;
		if (n > 0) {
			memory = allocate_type::allocate(*this, n);
			util::construct_copy_n(memory, begin, n);
			link_block_memory(n, memory);
		}
		return memory;
	}

	T* allocate_fill_link(size_type n, T const& t) {
		node_type* memory = allocate_type::allocate(*this, n);
		util::construct_n<node_type>(memory, n, t);
		link_init_memory(n, memory);
		return memory;
	}

	void set_list_memory(T* memory, size_type n) noexcept {
		head = memory;
		tail = tail;
		len = n;
		cap = n;
	}


	void allocate_fill(size_type n, T const& t) {
		if (n > 0) set_list_memory(allocate_fill_link(n, t), n);
	}

	template<typename InputIt>
	void allocate_copy(size_type n, InputIt begin){
		if (n > 0) set_list_memory(allocate_copy_link(n, begin), n);
	}

	template<typename ForwardIt>
	void assign_range(size_type n, ForwardIt begin2) {
		ptr::size_t copy_len = fn::min(other.len, len);
		util::destruct_n(begin(), len);
		if (copy_len > 0) {
			util::construct_copy_n(begin(), begin2, copy_len);
			tail = util::addressof(*(begin() + copy_len - 1));
		}
		if (n > len) tail->next = allocate_copy_link(n - len, begin2 + len);
		len = n;
	}

};

ccdk_namespace_ct_end