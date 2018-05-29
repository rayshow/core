#pragma once

#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/util/move.h>
#include<ccdk/mpl/util/construct.h>
#include<ccdk/mpl/iterator/bstree_iterator.h>
#include<ccdk/mpl/iterator/reverse_iterator.h>
#include<ccdk/mpl/fusion/local_obj.h>
#include<ccdk/mpl/function/bind_mfn.h>
#include<ccdk/mpl/util/compare.h>
#include<ccdk/memory/allocator_traits.h>
#include<ccdk/memory/allocator/simple_new_allocator.h>
#include<ccdk/container/impl/link_node.h>
#include<ccdk/container/adapter/stack.h>
#include<ccdk/container/adapter/queue.h>

ccdk_namespace_ct_start

using namespace mpl;

template<
	bool AllowEqualKey,
	typename Key,
	typename MappedType,
	typename T,
	typename ExtractKey,
	typename ExtractMapped,
	typename Size = uint32,                               //
	typename Alloc = mem::simple_new_allocator<T,Size>,   //rb-node allocator
	typename Node = rb_node<T>
>
class rb_tree : protected Alloc::template rebind< Node >
{
	using this_type  = rb_tree;
	using node_type  = Node;
	using link_type  = node_type * ;
	using clink_type = node_type const*;
	using key_type   = Key;
	using mapped_type = MappedType;

	/* common */
	using value_type = T;
	using pointer = T * ;
	using const_pointer = T const*;
	using reference = T & ;
	using const_reference = T const&;
	using size_type = Size;
	using difference_type = ptr::diff_t;
	using allocator_type = mem::allocator_traits< 
		typename Alloc::template rebind< Node > >;

	//iterator
	using iterator = it::iterator< bstree_tag, Node>;
	using const_iterator = it::iterator< bstree_tag, const Node>;
	using reverse_iterator = it::reverse_iterator< iterator >;
	using const_reverse_iterator = it::reverse_iterator< const_iterator>;

private:
	static constexpr ExtractKey     ExtractKeyFn{};
	static constexpr ExtractMapped  ExtractMappedFn{};

	fs::local_obj<node_type> head;      // a empty node, pointer to root
	size_type				 len;       // length

	//handful function
	CCDK_FORCEINLINE clink_type end_node() const noexcept { return head.address(); }
	CCDK_FORCEINLINE link_type& root() noexcept { return head->parent; }
	CCDK_FORCEINLINE key_type&  KeyOfLink(link_type node) noexcept { return ExtractKeyFn(node->data); }
	CCDK_FORCEINLINE link_type& left_most() noexcept { return head->left; }
	CCDK_FORCEINLINE link_type& right_most() noexcept { return head->right; }

public:

	// de-ctor 
	CCDK_FORCEINLINE ~rb_tree() { if (len) clear(); }

	// default and nullptr ctor 
	CCDK_FORCEINLINE rb_tree()
		:len{ 0 }, head{} {
		init_head();
	}
	CCDK_FORCEINLINE rb_tree(ptr::nullptr_t)
		: len{ 0 }, head{} {
		init_head();
	}

	//range-ctor
	template<typename InputIt>
	CCDK_FORCEINLINE rb_tree(InputIt begin, InputIt end)
		: rb_tree{} {
		insert(begin, end);
	}

	//range-n ctor
	template<typename InputIt>
	CCDK_FORCEINLINE rb_tree(InputIt begin, size_type n)
		: rb_tree{} {
		insert(begin, n);
	}

	//copy ctor
	CCDK_FORCEINLINE rb_tree(rb_tree const& other)
		: rb_tree{ other.begin(), other.size() } {}

	//move ctor
	CCDK_FORCEINLINE rb_tree(rb_tree && other) noexcept
		: len{ other.size() } {
		rvalue_set(other.head);
		other.rvalue_reset();
	}

	//swap content
	CCDK_FORCEINLINE void swap(rb_tree& other) noexcept {
		util::swap(len, other.len);
		auto& other_head = other.head;
		link_type tmp = other_head->left;
		other_head->left = head->left;
		head->left = tmp;
		tmp = other_head->right;
		other_head->right = head->right;
		head->right = tmp;
		tmp = other_head->parent;
		other_head->parent = head->parent;
		head->parent = tmp;
		util::swap(len, other.len);
	}

