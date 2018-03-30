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
#include<ccdk/container/container_mudule.h>

ccdk_namespace_ct_start

using namespace ccdk::mpl;

template<typename T>
struct bnode 
{
	T         value;
	bnode<T> *prev;
	bnode<T> *next;
};

template<
	typename T,
	typename Size = uint32,
	typename Alloc = mem::simple_new_allocator< T >,
	typename Node = bnode<T>
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
	typedef mem::list_allocate_adapter< typename Alloc::template rebind<Node>>  allocator_type; //allocate/delete multi-node

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
		destruct_content(); len = 0;
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

	// template copy ctor
	template<typename Size2>
	CCDK_FORCEINLINE explicit blist(blist<T,Size2,Alloc,Node> const& other) {
		allocate_copy(other.begin(), other.size());
	}




	CCDK_FORCEINLINE size_type size() const noexcept { return len;  }

	CCDK_FORCEINLINE iterator_type begin() noexcept { return { head }; }
	CCDK_FORCEINLINE iterator_type end() noexcept { return { head }; }



private:

	// destruct objects in list
	void destruct_content() noexcept {
		if (len > 0) util::destruct_n(begin(), len);
	}

	node_type* allocate_n(size_type n) {
		node_type *new_head, *new_tail;
		fs::tie(new_head, new_tail) = allocator_type::allocate(*this, n);
		new_tail->next = new_head;
		new_head->prev = new_tail;
		return new_head;
	}

	// allocate n elements and fill with t
	void allocate_fill(size_type n, T const& t) {
		if (n == 0) return;
		content = allocate_n(n);
		ccdk_safe_cleanup_if_exception(
			util::construct_fill_n(iterator_type{ content }, t, n),   //may throw if copy-ctor 
			allocator_type::deallocate(*this, content, n));        //delete content 
		len = n;
	}

	// allocate n elements and copy from [beginIt, beginIt+len)
	void allocate_copy(InputIt beginIt, size_type n) {
		if (n == 0) return;
		content = allocate_n(n);
		ccdk_safe_cleanup_if_exception(
			util::construct_copy_n(iterator_type{ content }, beginIt, n),   //may throw if copy-ctor 
			allocator_type::deallocate(*this, content, n));        //delete content 
		len = n;
	}
};
ccdk_namespace_ct_end