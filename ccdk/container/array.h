#pragma once

#include<typeinfo>
#include<exception>
#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/type_traits/decay.h>
#include<ccdk/mpl/util/forward.h>
#include<ccdk/mpl/util/swap.h>
#include<ccdk/mpl/util/addressof.h>
#include<ccdk/mpl/fusion/bad_cast_exception.h>

ccdk_namespace_mpl_fs_start

template<typename T, uint32 N>
class array
{
public:
	typedef T      value_type;
	typedef array  this_type;
private:
	T content[N];
public:
	array(std::initializer_list<T> const& list) {}
};

ccdk_namespace_mpl_fs_end