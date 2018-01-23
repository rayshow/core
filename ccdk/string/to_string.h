#pragma once

#include<ccdk/string/string_module.h>
#include<ccdk/mpl/util/copy.h>
#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/type_traits/array_length.h>
#include<ccdk/mpl/type_traits/remove_all_dim.h>
#include<ccdk/mpl/type_traits/is_same.h>
#include<ccdk/string/encoding/ascii.h>
#include<ccdk/string/to_string.h>

ccdk_namespace_string_start

template<typename From, typename To, bool = sizeof(From)< sizeof(To) >
struct is_char_convertible : mpl::true_ {};

template<typename From, typename To>
struct is_char_convertible<From,To,false> : mpl::false_ {};



template<typename String>
struct to_string_t<char, String>
{
	typedef typename String::char_type char_type;

	/* local version */
	String& operator()(char c, String& str) const
	{
		return str.push_back( char_type(c) );
	}
};

template<typename String>
struct to_string_t<char16, String>
{
	typedef typename String::char_type char_type;

	/* local version */
	String& operator()(char c, String& str) const
	{
		return str.push_back(char_type(c));
	}
};

ccdk_namespace_string_end