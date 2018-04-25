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
			//get pre-node 
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
		erase_at(to_delete, pos); 
	}

private:

	CCDK_FORCEINLINE  void init_head() noexcept {
		head->left =  head.address();
		head->right = head.address();
		head->parent = nullptr;
		head->set_black();
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

	CCDK_FORCEINLINE void destroy_node(link_type node) noexcept {
		util::destruct<T>(node);
		allocator_type::deallocate(*this, node, 1);
	}

	// delete node with single child or no child
	//   D    : to be delete node
	//   pos  : 0 mean node has no prev-node or next-node ,
	//          1 mean use prev-node instead, so may has left child
	//          2 mean use next-node instead, so may has right child
	CCDK_FORCEINLINE auto erase_at(link_type D, uint32 pos) {
		/*
			there are 3 case of to_delete node
				1: to_delete node is red( no child), delete directly, no child exists
				2: to_delete node is black and has one red child , delete and use child instead,
						turn child color to black
				3: to_delete node is black and has no child , have some sub-case and recursive :
					
					 C on the left

					       P  parent( P )
					     /  \ 
		      Sibling   S    C  delete node's child or up-tracking unbalance-branch( S has 1 more Black node than C )
				      /  \ 
				     L    R

					 1) R is Red => RL/RR is Black
					             
								 R     P 's color
							   /   \
							  S     P   Black
							 /	\  /  \
						    L   RL RR  C

					 2) R is Black, P is Red =>  S Black, L unkown
						      L is Black switch P and S's color, end
							  L is Red 
					        
							  S   Black
							/   \
						   L     P  Red
						Black   /  \
							   R    C 
						      Black

					 3) R is Black, P is Black, S is Red => L is Black
                               rotate left around P, swap P and S 's color

						   	  S   Black
							/   \
						   L     P  Red
						Black   /  \
							   R    C 
						      Black  
									 
						now P is un-balance , process as 2) 

					 4) R is Black, P is Black, S is Black, L/R is Black
					        turn S Red, now  P's parent( stop at root) is un-banlance, goto case 3.

					 
		*/ 

		ccdk_assert(node);

		// delete node's child, 0 or 1 child exist
		link_type C = nullptr;
		if (pos == 1) C = D->left;
		else if (pos == 2) C = D->right;
		
		// keep parent, branch and color
		link_type P = D->parent;
		bool D_is_left = D == P->left;
		bool D_is_red = D->is_red();

		// delete D
		destroy_node(D);

		//case 1
		if (D_is_red) {
			//D is red leaf, reset P's pointer
			ccdk_assert(!D->left && !D->right);
			if (D_is_left) P->left = nullptr;
			else P->right = nullptr;
		}
		else {
			ccdk_assert(D->is_black());

			// case 2
			if (C && C->is_red()) {
				// D has one red child C, C and P pointer to each other, turn C to Black
				ccdk_assert(!C->left && !C->right);
				C->parent = P;
				if (D_is_left) P->left = C;
				else P->right = C;
				C->set_black();
			}
			// case 3
			else {
				ccdk_assert(!C);
				bool C_is_left = D_is_left;

				while (P && P != root())
				{
					// sibling of Current process node
					link_type S = nullptr;
					if (C_is_left) S = P->right;
					else S = P->left;
					ccdk_assert(S);         // sibling must exists because Current process node is Black
					link_type L = S->left;  // sibling left child
					link_type R = S->right; // sibling right child



					//case 3.1 
					if (R && R->is_red()) {
						if (C_is_left) {
							rotate_left(R, S);
						}
					}

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

	CCDK_FORCEINLINE void rotate_right(link_type P, link_type G) noexcept {

		/*
				GG                GG
				|                 |
				G         =>      P
			  /   \             /   \
			  P     S          C     G
			/  \                   /   \
			C   CS                CS    S

			 GG : grand-grand-pa 
			 G  : grand-pa
			 P  : Parent
			 C  : Child
			 S  : Parent Sibling
			 CS : child Sibling
		*/
		ccdk_assert(P && G);
		ccdk_assert(P == G->left);

		//child brother 
		link_type CS = P->right;
		//grand-grand-pa
		link_type GG = G->parent;

		// grand-pa to parent's right
		P->right = G;
		G->parent = P;
		// child-sibling to grand-pa's left
		G->left = CS;
		if(CS) CS->parent = G;

		// parent pointer to grand-grand-pa
		P->parent = GG;
		if (GG->left == G) GG->left = P;
		else GG->right = P;
		
		//parent switch to black
		P->set_black();
		//grand-pa switch to red
		G->set_red();
	}


	CCDK_FORCEINLINE void rotate_left(link_type P, link_type G) noexcept {


		/*
			  GG                GG
			  |                 |
			  G         =>      P
			/   \             /   \
			S    P           G     C
			   /  \         / \     
			  CS   C       S   CS    

			GG : grand-grand-pa
			G  : grand-pa
			P  : Parent
			C  : Child
			S  : Parent Sibling
			CS : child Sibling
		*/
		ccdk_assert(P && G);
		ccdk_assert(P == G->right);

		//child sibling
		link_type CS = P->left;
		//grand-grand-pa
		link_type GG = G->parent;

		// grand-pa to parent's right
		P->left = G;
		G->parent = P;
		// child-brother to grand-pa's left
		G->right = CS;
		if(CS) CS->parent = G;

		// parent's parent pointer to grand-grand-pa
		P->parent = GG;
		if (GG->left == G) GG->left = P;
		else GG->right = P;

		//parent switch to plack
		P->set_black();
		//grand-pa switch to red
		G->set_red();
	}

	template<typename InputIt>
	CCDK_FORCEINLINE void allocate_copy(InputIt begin, size_type n) {

	}
};


ccdk_namespace_ct_end