	//emplace insert
	template<typename... Args>
	fs::pair<iterator, bool> emplace(Args&&... args) {
		link_type node = new_node(nullptr, util::forward<Args>(args)...);
		auto p = insert_impl(node);
		if (!p.second) destroy_node(node);
		return p;
	}

	// insert one node
	fs::pair<iterator, bool> insert(T const& t) { 
		link_type node = new_node(nullptr,t);
		auto p = insert_impl(node);
		//insert failed
		if (!p.second()) destroy_node(node);
		return p;
	 }

	// range insert
	template< 
		typename InputIt,
		typename = check_t< is_iterator<InputIt>> >
	CCDK_FORCEINLINE void insert(InputIt begin, InputIt end) {
		for (InputIt it = begin; it != end; ++it) {
			insert(*it);
		}
	}
	
	// range-n insert
	template< 
		typename InputIt,
		typename = check_t< is_iterator<InputIt>> >
	CCDK_FORCEINLINE void insert(InputIt begin, size_type n) {
		for (size_type i = 0; i<n; ++i,++begin) {
			insert(*begin);
		}
	}

	// delete one node
	void erase(const_iterator it) noexcept { erase_impl(it); }
	void erase(key_type const& key) noexcept { 
		link_type begin = lower_bound_impl(key);
		auto p = equal_range_impl(begin, key);
		erase({ begin }, { p.first });
		return p.second;
	}
	void erase(const_iterator begin, const_iterator end) noexcept {
		for (const_iterator it = begin; it != end; ++it) {
			erase(it);
		}
		return end;
	}

	void clear() {
		link_type node = root();
		while (node!= end_node()) {
			if (!node->left && !node->right) {
				link_type parent = node->parent;
				if (node == parent->left) {
					parent->left = nullptr;
				}
				else {
					parent->right = nullptr;
				}
				destroy_node(node);
				node = parent;
			}
			else if(node->left) {
				node = node->left;
			}
			else {
				node = node->right;
			}
		}
		init_head();
		len = 0;
	}

	// find by Key
	void find(Key const& key) { return find_impl(key); }

	CCDK_FORCEINLINE const_iterator lower_bound(Key const& key) const noexcept {
		return { lower_bound_impl(key) };
	}

	CCDK_FORCEINLINE const_iterator upper_bound(Key const& key) const noexcept {
		return { equal_range_impl(lower_bound_impl(key), key).first };
	}

	CCDK_FORCEINLINE fs::pair<const_iterator, const_iterator>
		equal_range(Key const& key) const noexcept {
		link_type begin = lower_bound_impl(key);
		link_type end = equal_range_impl(begin, key).first;
		return { { begin },{ end } };
	}

	//iterator
	CCDK_FORCEINLINE iterator begin() noexcept { 
		ccdk_assert(len);  return { head->left }; 
	}
	CCDK_FORCEINLINE iterator end() noexcept { 
		ccdk_assert(len);  return { head.address() };
	}
	CCDK_FORCEINLINE const_iterator cbegin() const noexcept { 
		ccdk_assert(len);  return { head->left }; 
	}
	CCDK_FORCEINLINE const_iterator cend() const noexcept {
		ccdk_assert(len);  return { head.address() }; 
	}
	CCDK_FORCEINLINE reverse_iterator rbegin() noexcept {
		ccdk_assert(len);  return { { head->right } };
	}
	CCDK_FORCEINLINE reverse_iterator rend() noexcept {
		ccdk_assert(len);  return { { head.address() } };
	}
	CCDK_FORCEINLINE const_reverse_iterator crbegin() const noexcept {
		ccdk_assert(len);  return { { head->right } };
	}
	CCDK_FORCEINLINE const_reverse_iterator crend() const noexcept {
		ccdk_assert(len);  return { { head.address() } };
	}

	//readonly attribute fn
	CCDK_FORCEINLINE size_type count(Key const& key) const noexcept {
		return equal_range_impl(lower_bound_impl(key), key).second;
	}

	CCDK_FORCEINLINE size_type size() const noexcept { return len;}
	CCDK_FORCEINLINE bool empty() const noexcept { return len == 0; }

	
////////////////////////////////////////////////////////////////////
//// transform / foreach

