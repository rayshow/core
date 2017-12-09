#pragma once

#include<ccdk/compile.h>
#include<ccdk/type.h>
#include<ccdk/mpl/base/and_.h>
#include<ccdk/mpl/type_traits/has_constructor.h>
#include<ccdk/mpl/type_traits/has_assigner.h>
#include<ccdk/mpl/type_traits/declval.h>
#include<ccdk/mpl/util/move.h>


namespace ccdk
{
	namespace mpl
	{
		namespace util_impl
		{
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
				using util::swap;
				for (int i = 0; i < N; ++i)
				{
					swap(t1[i], t2[i]);
				}
			}

			template< typename T>
			CCDK_FORCEINLINE
			void swap_impl(T& t1, T& t2) 
				noexcept( and_v(
					has_nothrow_move_constructor<T>,
					has_nothrow_move_assigner<T>) )
			{
				T tmp{ util::move(t1) };
				t1 = util::move(t2);
				t2 = util::move(tmp);
			}
		}


		namespace util
		{
			template<typename T1, typename T2>
			CCDK_FORCEINLINE
			void swap(T1& t1, T2& t2)
			{
				::ccdk::mpl::util_impl::swap_impl(t1, t2);
			}
		}
	}
}