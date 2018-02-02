#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/type_.h>
#include<ccdk/mpl/base/apply_.h>
#include<ccdk/mpl/base/derive_if.h>
#include<ccdk/mpl/type_traits/is_same.h>
#include<ccdk/mpl/mcontainer/mcontainer_fwd.h>
#include<ccdk/mpl/mcontainer/iterator_.h>

ccdk_namespace_mpl_start


/* from iterator begin to end  */
template< 
	typename begin, typename end, typename MFn,
	bool = is_same_v<deref_t<begin>,deref_t<end>>
>
struct it_count_if_ {};


template< typename Container, typename MFn>
struct count_if_
{};


ccdk_namespace_mpl_end