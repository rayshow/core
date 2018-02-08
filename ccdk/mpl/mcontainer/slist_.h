#pragma once

#include<ccdk/compile.h>
#include<ccdk/mpl/mcontainer/mcontainer_fwd.h>

ccdk_namespace_mpl_start

template<typename T, typename Next = null_ >
struct slist_
{
	typedef slist_           type;
	typedef slist_           this_type;
	typedef T                deref;
	typedef slist_           begin;
	typedef slist_<null_>    end;
	typedef Next             next;
	typedef T                front;
	typedef slist_<null_>    clear;
	typedef Next             pop_front;

	template<typename T2>
	using push_front = slist_< T2, this_type >;
};

/* no deref, no next */
template<>
struct slist_<null_, null_> {};

template<typename T, typename... Args>
struct slist_init_
{
	typedef slist_<T, typename slist_init_<Args...>::type> type;
};

template<typename T>
struct slist_init_<T>
{
	typedef slist_<T, slist_<null_> > type;
};

template<typename T,typename... Args>
using slist_init_t = typename slist_init_<T, Args...>::type;

ccdk_namespace_mpl_end