
#include<ccdk/container/container_mudule.h>

ccdk_namespace_ct_start

/* forwad link list node */
template<typename T>
struct forward_node {
	using value_type = T;
	T                value;
	forward_node<T> *next;
};

/* biward link list node */
template<typename T>
struct biward_node
{
	using value_type = T;
	T               value;
	biward_node<T> *prev;
	biward_node<T> *next;
};

/* avl tree node */
template<typename T>
struct avl_node 
{

};


/* rb tree node */
template<typename T>
struct rb_node
{
	using this_type = rb_node;
	using pointer   = rb_node*;
	enum color_t: uint8{ eRed, eBlack};

	T       value;
	pointer parent;     // to parent node
	pointer left;
	pointer right;
	color_t color;      // true:black, false: red
	
	CCDK_FORCEINLINE static pointer minimum(pointer node) noexcept {
		while (node->left) node = node->left;
		return node;
	}

	CCDK_FORCEINLINE static pointer maximum(pointer node) noexcept {
		while (node->right) node = node->right;
		return node;
	}
};
ccdk_namespace_ct_end


