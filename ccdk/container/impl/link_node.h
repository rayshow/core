#pragma once

#include<ccdk/container/container_mudule.h>

ccdk_namespace_ct_start

/* forwad link list node */
template<typename T>
struct forward_node {
	using value_type = T;
	T                data;
	forward_node<T> *next;
};

/* biward link list node */
template<typename T>
struct biward_node
{
	using value_type = T;
	T               data;
	biward_node<T> *prev;
	biward_node<T> *next;
};

/* rb tree node */
template<typename T>
struct rb_node
{
	using value_type = T;
	using pointer    = rb_node*;
	enum color_t: uint8{ eRed, eBlack};

	T       data;
	pointer parent;     // to parent node
	pointer left;
	pointer right;
	color_t color;      // node color

	CCDK_FORCEINLINE void set_red() {
		color = eRed;
	}

	CCDK_FORCEINLINE void set_black() {
		color = eBlack;
	}

	CCDK_FORCEINLINE void set_pointer(
		pointer p, pointer l, pointer r) {
		parent = p;
		left = l;
		right = r;
	}

	CCDK_FORCEINLINE void set_parent_left(
		pointer p, pointer l) {
		parent = p;
		left = l;
	}

	CCDK_FORCEINLINE void set_parent_right(
		pointer p, pointer r) {
		parent = p;
		right = r;
	}

	CCDK_FORCEINLINE bool is_red() {
		return color == eRed;
	}

	CCDK_FORCEINLINE bool is_black() {
		return color == eBlack;
	}
};
ccdk_namespace_ct_end