	// inorder visit
	template<typename Fn>
	CCDK_FORCEINLINE void foreach(Fn fn) {
		// more than 2^64 node may over local stack's stack size, but heap will hold it 
		local_stack<link_type, 128> stack;
		link_type node = root();
		while (node || !stack.empty()) {
			while (node) {
				stack.push(node);
				node = node->left;
			}
			node = stack.top();
			stack.pop();
			fn(node->data);
			node = node->right;
		}
	}

	 
	CCDK_FORCEINLINE void preorder(link_type node = nullptr) {
		queue<fs::pair<int,link_type>> que{};
		que.push({ 1,root() });
		int prev = 1;
		int space = 40;
		while (!que.empty())
		{
			auto p = que.front();
			que.pop();
			
			if (p.first() != prev) {
				DebugValueItEnd();
				prev = p.first();
			}
			if (p.second()) {
				std::cout << p.second()->data.first();
				if (p.second()->is_red()) {
					std::cout << "(R)";
				}
				else {
					std::cout << "(B)";
				}
				std::cout << "  ";
				que.push({ p.first() + 1, p.second()->left });
				que.push({ p.first() + 1, p.second()->right });
			}
			else {

				DebugValueIt("N(B)");
				std::cout << "  ";
			}
			
			
		}
		DebugValueItEnd();
	}

	CCDK_FORCEINLINE void print_tree() {
		preorder(root());
	}
//// implements 
private:

	CCDK_FORCEINLINE void rvalue_set(fs::local_obj<node_type> & other) noexcept {
		head->parent = other->parent;
		head->left = other->left;
		head->right = other->right;
	}

	CCDK_FORCEINLINE void rvalue_reset() noexcept {
		init_head();
		len = 0;
	}

	CCDK_FORCEINLINE  void init_head() noexcept {
		head->left =  head.address();
		head->right = head.address();
		head->parent = nullptr;
		head->set_black();
	}

	CCDK_FORCEINLINE static link_type min_node(link_type node) noexcept {
		while (node->left) node = node->left;
		return node;
	}

	CCDK_FORCEINLINE static link_type max_node(link_type node) noexcept {
		while (node->right) node = node->right;
		return node;
	}

	CCDK_FORCEINLINE link_type prev(link_type node) noexcept {
		if (node->left) return max_node(node->left);
		while (node != root()) {
			link_type parent = node->parent;
			if (node == parent->right) return parent;
			node = parent;
		}
		return nullptr;
	}

	CCDK_FORCEINLINE link_type next(link_type node) noexcept {
		if (node->right) return min_node(node->right);
		while (node != root()) {
			link_type parent = node->parent;
			if (node == parent->left) return parent;
			node = parent;
		}
		return nullptr;
	}

	CCDK_FORCEINLINE clink_type lower_bound_impl(key_type const& key) const noexcept {
		clink_type node = root();
		bool find = false;
		while (node) {
			key_type& node_key = KeyOfLink(node);
			// key < node_key
			if (KeyCmpFn(key, node_key)) {
				node = node->left;
			}
			//node_key < key
			else if (KeyCmpFn(node_key, key)) {
				node = node->right;
			}
			else {
				find = true;
				break;
			}
		}
		if (!find) return end_node();
		return node;
	}

	CCDK_FORCEINLINE fs::pair<clink_type,size_type>
		equal_range_impl(clink_type node, key_type const& key) const noexcept {
		//key >= node
		ccdk_assert(!KeyCmpFn(key, KeyOfLink(node)) && node);
		while (node != end_node()) {
			if (KeyCmpFn(KeyOfLink(node), key)) break;
			node = node->next();
			++count;
		}
		return { node, count };
	}

	template<typename... Args>
	CCDK_FORCEINLINE link_type new_node(link_type parent, Args&&... args) {
		link_type new_node = allocator_type::allocate(*this, 1);
		// value is in the front of Node Type, so just use node's address 
		util::construct<value_type>(new_node, util::forward<Args>(args)...);
		new_node->left = nullptr;
		new_node->right = nullptr;
		//new_node except root is always red
		new_node->set_red();
		new_node->parent = parent;
		return new_node;
	}

	// destruct one node and delete memory
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
					
					 C on the right

					       P  parent( P )
					     /  \ 
		      Sibling   S    C  delete node's child or up-tracking unbalance-branch( S has 1 more Black node than C )
				      /  \ 
				     O    I
				   S's outer child and inner child

