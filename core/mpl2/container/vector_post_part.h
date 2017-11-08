#pragma once

//preprocessor
#include<core/preprocessor/range_call.h>
#include<core/preprocessor/range_prefix.h>
#include<core/preprocessor/uint8_add_one.h>
#include<core/preprocessor/uint8_sub_one.h>
#include<core/mpl/container/vector_decl.h>

namespace Aurora3D
{
	namespace mpl
	{
		//get SubVector at [Pos, end)
		template<typename S, int Pos> struct VectorPostPart
		{
			static_assert(Pos <= S::length && Pos >= 0, "VectorErase Pos out of range.");
			static_assert(S::length != 0, "VectorErase vector size need be greater than 0.");
		};

		//erase 0
		template<typenameprefix,... TArgs>
		struct VectorPostPart< Vector_<TArgsprefix,...>, 0>
		{
			typedef Vector_<TArgsprefix,...> type;
		};

		// 1~ A3D_MPL_VECTOR_CAPACITY
#define MPL_VECTOR_POST_PART_SEPCIALIZATION_DECL(Index, _1,_2)                                                    \
		template<A3D_PP_RANGE_PREFIX(typename T, 0, Index, (,)), typenameprefix,... TArgs>                               \
		struct VectorPostPart< Vector_<A3D_PP_RANGE_PREFIX(T, 0, Index, (,)), TArgsprefix,...>, A3D_PP_ADD1(Index)>      \
		{ typedef Vector_<TArgsprefix,...> type; };
		A3D_PP_RANGE_CALL(0, A3D_PP_SUB1(A3D_MPL_VECTOR_CAPACITY), 1, MPL_VECTOR_POST_PART_SEPCIALIZATION_DECL, (, ))
#undef  MPL_VECTOR_POST_PART_SEPCIALIZATION_DECL
	}
}
