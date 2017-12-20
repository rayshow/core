#pragma once
#include<ccdk/mpl/mpl_module.h>

ccdk_namespace_mpl_sp_start

	template<typename T>
	struct normal_deleter
	{
		constexpr void operator()(T*& t) const
		{
			ptr::safe_delete(t);
		}
	};

	template<typename T>
	struct normal_deleter<T[]>
	{
		constexpr void operator()(T*& t) const
		{
			ptr::safe_delete_array(t);
		}
	};

	template<>
	struct normal_deleter<void>
	{
		constexpr void operator()(void *& t) const
		{
			delete t;
			t = nullptr;
		}
	};

ccdk_namespace_mpl_sp_end