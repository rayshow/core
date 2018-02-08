#pragma once

#include<ccdk/compile.h>
#include<ccdk/mpl/mcontainer/mcontainer_fwd.h>
#include<ccdk/mpl/base/type_.h>

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
	typedef false_           empty;
	typedef uint32_< 1 + Next::size::value> size;

	template<typename T2>
	using push_front = slist_< T2, this_type >;
};


template<>
struct slist_<null_, null_> 
{
	typedef slist_     type;
	typedef slist_     this_type;
	/* no deref, no next , no front, no pop_front, */
	typedef slist_     begin;
	typedef slist_     end;
	typedef true_      empty;
	typedef uint32_<0> size;

	template<typename T2>
	using push_front = slist_< T2, this_type >;
};

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