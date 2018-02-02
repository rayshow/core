#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/type_.h>
#include<ccdk/mpl/mcontainer/mcontainer_fwd.h>
#include<ccdk/mpl/mcontainer/backward/push_back_.h>
#include<ccdk/mpl/mcontainer/val_pack.h>

ccdk_namespace_mpl_start

namespace mct_detail
{
	template<typename Container, typename T, T Cur, T End>
	struct make_val_pack_impl:
		make_val_pack_impl<push_back_t<Container, compile_t<T,Cur>>, T, Cur+1, End >
	{
		static_assert(Cur<End,"Cur need small then End")
	};

	template<typename Container, typename T, T End>
	struct make_val_pack_impl<Container, T, End, End>
	{
		typedef Container type;
	};
}

template<typename T, T Start, T End>
struct make_val_pack : mct_detail::make_val_pack_impl< val_pack<T>, T,Start,End> {};

ccdk_namespace_mpl_end