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

using namespace mpl;

// just call util::cmp

template<typename T>
struct default_cmp {

};

template<typename T>
struct default_to_key {
	operator()
};

template<
	typename Key,
	typename T,
	typename Size = uint32,
	typename Alloc = mem::simple_new_allocator<T>,
	typename MapToKey = default_to_key<T>,
	typename Cmp  = default_cmp<T>,
	typename Node = rb_node<T>
>
class rb_tree: protected Alloc::rebind< Node >
{
	using this_type  = rb_tree;
	using node_type  = Node;
	using link_type  = node_type *;
	using key_type   = Key;
	using cmp_type   = Cmp;
	using map_type   = MapToKey;

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
	fs::local_obj<node_type> head;   // a empty node, pointer to root
	size_type				 len;    // length
	cmp_type				 cmp;    // cmp fn

	static constexpr map_type mapping{};  //mapping fn

	//handful function
	link_type& root() noexcept { return head->parent; }
	link_type& key_of(link_type node) noexcept { return node->value; }
	link_type& left_most() noexcept { return head->left; }
	link_type& right_most() noexcept { return head->right; }

public:

	/* de-ctor */

	/* default and nullptr ctor */
	CCDK_FORCEINLINE rb_tree() :len{ 0 }, head{}, cmp{} { init_head(); }
	CCDK_FORCEINLINE rb_tree(ptr::nullptr_t) 
		: len{ 0 }, head{}, cmp{} { init_head(); }

	/* with cmp */
	CCDK_FORCEINLINE explicit rb_tree(cmp_type cmp)
		:len{ 0 }, head{}, cmp{ cmp } { init_head(); }


	void insert(T const& t) {
		link_type parent = head.address();
		link_type child = root();
		auto map_to = map_type{};
		//
		while (child) {
			parent = child;
			child = cmp(mapping(t), key_of(child)) ? left(child) : right(child);
		}
		return insert_at(parent, t);
	}

	
	void erase(const_iterator it) {

	}


private:

	CCDK_FORCEINLINE  void init_head() noexcept {
		head->left =  head.address();
		head->right = head.address();
		head->parent = nullptr;
		head->color = node_type::kBlack;
	}

	CCDK_FORCEINLINE link_type create_node(T const& t, link_type parent) {
		link_type new_node = allocator_type::allocate(*this, 1);
		// value is in the front of Node Type, so just use node's address 
		util::construct(new_node, t);
		new_node->left = nullptr;
		new_node->right = nullptr;
		//new_node except root is always red
		new_node->set_red();
		new_node->parent = parent;
		return new_node;
	}

	//https://www.cnblogs.com/tongy0/p/5460623.html
	CCDK_FORCEINLINE auto erase_at(link_type node) {

	}


	template<bool AllocEqual>
	CCDK_FORCEINLINE auto insert_at(link_type parent, T const & t) {
		// compare value with parent node value
		bool greater = cmp(mapping(t), key_of(node));

		// find equal key and not allow equal key, return false
		if (!greater && !cmp(key_of(node), mapping(t)) && !AllocEqual ) {
			return fs::make_pair(parent, false);
		}

		link_type new_node = create_node(t, parent);
		//root is empty 
		if ( parent == head.address()) {
			ccdk_assert(root() == nullptr);
			root() = new_node;
			//root is always black
			new_node->set_black();  
			left_most() = new_node;
			right_most() = new_node;
		}
		// key of t > key of parent, insert to right
		else if (greater) {
			ccdk_assert(parent->right == nullptr);
			// insert to right
			parent->right = new_node;
			// new node is the new right most
			if (parent == right_most()) {
				right_most() = new_node;
			}
		}
		// key of t < key of parent, insert to left
		else{
			ccdk_assert(parent->left == nullptr );
			parent->left = new_node;
			if (parent == left_most()) {
				left_most() = new_node;
			}
		}
		// insert at root's child, no need rebalance
		if (parent != root()) {
			rebalance_at(parent, new_node)
		}
		return fs::make_pair(new_node, true);
	}


