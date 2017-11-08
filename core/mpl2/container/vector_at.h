#pragma once

//preprocessor
#include<core/preprocessor/range_call.h>
#include<core/preprocessor/range_prefix.h>
#include<core/mpl/container/vector_decl.h>

namespace Aurora3D
{
	namespace mpl
	{
		//main define of RandomAt
		template<typename S, int P> struct VectorAt
		{
			static_assert(P < LengthV(S) && P >= 0, "At pos out of range.");
		};

		//RandomAt 0
		template<typename S>
		struct VectorAt<S, 0> { typedef typename S::t0 type; };

		//RandomAt 1 ~ A3D_MPL_VECTOR_CAPACITY-1
#define	MPL_RANDOM_AT_SPECIALIZATION_DECL(Index, _1, _2)                                              \
		template<typename S>struct VectorAt<S, Index>{ typedef typename S::t ## Index type; };
		A3D_PP_RANGE_CALL(1, A3D_MPL_VECTOR_CAPACITY, 1, MPL_RANDOM_AT_SPECIALIZATION_DECL, _)
#undef	MPL_VECTOR_AT_SPECIALIZATION_DECL
	}
}
