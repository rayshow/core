#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/bool_.h>

ccdk_namespace_mpl_units_start

typedef float default_value_type;  /* float as units value type is enough for most context */


template<typename From, typename To>
struct converter_
{
	static constexpr value = false;
};

template<typename From, typename To>
struct has_converter :converter_<From, To> {};


/* get main transform when two transform do operation */
template<typename Transform1, typename Transform>
struct main_transform;

/* if same type ,just const-ref to it, else use copy constructor to new type */
template<typename From, typename To>
struct copy_or_ref
{
	typedef To type;
};

template<typename To>
struct copy_or_ref<To, To>
{
	typedef To const& type;
};

template<typename From, typename To>
using copy_or_ref_t = typename copy_or_ref<From, To>::type;

ccdk_namespace_mpl_units_end