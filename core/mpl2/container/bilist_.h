#pragma once

#include<core/mpl/if.h>
#include<core/mpl/container/node_.h>
#include<core/mpl/container/bilist_decl.h>
// Bilist
namespace Aurora3D
{
	namespace mpl
	{
		// A type of bi-directional-list
		// PushFront, PushBack, Front, Back, PopFront, PopBack can be O(n) = 1
		// At best O(n) = 1  worst O(n) = n
		// Length  O(n) = 1
		// iterate from front to back(back to front) best O(n) = n, worest O(n) = 2n
		template<typename Head, typename Tail, int CommonLength>
		struct Bilist_
		{
			typedef Head head;
			typedef Tail tail;
			typedef Bilist_<Tail, Head, CommonLength> reverse;
			static const int commonLength = CommonLength;
			static const int headLength = NodeLength<Head>::value;
			static const int tailLength = NodeLength<Tail>::value;
			static const int length = headLength + tailLength - CommonLength;
		};
	}
}