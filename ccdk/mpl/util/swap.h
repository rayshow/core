#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/util/move.h>

ccdk_namespace_mpl_util_start

	template<typename T>
	CCDK_FORCEINLINE
	void swap_impl(T* t1, T* t2) noexcept
	{
		T* tmp = t1;
		t1 = t2;
		t2 = tmp;
	}

	template<
		typename T,
		ptr::size_t N
	>
	CCDK_FORCEINLINE
	void swap_impl(T(&t1)[N], T(&t2)[N]) 
	{
		for (int i = 0; i < N; ++i)
		{
			swap(t1[i], t2[i]);
		}
	}

	template< typename T>
	CCDK_FORCEINLINE
	void swap_impl(T& t1, T& t2)
	{
		T tmp{ util::move(t1) };
		t1 = util::move(t2);
		t2 = util::move(tmp);
	}
	
	template<typename T1, typename T2>
	CCDK_FORCEINLINE
	void swap(T1& t1, T2& t2)
	{
		swap_impl(t1, t2);
	}

ccdk_namespace_mpl_util_end