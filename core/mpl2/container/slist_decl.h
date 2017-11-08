#pragma once

#include<core/mpl/null_.h>

namespace Aurora3D
{
	namespace mpl
	{
		template<typename Head = Null_> struct Slist_;

		template<typename L> struct SlistReverse;

		template<typename L1, typename L2, int Length = L1::length>
		struct SlistAppend;
	}
}