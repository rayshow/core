#pragma once

#include<core/mpl/if.h>
#include<core/mpl/container/node_.h>
#include<core/mpl/container/bilist_decl.h>
#include<core/mpl/short_inner_type_decl.h>

namespace Aurora3D
{
	namespace mpl
	{
		//sync head from tail
		//if   S::length > S::headLength return synchoronized Bilist_
		//else return S
		template<typename S, bool SyncHead>
		struct BilistSync :
			public BoolDeriveIf< GreaterV(LengthV(S),HeadLengthV(S)),
			Bilist_<NodeAppendT< Head<S>, NodeReverseT<Tail<S>, TailLengthV(S) - LengthV(S)> >,
			Tail<S>, TailLengthV(S)>,S>{};

		//sync tail from head
		//if   S::length > S::tailLength return synchoronized Bilist_
		//else return S
		template<typename S>
		struct BilistSync<S,false>:
			public BoolDeriveIf< GreaterV(LengthV(S), TailLengthV(S)),
			Bilist_<Head<S>, NodeAppendT<Tail<S>, NodeReverseT<Head<S>, HeadLengthV(S) - LengthV(S)> >,
			HeadLengthV(S)>, S>{};
	}
}