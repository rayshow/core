#pragma once

#include<core/mpl/choose.h>
#include<core/mpl/container/node_.h>
#include<core/mpl/container/slist_decl.h>

namespace Aurora3D
{
	namespace mpl
	{
		// append L2 to L1, result is a Slist_
		// if      L1::head ==Null_ return L2
		// else if L2::head ==Null_ return L1
		// else    return Slist_<L1::head append L2::head>
		template<typename L1, typename L2, int Length>
		struct SlistAppend : public BoolDeriveChoose3<
			L1::length == 0,
			L2::length == 0,
			L2, L1,
			Slist_<typename NodeAppend<
			typename L1::head,
			typename L2::head, Length>::type >>{};

	}
}