#pragma once

#include<core/mpl/container/node_.h>

namespace Aurora3D
{
	namespace mpl
	{
#define HeadLengthV(T)   (T::headLength)
#define TailLengthV(T)   (T::tailLength)
#define CommonLengthV(T) (T::commonLength)

		//Bi
		template<typename Head = Null_, typename Tail = Null_, int CommonLength = 0>
		struct Bilist_;

		template<typename S, bool SyncHead = true>
		struct BilistSync;

		template<typename S, bool SyncHead = true>
		using BilistSyncT = typename BilistSync<S, SyncHead>::type;
	}
}