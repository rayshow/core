#pragma once

#include<ccdk/mpl/mpl_module.h>

ccdk_namespace_mpl_it_start

/* iterator satisfies: dereferenceable, incrementiable, swapable */
struct iterator_category {};

/* input iterator satisfies: equal-comparable, member-reference, post-incrementiable,   */
struct input_iterator_category: iterator_category {};

/* output iterator satisfies: assignable, post-incrementiable*/
struct output_iterator_category : iterator_category {};

/* input iterator satisfies: equal-comparable, member-reference, post-plus,   */
struct forward_iterator_category : input_iterator_category {};

/* biward iterator satisfies: decrementable, post-decrementable*/
struct biward_iterator_category : forward_iterator_category {};

/* random iterator satisfies: comparable, subscriptable, jumpable */
struct random_iterator_category : forward_iterator_category {};


template<typename... Args>
class iterator;

template<typename... Args>
class const_iterator;

template<typename... Args>
class reverse_iterator;

template<typename... Args>
class const_reverse_iterator;

ccdk_namespace_mpl_it_end