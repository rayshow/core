#pragma once

#include<ccdk/preprocessor/seq_foreach_tuple.h>

#include<ccdk/type.h>
#include<ccdk/mpl/base/if_.h>
#include<ccdk/mpl/base/condi_derive.h>
#include<ccdk/mpl/base/make_indice.h>
#include<ccdk/mpl/base/val_pack_merge.h>
#include<ccdk/mpl/type_traits/traits_case.h>


namespace ccdk
{
	namespace mpl
	{
		template<template<typename...> typename T>
		using case_index_t = typename case_index<T>::type;

		template<typename T, bool b>
		using case_val_t = typename case_val<T, b>::type;

		namespace tt_detail
		{
			template< uint64 u1, uint64 u2>
			struct u64x2
			{
				static constexpr uint64 v1 = u1;
				static constexpr uint64 v2 = u2;
				typedef u64x2<u1, u2> type;
			};

			template<typename T>
			struct is_default_ :false_ {};

			template<> struct is_default_< u64x2<(uint64)-1, (uint64)-1> >:true_ {};

			template<uint32 bit, uint32... bits>
			struct sft_bit_or  
			{
				static constexpr uint64 value = (1ull << bit) | sft_bit_or<bits...>::value;
			};

			template<uint32 bit>
			struct sft_bit_or<bit> {
				static constexpr uint64 value = (1ull << bit);
			};

			template<uint32 tag, uint32...>
			struct u64x2_create_impl;

			//single
			template<uint32 bit>
			struct u64x2_create_impl<ccdk_tt_rank_single_val, bit>
			{
				static_assert(bit >= 0 && bit < 64, "out of range");
				static constexpr uint64 value = 1ull << bit;
				typedef u64x2<0, value> type;
			};

			//and
			template<uint32... bits>
			struct u64x2_create_impl<ccdk_tt_rank_and_val, bits...>
			{
				typedef u64x2<sft_bit_or<bits...>::value , 0 > type;
			};

			//or
			template<uint32... bits>
			struct u64x2_create_impl<ccdk_tt_rank_or_val, bits...>
			{
				typedef u64x2<0, sft_bit_or<bits...>::value> type;
			};

			//not single/ or composed 
			template<uint32... bits>
			struct u64x2_create_impl<ccdk_tt_rank_not_val, bits...>
			{
				typedef u64x2<(uint64)-1, ~sft_bit_or<bits...>::value> type;
			};

			//not and composed 
			template<uint32... bits>
			struct u64x2_create_impl<ccdk_tt_rank_not_and_val, bits...>
			{
				typedef u64x2<~sft_bit_or<bits...>::value, (uint64)-1> type;
			};

			//default
			template<uint32... bits>
			struct u64x2_create_impl<ccdk_tt_rank_default_val, bits...>
			{
				typedef u64x2<-1, -1> type;
			};

			template<typename T>
			struct u64x2_create;

			template<uint32 tag, uint32... bits>
			struct u64x2_create<indice_pack<tag, bits...>> 
				:u64x2_create_impl<tag, bits...> {};

			template<typename T>
			using u64x2_create_t = typename u64x2_create<T>::type;


			//is and composed traits
			template<typename T, bool = (T::v1!=0)>
			struct u64x2_not
			{
				typedef u64x2<~T::v1, 0> type;
			};

			//is single or ( or composed ) traits
			template<typename T>
			struct u64x2_not<T, false>
			{
				typedef u64x2<0, ~T::v2> type;
			};

			template<typename T>
			using u64x2_not_t = typename u64x2_not<T>::type;


			//both is not and composed
			template<typename T, typename U, bool = (T::v1!=0), bool = (U::v1 != 0)>
			struct u64x2_or2
			{
				typedef u64x2< 0, T::v2 | U::v2> type;
			};

			//T is and composed
			template<typename T, typename U>
			struct u64x2_or2<T,U,true,false>
			{
				typedef u64x2< 0, T::v1 & U::v2> type;
			};

			//U is and composed
			template<typename T, typename U>
			struct u64x2_or2<T, U, false, true>
			{
				typedef u64x2< 0, T::v2 & U::v1> type;
			};


			static constexpr bool is_single_bit(uint64 a)
			{
				if (a == 0) return false;
				if (a & (a - 1)) return false;
				return true;
			}

			//U U both is and composed, need check down grade
			template<typename T, typename U>
			struct u64x2_or2<T, U, true, true>
				:bool_condi_derive< is_single_bit(T::v1 & U::v1 ),
				u64x2< 0, T::v1 & U::v1>,
				u64x2< T::v1 & U::v1, 0>>{};

			template<typename T,  typename... Args>
			struct u64x2_or : u64x2_or2< T, typename u64x2_or<Args...>::type >{};

			template<typename T> struct u64x2_or<T> { typedef T type; };
			
			//special and operation, no one is and composed
			template<typename T, typename U, bool = (T::v1!=0), bool = (U::v1!=0),
				bool = (is_default_<T>::value || is_default_<U>::value)>
			struct u64x2_and2
			{
				typedef u64x2<0, T::v2 & U::v2 > type;
			};

