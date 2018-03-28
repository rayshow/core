#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/type_traits/has_inner_type.h>

ccdk_namespace_mpl_it_start

/* iterator satisfies: equal-comparable, dereferenceable, incrementiable, swapable */
struct iterator_category {};

/* input iterator satisfies: equal-comparable, member-reference, post-incrementiable,   */
struct input_category: iterator_category {};

/* output iterator satisfies: assignable, post-incrementiable*/
struct output_category : iterator_category {};

/* input iterator satisfies: equal-comparable, member-reference, post-plus,   */
struct forward_category : input_category {};

/* biward iterator satisfies: decrementable, post-decrementable*/
struct biward_category : forward_category {};

/* random iterator satisfies: comparable, subscriptable, jumpable */
struct random_category : biward_category {};

template<typename... Args>
class iterator;



ccdk_namespace_mpl_it_end