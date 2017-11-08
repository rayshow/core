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
		struct BilistBack :
			DeriveIf<IsNull_< Tail<S>>,
			NodeLast< Head<S> >,
			NodeFirst< Tail<S>> >
		{
			static_assert(LengthV(S) != 0, "S can't be empty");
		};

		//always O(1)
		template<typename S, typename T>
		struct BilistPushBack :public Bilist_< Head<S>,Node_<T, Tail<S> >> {};

		namespace detail
		{
			//pop 1 element from S::head
			template<typename S>
			struct BilistPopBackImpl
				:public Bilist_< Head<S>, Next<Tail<S>>,
				BoolIfV(LengthV(S) == CommonLengthV(S),
					CommonLengthV(S)-1, CommonLengthV(S)) >
			{};
		}

		//if   S::tailLength > 0  return PopBack(S)
		//else Synchronize tail from head then PopFront(new_S)
		template<typename S>
		struct BilistPopBack:
			public detail::BilistPopBackImpl<
			BoolDeriveIf< GreaterV(TailLengthV(S),0), S, BilistSyncT<S, false> >>
		{
			static_assert(LengthV(S) != 0, "bilist can't be empty");
		};
	}
}