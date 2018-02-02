#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/type_.h>
#include<ccdk/mpl/mcontainer/mcontainer_fwd.h>
#include<ccdk/mpl/mcontainer/extensible_.h>

ccdk_namespace_mpl_start

template<typename T>
struct val_pack<T>
{
	typedef forward_category     category;
	typedef val_pack             type;
	typedef val_pack             begin;
	typedef val_pack             end;
	typedef uint32_<0>           size;
	typedef true_                empty;
	typedef val_pack             clear;

	template<typename P>
	struct push_back;


	template<T v>
	struct push_back< compile_t<T,v>>
	{
		typedef val_pack<T, v> type;
	};

	template<typename P>
	struct push_front;
			
	template<T v>
	struct push_front< compile_t<T,v>>
	{
		typedef val_pack<T, v> type;
	};

	template<typename P>
	struct merge;

	template<T... args2>
	struct merge< val_pack<T, args2...> >
	{
		typedef val_pack<T, args2...> type;
	};
};

template<typename T, T first, T... args>
struct val_pack <T,first,args...>
{ 
	typedef forward_category     category;
	typedef val_pack             type; 
	typedef val_pack             begin;
	typedef val_pack<T>          end;
	typedef val_pack<T, args...> next;
	typedef compile_t<T, first>  deref;
	typedef val_pack<T, args...> pop_front;
	typedef compile_t<T, first>  front;
	typedef uint32_<sizeof...(args)+1> size;
	typedef false_               empty;
	typedef val_pack<T>          clear;
			
	template<typename P>
	struct push_front;

	template<T v>
	struct push_front< compile_t<T, v>>
	{
		typedef val_pack<T, v, first, args...> type;
	};

	template<typename P>
	struct push_back;

	template<T v>
	struct push_back< compile_t<T, v>>
	{
		typedef val_pack<T, first, args..., v> type;
	};

	template<typename P>
	struct merge;

	template<T... args2>
	struct merge< val_pack<T, args2...> >
	{
		typedef val_pack<T, first, args..., args2...> type;
	};
};

/* indice pack */
template<uint32... indice> using indice_pack = val_pack<uint32, indice...>;

/* is indice pack */
template<typename T> struct is_indice_pack :false_ {};
template<uint32... indice> struct is_indice_pack< indice_pack<indice...>> :true_ {};

/* indice pack first */
template<typename T, T c, T... args> struct val_first { static constexpr T value = c; };

namespace mpl_impl
{
	template<typename T, T Cur, T End, typename Container>
	struct make_val_pack_impl;

	template<typename T, T Cur, T End, T... args>
	struct make_val_pack_impl<T,Cur,End, val_pack<T, args...>> : make_val_pack_impl< T, Cur + 1, End, val_pack<T,args...,Cur> >
	{
		static_assert(Cur < End, "Cur need small then End");
	};

	template<typename T, T End, T... args>
	struct make_val_pack_impl<T, End, End, val_pack<T,args...>>
	{
		typedef val_pack<T, args...> type;
	};
}

/* make val pack */
template<typename T, T Start, T End>
struct make_val_pack : mpl_impl::make_val_pack_impl<T, Start, End, val_pack<T>> {};

/* make indice */
template<uint32 End>
using make_indice = typename make_val_pack<uint32, 0, End>::type;

template<uint32 Start, uint32 End>
using make_indice_from = typename make_val_pack< uint32, Start, End>::type;

template<uint32 End, uint32 IgnoreStart, uint32 IgnoreEnd>
using make_indice_ignore = merge_t< make_indice<IgnoreStart>, make_indice_from<IgnoreEnd, End> >;


ccdk_namespace_mpl_end