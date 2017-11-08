#pragma once

//preprocessor
#include<core/preprocessor/range_call.h>
#include<core/preprocessor/range_prefix.h>
#include<core/preprocessor/uint8_sub_one.h>
#include<core/mpl/container/vector_decl.h>

namespace Aurora3D
{
	namespace mpl
	{
		//return SubVector at [0,Pos)
		template<typename S, int Pos> struct VectorForePart
		{
			static_assert(Pos <= S::length && Pos >= 0, "VectorForePart Pos out of range.");
			
		};

		// empty
		template<typenameprefix,... TArgs>
		struct VectorForePart< Vector_<TArgsprefix,...>, 0>
		{ typedef Vector_<> type; };

		// 1~ A3D_MPL_VECTOR_CAPACITY-1
#define MPL_VECTOR_BOFORE_PART_SEPCIALIZATION_DECL(Index, _1,_2)                                          \
		template<A3D_PP_RANGE_PREFIX(typename T, 0, Index, (,)), typenameprefix,... TArgs>                       \
		struct VectorForePart< Vector_<A3D_PP_RANGE_PREFIX(T, 0, Index, (,)), TArgsprefix,...>, Index>           \
		{ typedef Vector_<A3D_PP_RANGE_PREFIX(T, 0, A3D_PP_SUB1(Index), (,))> type; };  
		A3D_PP_RANGE_CALL(1, A3D_PP_SUB1(A3D_MPL_VECTOR_CAPACITY), 1, MPL_VECTOR_BOFORE_PART_SEPCIALIZATION_DECL, (, ))
#undef  MPL_VECTOR_BOFORE_PART_SEPCIALIZATION_DECL
	}
}
