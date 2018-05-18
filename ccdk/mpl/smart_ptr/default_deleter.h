#pragma once
#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/type_traits/is_same.h>

ccdk_namespace_mpl_sp_start

	template<typename T>
	struct default_deleter
	{
		void operator()(T*& t) const
		{
			ptr::safe_delete(t);
		}
	};

	template<typename T>
	struct default_deleter<T[]>
	{
		void operator()(T*& t) const
		{
			ptr::safe_delete_array(t);
		}
	};

ccdk_namespace_mpl_sp_end

ccdk_namespace_mpl_tt_impl_start

//special partialization
template<typename T1, typename T2> 
struct is_same_impl< ccdk::mpl::sp::default_deleter<T1>, ccdk::mpl::sp::default_deleter<T2>> :true_ {};

ccdk_namespace_mpl_tt_impl_end

