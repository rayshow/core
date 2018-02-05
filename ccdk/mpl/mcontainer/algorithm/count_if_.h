#pragma once

#include<ccdk/mpl/base/apply_.h>
#include<ccdk/mpl/base/derive_if.h>
#include<ccdk/mpl/mcontainer/iterator_.h>
#include<ccdk/mpl/mcontainer/extensible_.h>


ccdk_namespace_mpl_start

namespace mpl_impl
{
	template< typename It, CCDK_MPL_TFN1(TFn), typename Count, uint32 Step>
	struct count_if_impl :
		count_if_impl< next_t<It>, TFn,
			derive_if< TFn<deref_t<It>>, next_t<Count>, Count>, Step-1>
	{};

	template< typename It, CCDK_MPL_TFN1(TFn), typename Count>
	struct count_if_impl<It, TFn, Count, 0>:Count {};
}

template< typename Container, CCDK_MPL_TFN1(TFn), uint32 Step = size_v<Container>>
struct count_if_: mpl_impl::count_if_impl< 
				begin_t<Container>, TFn, uint32_<0>, Step>
{
	static_assert(Step <= size_<Container>::value, "step need less then Container");
};


ccdk_namespace_mpl_end