	// 
	void rebalance_at(link_type p, link_type c, link_type gp) {
		/*
			  *  grand-parent( gp )
			/  \
			*  *  parent( p ) and parent-brother( pb )
			/ \
			child ( c ) and child-brother

			there are  cases:
				case 1: p black, no need process
				case 2: p on the left
					sub-case 1: p red, pb red
						turn p and pb black, gp red, recursive justify  gp as c
					sub-case 2: p red, pb black, insert left
						rotate right, switch p and gp's color
					sub-case 3: p red, pb black, insert right
						

				case 3: p on the right, mirror with on the left
		*/

		// case 1
		if (p->is_black()) return;
		
		while (  p!=root() && p->is_red() && p != head.address() ) {
			bool parent_on_left = p == gp->left;
			bool child_on_right = c == p->right;
			link_type pb = parent_on_left ? gp->right : gp->left;
			bool pb_is_red = pb && pb->is_red();

			//case 2.1  / 3.1
			if (pb_is_red) {
				p->set_black();
				pb->set_black();
				gp->set_red();
				c = gp;
				p = c->parent;
				gp = p->parent;
			}
			// case 2
			else if (parent_on_left) {
				if (child_on_right) {
					// case 2.3
					rotate_left(c, p);
					util::swap(c, p);
				}
				// case 2.2
				rotate_right(p, gp);
			}
			// case 3 mirror with case 2
			else {
				if (!child_on_right) {
					// case 2.3
					rotate_right(c, p);
					util::swap(c, p);
				}
				// case 2.2
				rotate_left(p, gp);
			}
		}// while
	}

	CCDK_FORCEINLINE void rotate_right(link_type p, link_type gp) noexcept {

		/*
							  *  grand-grand-pa( ggp )             * ( ggp )
							  |                                    |
							  *  grand-parent( gp )         =>     *  ( p )
							/   \                                /   \
			  parent( p )  *    *   parent-brother( pb )    (c) *     * ( gp )
						 /   \                                      /   \
			child ( c ) *     *  child-brother(cb)            (cb) *     * ( pb )

			 orginal: c,p  = red  cb, gp, pb = black
			 new    : c,gp = red  p,  cb, pb = black

		*/

		//child brother 
		link_type cb = p->right;
		//grand-grand-pa
		link_type ggp = gp->parent;

		// grand-pa to parent's right
		p->right = gp;
		// child-brother to grand-pa's left
		gp->left = cb;
		// parent's parent pointer to grand-grand-pa
		p->parent = ggp;
		//parent switch to plack
		p->set_black();
		//grand-pa switch to red
		gp->set_red();
	}


	CCDK_FORCEINLINE void rotate_left(link_type p, link_type gp) noexcept {

		/*
						  *  grand-grand-pa( ggp )             * ( ggp )
						  |                                    |
						  *  grand-parent( gp )         =>     *  ( p )
						/   \                                /   \
 parent-brother( pb )  *    *   parent( p )            (gp) *     * ( c )
                          /   \                           /   \
     child-brother ( cb ) *     *  child(c)         (pb) *     * ( cb )

				orginal: c,p  = red  cb, gp, pb = black
				new    : c,gp = red  p,  cb, pb = black

		*/

		//child brother 
		link_type cb = p->left;
		//grand-grand-pa
		link_type ggp = gp->parent;

		// grand-pa to parent's right
		p->left = gp;
		// child-brother to grand-pa's left
		gp->right = cb;
		// parent's parent pointer to grand-grand-pa
		p->parent = ggp;
		//parent switch to plack
		p->set_black();
		//grand-pa switch to red
		gp->set_red();
	}

	
	template<typename InputIt>
	CCDK_FORCEINLINE void allocate_copy(InputIt begin, size_type n) {

	}
};


ccdk_namespace_ct_end