					 1) I is Red => IL/IR is Black, balanced end
					             
								 R     P 's color
							   /   \
							  S     P   Black
							 /	\  /  \
						    L   IL IR  C

					 2) I is Black(include nullptr), P is Red =>  S Black, O unkown
						      (1) O is Black(nullptr at some time with I) switch P and S's color,  balanced end
							  (2) O is Red 
					        
									  S   Black
									/   \ 
								   O     P  Red
								Black   /  \
									   I    C
									  Black Black

									  balanced end

					 3) I is Black, P is Black, S is Red => O is Black
                               rotate left around P, swap P and S 's color

						   	  S   Black
							/   \
						   O     P  Red
						Black   /  \
							   I    C 
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
			} else {
				ccdk_assert(!C);
				bool C_is_left = D_is_left;
				link_type I = nullptr;  // innser child of S
				link_type O = nullptr;  // out child of S
				void(this_type::*rotate_down)(link_type, link_type) = nullptr;
				void(this_type::*rotate_up)(link_type, link_type) = nullptr;
				link_type S = nullptr;

				// case 3
				while (P && P != head->address())
				{
					if (C_is_left) {
						S = P->right;
						ccdk_assert(S);  //S must exists
						I = S->left;
						O = S->right;
						rotate_down = &this_type::rotate_left;
						rotate_up = &this_type::rotate_right;
					}
					else {
						S = P->left;
						ccdk_assert(S);  //S must exists
						I = S->right;
						O = S->left;
						rotate_down = &this_type::rotate_right;
						rotate_up = &this_type::rotate_left;
					}
					//case 3.1 
					if (I && I->is_red()) {
						// S is black
						ccdk_assert(S->is_black());
						(this->*rotate_up)(I, S);
						(this->*rotate_down)(I, P);
						break; //balanced , end
					}
					else if(P->is_red()){
						// I is black, S is black
						ccdk_assert( S->is_black());
						//case 3.2.2
						if (I && I->is_red()) {
							(this->*rotate_up)(S, P);
						}
						//case 3.2.1
						else {
							// O is black
							P->set_black();
							S->set_red();
						}
						break; //balanced , end
					}
					// case 3.3
					else if (S->is_red()) {
						//I/O is black, P is black, S Red
						(this->*rotate_up)(S, P);
						P->set_red();
						S->set_black();
						//now S's child is un-balance and P is red, loop while goto 3.2
					}
					else {
						// S/P/I/O/C all Black
						ccdk_assert(S->is_black());
						S->set_red();
						//now P is balance, but P'parent is un-balance, goto case 3
						C = P;
						P = P->parent;
						C_is_left = C == P->left;
					}
				}//while

			}//else case2

		}//else case 1

		root()->set_black();
	}

	// do actually erase
	CCDK_FORCEINLINE void erase_impl(const_iterator it)
	{
		/*
			there are three case to choose delete node:
			if   prev-value-node exists, swap and delete it instead
			else next-value-node exists, swap and delete it instead
			else no left and right child exist,  delete node
		*/
		node_type node =  it.content;
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

	// do actually insert 
	CCDK_FORCEINLINE fs::pair<iterator,bool> 
		insert_at(link_type parent, link_type new_node, bool insert_at_left) 
	{
		new_node->parent = parent;
		
		// key of new node < key of parent, insert to left
		if (insert_at_left) {
			if (parent->left) {
				int a = 0;
			}
			ccdk_assert(parent->left == nullptr);
			parent->left = new_node;
			if (parent == left_most()) {
				left_most() = new_node;
			}
		}
		// key of t > key of parent, insert to right
		else {
			ccdk_assert(parent->right == nullptr);
			// insert to right
			parent->right = new_node;
			// new node is the new right most
			if (parent == right_most()) {
				right_most() = new_node;
			}
		}
		// insert at root's child, no need rebalance
		if (parent != root()) {
			rebalance_at(parent, new_node);
		}
		++len;
		return { {new_node}, true };
	}

	CCDK_FORCEINLINE fs::pair<iterator,bool>
		insert_impl(link_type new_node) {

		//root is empty 
		if (root() == nullptr) {
			ccdk_assert(root() == nullptr);
			new_node->parent = head.address();
			//root is always black
			new_node->set_black();
			head->set_pointer(new_node, new_node, new_node);
			++len;
			return { { new_node }, true };
		}

		link_type parent = head.address();
		link_type child = root();

		key_type new_key = KeyOfLink(new_node);
		bool insert_at_left = false;
		while (child) {
			parent = child;
			//t.key > child.key ?
			insert_at_left = util::compare(new_key, KeyOfLink(child));
			child = insert_at_left ? child->left : child->right;
		}

		//if allow multi-common-keys, just insert and return
		if (AllowEqualKey) {
			return insert_at(parent, new_node, insert_at_left);
		}

		// if insert right, parent need greater than t
		// else prev-node need greater than t
		link_type prev_insert = parent;
		//greater than parent 
		if (insert_at_left) {
			if (parent == left_most()) {
				return insert_at(parent, new_node, true);
			}
			else {
				prev_insert = prev(parent);
				//if parent is not the left-most node, prev exists
				ccdk_assert(prev_insert);
			}
		}

		// check again, prev node must greater than new_node
		if (util::compare(KeyOfLink(prev_insert), new_key)) {
			return insert_at(parent, new_node, insert_at_left);
		}
		//here insert failed with not-unique-key
		return { {prev_insert}, false };
	}

	// 
	void rebalance_at(link_type P, link_type C) {
		/*
			   G  grand-parent
			 /  \
			P    S  parent( P ) and parent-sibling( S )
		   /  \
		  C    C  child ( C left or right ) C's sibling is CS

			there are  cases:
				case 1: P black, no need process
				case 2: P on the left and is Red => G is Black
					sub-case 1: S red
						turn P and S black, G red, recursive justify G as C
					sub-case 2: S black => G black, S=Nil,CS=Nil
						insert at left(Red) rotate right, switch P and G's color
						        C   Black
                              /   \ 
						Red  P     G  Red
							     
					sub-case 3: P red, G black, insert right => CS is Nil, S is Nil
						rise C to G, and P as C's left and G as C's right
							   C  Black
                             /   \
						Red P     G  Red
				case 3: P on the right, mirror with on the left
		*/

		// case 1
		if (P->is_black()) return;
		
		// P is red, if is black goto case 1 stop
		while ( P !=root() && P->is_red() && P != head.address() ) {
			link_type G = P->parent;
			bool parent_on_left = P == G->left;
			link_type S = parent_on_left ? G->right : G->left;
			bool S_is_red = S && S->is_red();

			//case 2.1  / 3.1
			if (S_is_red) {
				P->set_black();
				S->set_black();
				G->set_red();
				//recursive to head
				C = G;
				P = C->parent;
			}
			// case 2.2 2.3 3.2 3.3
			else {
				bool child_on_right = C == P->right;
				//S is black and P is Red + P no child => S is Nil, CS is Nil, G is red
				if (S != nullptr) {
					int a = 0;
				}
				ccdk_assert(P->is_red());
				ccdk_assert(G->is_black());
				if (parent_on_left) {
					if (child_on_right) {
						// case 2.3
						link_type CL = C->left;
						link_type CR = C->right;
						link_type GP = G->parent;

						//C is red so child must be black
						ccdk_assert(!CL || CL->is_black());  
						ccdk_assert(!CR || CR->is_black());
						C->set_pointer(GP, P, G);
						P->set_parent_right(C, CL);
						G->set_parent_left(C, CR);
						C->set_black();
						G->set_red();
					}
					else {
						// case 2.2 visited
						rotate_right(P, G);
					}
				}
				else {
					// case 3.3 mirror with 2.3
					if (!child_on_right) {
						link_type CL = C->left;
						link_type CR = C->right;
						link_type GP = G->parent;

						//C is red so child must be black
						ccdk_assert(!CL || CL->is_black());
						ccdk_assert(!CR || CR->is_black());
						C->set_pointer(GP, G, P);
						P->set_parent_left(C, CR);
						G->set_parent_right(C, CL);
						C->set_black();
						G->set_red();
					}
					else {
						// case 3.2 mirror with 2.2
						rotate_left(P, G);
					}
				}
			} 
		}// while
		root()->set_black();
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
		if (GG == head.address()) {
			GG->parent = P;
		}
		else {
			if (GG->left == G) GG->left = P;
			else GG->right = P;
		}
		
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
		if (GG == head.address()) {
			GG->parent = P;
		}
		else {
			if (GG->left == G) GG->left = P;
			else GG->right = P;
		}

		//parent switch to plack
		P->set_black();
		//grand-pa switch to red
		G->set_red();
	}
};


ccdk_namespace_ct_end