#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/integer_.h>
#include<ccdk/mpl/mcontainer/mcontainer_fwd.h>
#include<ccdk/mpl/mcontainer/push_back.h>

ccdk_namespace_mpl_start

namespace mct_detail
{
	template<typename Container, typename T, T cur, T end>
	struct make_val_pack_impl:
		make_val_pack_impl< 
			typename push_back<Container, compile_t<T,cur>>::type,
			T, cur+1,end
		>
	{};

	template<typename Container, typename T, T end>
	struct make_val_pack_impl<Container, T, end, end>
	{
		typedef Container type;
	};
}


template<typename T, T start, T end>
struct make_val_pack:mct_detail::make_val_pack_impl< val_pack<T>, T,start,end>
{};

ccdk_namespace_mpl_end