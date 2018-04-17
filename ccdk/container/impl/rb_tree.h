#pragma once

#include<ccdk/mpl/fusion/local_obj.h>
#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/mcontainer/val_pack.h>
#include<ccdk/mpl/iterator/reverse_iterator.h>
#include<ccdk/mpl/util/move.h>
#include<ccdk/mpl/util/construct.h>
#include<ccdk/mpl/function/operator.h>
#include<ccdk/mpl/fusion/local_obj.h>
#include<ccdk/memory/allocator_traits.h>
#include<ccdk/memory/simple_new_allocator.h>
#include<ccdk/container/impl/link_node.h>

ccdk_namespace_ct_start

// just call util::cmp

template<typename T>
struct default_cmp {

};

template<
	typename T,
	typename Size = uint32,
	typename Alloc = mem::simple_new_allocator<T>,
	typename Cmp  = default_cmp<T>,
	typename Node = rb_node<T>
>
class rb_tree: protected Alloc::rebind< Node >
{
	using this_type = rb_tree;
	using node_type = Node;
	using link_type = node_type *;
	using cmp_type  = cmp_type;

	/* common */
	using value_type      = T;
	using pointer         = T * ;
	using const_pointer   = T const*;
	using reference       = T & ;
	using const_reference = T const&;
	using size_type       = Size;
	using difference_type = ptr::diff_t;
	using allocator_type  = mem::allocator_traits< typename Alloc::rebind< Node > >;

private:
	fs::local_obj<node_type> head;   // a empty node
	size_type				 len;    // length
	cmp_type				 cmp;    // cmp fn

	link_type root() noexcept { return head->parent; }
	const link_type root() const noexcept { return head->parent; }
public:

	/* de-ctor */

	/* default and nullptr ctor */
	CCDK_FORCEINLINE rb_tree() :len{ 0 }, head{}, cmp{} {}
	CCDK_FORCEINLINE rb_tree(ptr::nullptr_t) : len{ 0 }, head{}, cmp{} {}

	/* with cmp */
	CCDK_FORCEINLINE explicit rb_tree( cmp_type cmp ):len{0}, head{nullptr}, cmp{cmp}{}

	/* range */
	template<typename InputIt>
	CCDK_FORCEINLINE rb_tree(InputIt begin, InputIt end) {
		allocate_copy(begin, util::distance(begin, end));
	}

private:

	CCDK_FORCEINLINE  void init_head() noexcept {
		head->left =  head.address();
		head->right = head.address();
		head->parent = nullptr;
	}
	

	void local_allocate_n(size_type n)  {

	}

	void insert() {

	}

	template<typename InputIt>
	CCDK_FORCEINLINE void allocate_copy(InputIt begin, size_type n) {

	}
};


ccdk_namespace_ct_end