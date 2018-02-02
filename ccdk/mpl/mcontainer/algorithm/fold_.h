#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/type_.h>
#include<ccdk/mpl/base/apply_.h>
#include<ccdk/mpl/type_traits/is_same.h>
#include<ccdk/mpl/base/derive_if.h>
#include<ccdk/mpl/mcontainer/mcontainer_fwd.h>
#include<ccdk/mpl/mcontainer/forward/deref_.h>
#include<ccdk/mpl/mcontainer/forward/begin_.h>
#include<ccdk/mpl/mcontainer/forward/end_.h>
#include<ccdk/mpl/mcontainer/forward/next_.h>

ccdk_namespace_mpl_start

namespace mct_detail
{
	template<typename It, typename State, typename P>
	struct fold_impl {};
}

template<typename Container, typename T>
struct fold_ : mct_detail::fold_impl < begin_t_<Container>, T, 0 > {};

ccdk_namespace_mpl_end