			//T is and composed
			template<typename T, typename U, bool b1, bool b2>
			struct u64x2_and2<T, U, b1, b2, true>
			{
				typedef u64x2< T::v1 & U::v1, T::v2 & U::v2 > type;
			};

			//T is and composed
			template<typename T, typename U>
			struct u64x2_and2<T,U,true,false,false>
			{
				typedef u64x2< T::v1 | U::v2, T::v2 > type;
			};

			//U is and composed
			template<typename T, typename U>
			struct u64x2_and2<T, U, false, true, false>
			{
				typedef u64x2< T::v2 | U::v1, U::v2 > type;
			};

			//both is composed
			template<typename T, typename U>
			struct u64x2_and2<T, U, true, true, false>
			{
				typedef u64x2< T::v1 | U::v1, T::v2|U::v2 > type;
			};

			template<typename T, typename... Args>
			struct u64x2_and : u64x2_and2< T, typename u64x2_and<Args...>::type > {};

			template<typename T> struct u64x2_and<T> { typedef T type; };


			template<uint32 tag, typename T, typename... Args>
			struct rank_merge
				: rank_merge< tag, typename rank_merge<tag, Args...>::type > {};

			template<uint32 tag, uint32 tag1, uint32 tag2, uint32... args1, uint32... args2>
			struct rank_merge< tag, indice_pack<tag1, args1...>, indice_pack<tag2, args2...>>
			{
				typedef indice_pack<tag, args1..., args2...> type;
			};

			template<uint32 tag, uint32 tag1, uint32... args1>
			struct rank_merge<tag, indice_pack<tag1, args1...> >
			{ typedef indice_pack<tag1,args1...> type; };

			template<uint32 tag, typename... Args>
			using rank_merge_t = typename rank_merge<tag, Args...>::type;

			template<uint32 tag, typename T> struct replace_tag {};

			template<uint32 tag, uint32 tag1, uint32... args> 
			struct replace_tag<tag, indice_pack<tag1, args...>> 
			{
				typedef indice_pack<tag, args...> type;
			};



	
			template<uint32 tag, template<typename...> typename... Args>
			using case_recursive_t = u64x2_create_t<
				rank_merge_t<tag, case_index_t<Args>...> >;


			//not and_/or_/not_
			template<typename T, bool b>
			using select_item_t =  u64x2_create_t< case_val_t<T,b> >;
			
			//is and_/or_/not_
			template<uint32 tag, typename... Args>
			using merge_recursive_t = rank_merge_t<tag, case_val_t<Args,false>...>;

			using default_indice_t = indice_pack< ccdk_tt_rank_default_val, 0>;
		}

		//case_val< and_<...>>
		template<typename... Args> struct case_val< and_<Args...>, true>
			: condi_derive< and_<Args...>, case_val< and_<Args...>, false>,
				tt_detail::default_indice_t>{};

		template<typename... Args> struct case_val< and_<Args...>,false>
			: tt_detail::merge_recursive_t< ccdk_tt_rank_and_val, Args...> {};

		//case_val< or_<...>>
		template<typename... Args> struct case_val< or_<Args...>, true>
			: condi_derive< or_<Args...>, case_val< or_<Args...>, false>,
				tt_detail::default_indice_t>{};

		template<typename... Args> struct case_val< or_<Args...>, false>
			: tt_detail::merge_recursive_t< ccdk_tt_rank_or_val, Args...> {};
		
		//case_val< not_<T>>
		template<typename T> struct case_val< not_<T>, true>
			: condi_derive< not_<T>, case_val< not_<T>, false>, tt_detail::default_indice_t> {};

		template<typename T> struct case_not_val 
			: tt_detail::replace_tag< ccdk_tt_rank_not_val, case_val_t<T, false> > {};
		template<typename... Args> struct case_not_val< and_<Args...>>
			: tt_detail::replace_tag< ccdk_tt_rank_not_and_val, case_val_t<and_<Args...>, false> > {};

		template<typename T> struct case_val< not_<T>, false> :case_not_val<T> {};
			

		//case
		template<template<typename...> typename T>
		using case_t = tt_detail::u64x2_create_t<  typename case_index<T>::type >;

		template<template<typename...>typename ... Args>
		using case_and = tt_detail::case_recursive_t< ccdk_tt_rank_and_val, Args...>;

		template<template<typename...>typename ... Args>
		using case_or = tt_detail::case_recursive_t< ccdk_tt_rank_or_val, Args...>;

		
		// dispatch
		template<typename... Args>
		struct select_ : tt_detail::u64x2_and< tt_detail::select_item_t<Args,true>... > {};

		template<typename... Args>
		using select_t = typename select_<Args...>::type;

		template<typename... Args>
		constexpr typename select_<Args...>::type select_case{};

		// default
		using default_t = tt_detail::u64x2<(uint64)-1, (uint64)-1>;
	}
}