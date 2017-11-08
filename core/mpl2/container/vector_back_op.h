#pragma once

#include<core/mpl/container/vector_decl.h>
#include<core/mpl/short_inner_type_decl.h>

namespace Aurora3D
{
	namespace mpl
	{
		template<typename S>
		struct VectorBack
		{
			static_assert(LengthV(S) != 0, "Vector length is zero.");
			typename Back<S> type;
		};

		template<typename S, typename T> 
		struct VectorPushBack
		{
			static_assert(LengthV(S) <= A3D_MPL_VECTOR_CAPACITY, "Vector capacity is full, please increase A3D_MPL_VECTOR_CAPACITY.");
		};

		template<typename T, typenameprefix,... Args>
		struct VectorPushBack<Vector_<Argsprefix,...>, T>
		{
			typedef Vector_<Argsprefix,...,T> type;
		};

		template<typename S> struct VectorPopBack :
			public VectorEraseAt<S, LengthV(S) - 1> {};

	}
}