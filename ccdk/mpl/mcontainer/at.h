#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/mcontainer/mcontainer_fwd.h>

ccdk_namespace_mpl_start

namespace mct_detail
{
	/* for forward container */
	template<typename It, uint32 Index>
	struct forward_at : forward_at< typename next<It>::type, Index - 1> {};

	template<typename It> struct forward_at<It, 0> : deref<It> {};

	/* implements by Category */
	template<typename Container, uint32 Index, typename Categroy>
	struct at_impl;

	/* forward conatiner */
	template<typename Container, uint32 Index>
	struct at_impl< Container, Index, forward_category> :forward_at< typename begin<Container>::type, Index> {};

}


/* at : get type at Index of Container */
template<typename Container, uint32 Index> 
struct at : mct_detail::at_impl< Container, Index, typename Container::category> {};

template<typename Container, uint32 Index>
using at_t = typename at<Container, Index>::type;

ccdk_namespace_mpl_end