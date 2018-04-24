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
	static constexpr map_type mapping{};  //mapping fn

	fs::local_obj<node_type> head;   // a empty node, pointer to root
	size_type				 len;    // length
	cmp_type				 cmp;    // cmp fn

	

	//handful function
	CCDK_FORCEINLINE link_type& root() noexcept { return head->parent; }
	CCDK_FORCEINLINE link_type& key_of(link_type node) noexcept { return node->value; }
	CCDK_FORCEINLINE link_type& left_most() noexcept { return head->left; }
	CCDK_FORCEINLINE link_type& right_most() noexcept { return head->right; }

	CCDK_FORCEINLINE static pointer min_node(pointer node) noexcept {
		while (node->left) node = node->left;
		return node;
	}

	CCDK_FORCEINLINE static pointer max_node(pointer node) noexcept {
		while (node->right) node = node->right;
		return node;
	}

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
		/*
			there are three case to choose delete node:
			if   prev-value-node exists, swap and delete it instead
			else next-value-node exists, swap and delete it instead
			else no left and right child exist,  delete node
		*/
		node_type node = it.content;
		node_type to_delete = node;
		uint32 pos = 0;    // delete node
		if (node->left) {
			to_delete = max_node(node->left);
			node->value = to_delete->value;
			pos = 1;    // delete left-biggest-node instead
		}
		//use right-next-node instead
		else if (node->right) {
			to_delete = min_node(node->right);
			node->value = to_delete->value;
			pos = 2;    //delete right-smallest-node instead
		}
		erase_at(to_delete, pos); //
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

	CCDK_FORCEINLINE auto erase_at(link_type node, uint32 pos) {
		/*
			there are 4 case of to_delete node
				case 1: to_delete node is red, delete directly, and link child( only one child exists )
				case 2: to_delete node is black and has one red child , delete and use child instead,
						turn child color to black
				case 3: to_delete node is black and has no child , have some sub-case:
					
					       *  parent( P )
					     /  \
		sibling( S )    *    *  to be delete( node )
				      /  \  /
				     *    * * child 
				 sibling child left(L) sibling child right(R)
					sub-case 1 : S black and  P red , switch S and P's color, if L and R is red turn black
					sub-case 2 : S red  and P black , rotate left around R, rotate right around R, turn  S red
					sub-case 3 : S black and P black ,L exists and is red, rotate right around S, turn L black
					sub-case 4 : S black and P black, S no child ,turn S red, rebalance from P to root
		*/

		// 0 or 1 child exists
		link_type child = nullptr;

		if (pos == 1) child = node->left;
		else if (pos == 2) child = node->right;

		//case 1
		if (node->is_red()) {
			if (child) child->parent = node->parent;
			node->parent = child;
			destroy_node(node);
		}
		else {
			// case 2
			if (child && child->is_red()) {
				child->parent = node->parent;
				node->parent = child;
				child->set_black();
				destroy_node(node);
			}
			// case 3
			else {
				link_type P = node->parent;
				link_type S = nullptr;
				bool node_is_left = false;
				if (node == P->left) {
					S = P->right;
					node_is_left = true;
				}
				else {
					S = P->left;
				}
				ccdk_assert(S);         // sibling must exists because node is black
				link_type L = S->left;  // sibling left child
				link_type R = S->right; // sibling right child
				//case 3.1 
				if (S->is_black() && P->is_red()) {
					S->set_red();
					P->set_black();
					if (L) { ccdk_assert(L->is_red()); L->set_black(); }
					if (R) { ccdk_assert(R->is_red()); R->set_black(); }
				}
				//case 3.2 
				else if (S->is_red() && P->is_black()) {
					ccdk_assert(L && R && L->is_black() && R->is_black());
					rotate_left(R, S);
					ccdk_assert(R->parent == P && R->left == S && S->left == L );
					rotate_right(R, P);
					ccdk_assert(R->left == S && R->right == P );
					L->set_red();
				}
				// case 3.3 
				else if (S->is_black() && P->is_black() && L) {
					ccdk_assert(L->is_red());
					rotate_right(S, P);
					ccdk_assert(S->left == L && S->right == P && P->left == R);
					L->set_black();
				}
				// case 3.4
				else {
					ccdk_assert(S->is_black() && P->is_black() && !L && !R);
					S->set_red();
					while (P != root()) {
						link_type G = P->parent;
						bool P_is_left = P == G->left;
						link_type PS = P_is_left ? G->right : G->left;  //parent-sibling

						if (PS) {
							if (P_is_left) rotate_left(PS, G);
							else rotate_right(PS, G);
							  
						}
						// PS not exists
						else {
							P = P->parent;
						}
					}
					root()->set_black();
				}
			}
		}
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
			root()->set_black();
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
		gp->parent = p;
		// child-brother to grand-pa's left
		gp->left = cb;
		cb->parent = gp;

		// parent's parent pointer to grand-grand-pa
		p->parent = ggp;
		if (ggp->left == gp) ggp->left = p;
		else ggp->right = p;
		
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
		gp->parent = p;
		// child-brother to grand-pa's left
		gp->right = cb;
		cb->parent = gp;

		// parent's parent pointer to grand-grand-pa
		p->parent = ggp;
		if (ggp->left == gp) ggp->left = p;
		else ggp->right = p;

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