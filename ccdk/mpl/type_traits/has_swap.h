#pragma once

#include<ccdk/mpl/type_traits/declval.h>
#include<ccdk/mpl/base/type_.h>

namespace ccdk
{
	namespace mpl
	{
		namespace tt_impl
		{
			template<typename T1, typename T2>
			struct has_swap_helper
			{
				template<typename P1,typename P2,
					typename =decltype( declval<P1>().swap(declval<P2>()) )>
				constexpr static bool sfinae(int) { return true; }

				template<typename P1, typename P2>
				constexpr static bool sfinae(...) { return false; }

				constexpr static bool value = sfinae<T1, T2>(0);
			};
		}
		template<typename T1, typename T2>
		struct has_swap :bool_< tt_impl::has_swap_helper<T1, T2>::value > {};
		
		template<typename T1, typename T2>
		static constexpr bool  has_swap_v = tt_impl::has_swap_helper<T1, T2>::value;
	}
}