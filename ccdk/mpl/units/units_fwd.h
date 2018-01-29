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

ccdk_namespace_mpl_units_end