#pragma once

#include<core/mpl/if.h>
#include<core/mpl/container/bilist_decl.h>
#include<core/mpl/container/node_.h>
#include<core/mpl/short_inner_type_decl.h>

namespace Aurora3D
{
	namespace mpl
	{
		//if(head not empty) O(1) 
		//else O(n)
		template<typename S>
		struct BilistFront :
			DeriveIf< HeadLengthV(S)==0,
			NodeLast<Tail<S>>,
			NodeFirst<Head<S>> >
		{
			static_assert(S::length != 0, "S can't be empty");
		};

		//always O(1)
		template<typename S, typename T>
		struct BilistPushFront :public Bilist_< Node_<T, Head<S> >, Tail<S> > {};

		namespace detail
		{
			//pop 1 element from S::head
			template<typename S>
			struct BilistPopFrontImpl
				:public Bilist_< Next<Head<S>>, Tail<S>,
				BoolIfV(HeadLengthV(S)==CommonLengthV(S),
					CommonLengthV(S)-1,CommonLengthV(S))>
			{};
		}

		//if   S::headLength >0  return PopFront(S)
		//else Synchronize head from tail then PopFront(new_S)
		template<typename S>
		struct BilistPopFront:
			public detail::BilistPopFrontImpl<
			BoolDeriveIf< GreaterV(HeadLengthV(S),0), S, BilistSyncT<S, true> >>
		{
			static_assert(S::length != 0, "bilist can't be empty");
		};
	}
}