#pragma once

#include<core/mpl/if.h>
#include<core/mpl/container/bilist_decl.h>
#include<core/mpl/container/node_.h>
#include<core/mpl/short_inner_type_decl.h>

namespace Aurora3D
{
	namespace mpl
	{
		namespace detail
		{
			template<typename L1, typename L2, bool UseHead>
			struct BilistAppendImpl
			{
				typedef BilistSyncT<L1, true> newL1;
				typedef BilistSyncT<L2, true> newL2;
				typedef NodeAppend< Head<newL1>, Head<newL2> > newHead;
				typedef NodeReverseT<newHead> newTail;
				typedef Bilist_<newHead, newTail, LengthV(newHead) > type;
			};

			template<typename L1, typename L2>
			struct BilistAppendImpl<L1,L2,false>
			{
				typedef BilistSyncT<L1, false> newL1;
				typedef BilistSyncT<L2, false> newL2;
				typedef NodeAppend< Tail<newL1>, Tail<newL2>> newTail;
				typedef NodeReverseT<newTail> newHead;
				typedef Bilist_<newHead, newTail, LengthV(newHead) > type;
			};
		}

		template<typename L1, typename L2>
		struct BilistAppend : public detail::BilistAppendImpl<L1, L2,
			GreaterEqV( HeadLengthV(L1) + HeadLengthV(L2),
				TailLengthV(L1) + TailLengthV(L2))>
		{
			static_assert(L1::length > 0 && L2::length > 0, "either Bilist  can be empty.");
		};
	}
}