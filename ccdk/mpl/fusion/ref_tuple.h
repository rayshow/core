#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/make_indice.h>
#include<ccdk/mpl/util/forward.h>
#include<ccdk/mpl/function/create.h>

ccdk_namespace_mpl_fs_start

	template<uint32 K, typename V>
	struct ref_item_t
	{
		V& v;
		ref_item_t(V&& inV)
			:v{ inV }
		{}
	};

	template<uint32 K, typename V>
	V& ref_at(const ref_item_t<K, V>& r)
	{
		return r.v;
	}

	template<
		typename I,
		typename... Args
	>
	struct ref_tuple_impl;

	template<
		uint32... indice,
		typename... Args
	>
	struct ref_tuple_impl<
		indice_pack<indice...>, Args...>
		:ref_item_t<indice, Args>...
	{
		ref_tuple_impl(Args&&... args)
			:ref_item_t<indice, Args>( util::forward<Args>(args) )...
		{}
	};

	template<typename... Args>
	struct ref_tuple
	{
		static constexpr uint32 L = sizeof...(Args);
		typedef ref_tuple_impl< make_indice<L>, Args...> storage_type;
		storage_type storage;

		ref_tuple(Args&&... args)
			:storage{  util::forward<Args>(args)... }
		{
		}

		template<uint32 index>
		constexpr decltype(auto) at() const
		{
			return ref_at<index>(storage);
		}
	};

	template<uint32 K, typename... Args>
	decltype(auto) ref_tuple_at(const ref_tuple<Args...>& t)
	{
		return ref_at<K>(t.storage);
	}
		
	constexpr fn::create_raw_t< ref_tuple> create_ref_tuple{};

	template<typename T> struct is_ref_tuple :false_ {};
	template<typename... Args> struct is_ref_tuple< ref_tuple<Args...> > :true_ {};

ccdk_namespace_mpl_fs_end