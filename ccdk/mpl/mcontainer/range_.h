#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/type_.h>
#include<ccdk/mpl/base/arithmetic_.h>
#include<ccdk/mpl/type_traits/is_integer.h>
#include<ccdk/mpl/mcontainer/mcontainer_fwd.h>

ccdk_namespace_mpl_start

template<typename T, T Start, T End>
struct range_reverse_iter_
{ 
	static constexpr T value = Start;
	typedef range_reverse_iter_<T,Start - 1, End> next;
	typedef compile_t<T, Start>                   deref;
};

/* no-define for no-range_ */
template<typename T1, typename T2> struct range_merge_;


template<typename T, T Start, T End>
struct range_
{
	static_assert(Start < End, "invalid range_");
	static constexpr T value = Start;
	typedef range_                                         type;
	typedef range_                                         this_type;
	typedef range_                                         begin;
	typedef range_<T,End, End>                             end;
	typedef range_reverse_iter_<T, End - 1, End - 1>       rbegin;
	typedef range_reverse_iter_<T, Start - 1, Start - 1>   rend;
	typedef range_<T, Start + 1, End>                      next;
	typedef compile_t<T,Start>                             front;
	typedef compile_t<T,End - 1>                           back;
	typedef compile_t<T,Start>                             deref;
	
	template<typename Index>
	using at = compile_t<T, Start + Index::value>;

	/* split  by index */
	template<typename Index>
	struct split_at
	{
		static_assert(Index::value >= 0 && Index::value<= End-Start, "split index out of range");
		typedef range_<T, Start, Start + Index::value > head;
		typedef range_<T, Start + Index::value, End>    tail;
	};

	/* split by index */
	template<typename Iter>
	struct split
	{
		static_assert(Iter::value >= Start && Iter::value < End, "split iter out of range");
		typedef range_<T, Start, Iter::value> head;
		typedef range_<T, Iter::value, End>   end;
	};

	template<typename T2>
	using merge = range_merge_<this_type, T2>;
};

template<typename T, T Start1, T End1, T Start2, T End2> 
struct range_merge_<range_<T, Start1, End1>, range_<T, Start2, End2>>
{
	static_assert(!(End1 < Start2 || End2 < Start1), "invalid range merge");
	typedef range_< T, min_cv<T, Start1, Start2>, min_cv<T, End1, End2> > type;
};


ccdk_namespace_mpl_end