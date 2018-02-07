#pragma once

#include<ccdk/compile.h>
#include<ccdk/mpl/mcontainer/mcontainer_fwd.h>

ccdk_namespace_mpl_start

template<typename T = null_, typename Next = null_ >
struct slist_
{
	typedef slist_   type;
	typedef T        deref;
	typedef slist_   begin;
	typedef slist_<> end;
	typedef Next     next;
	typedef T        front;

};

ccdk_namespace_mpl_end