
#include<ccdk/container/container_mudule.h>

ccdk_namespace_ct_start

/* forwad link list node */
template<typename T>
struct forward_node {
	using value_type = T;
	T                value;
	forward_node<T> *next;
};

template<typename T>
struct biward_node
{
	using value_type = T;
	T               value;
	biward_node<T> *prev;
	biward_node<T> *next;
};
ccdk_namespace_ct_end


