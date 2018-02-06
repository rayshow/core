#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/type_.h>
#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/type_traits/is_integer.h>
#include<ccdk/mpl/mcontainer/mcontainer_fwd.h>

ccdk_namespace_mpl_start

template<typename T, T Start, T End>
struct range_
{
	typedef range_                           type;
	typedef range_                           begin;
	typedef range_<T,End, End>               end;
	typedef range_<T,End - 1, End - 1>       rbegin;
	typedef range_<T,Start - 1, Start - 1>   rend;
	typedef range_<T, Start + 1, End>        next;
	typedef compile_t<T,Start>               front;
	typedef compile_t<T,End - 1>             back;
	typedef compile_t<T,Start>               deref;
	
	template<uint32 Index>
	struct at
	{
		typedef compile_t<T, Start + Index>  type;
	};
};

ccdk_namespace_mpl_end