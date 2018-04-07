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

template<typename T>
struct local_obj
{
private:
	int8  bytes[sizeof(T)];

public:


};

ccdk_namespace_mpl_fs_end