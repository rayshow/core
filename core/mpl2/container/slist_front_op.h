#pragma once

#include<core/mpl/container/slist_decl.h>
#include<core/mpl/container/node_.h>
#include<core/mpl/short_inner_type_decl.h>

namespace Aurora3D
{
	namespace mpl
	{
		template<typename S>
		struct SlistFront
		{
			static_assert(LengthV(S) != 0, "Slist length can't be zero.");
			typedef Type<Head<S>> type;
		};

		template<typename S, typename T>
		struct SlistPushFront
		{
			typedef Slist_<Node_<T, Head<S>>> type;
		};

		template<typename S> 
		struct SlistPopFront
		{
			static_assert(LengthV(S) != 0, "Slist length can't be zero.");
			typedef Slist_<Next<S>> type;
		};
	}
}