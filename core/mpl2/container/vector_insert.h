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
		template<typename S,int Pos> struct VectorInsertImpl
		{
			static_assert(Pos <= S::length && Pos >= 0, "VectorInsert Pos out of range.");
		};

		//VectorInsert at Pos 0
		template<typenameprefix,... TArgs>
		struct VectorInsertImpl<Vector_<TArgsprefix,...>, 0>
		{
			template<typename U,typenameprefix,... UArgs>
			struct Apply
			{
				static_assert(sizeofprefix,...(TArgs)+1+sizeofprefix,...(UArgs) < A3D_MPL_VECTOR_CAPACITY, "VectorInsert vector capacity is full.");
				typedef Vector_<U, UArgsprefix,..., TArgsprefix,...> type;
			};
		};

#define MPL_VECTOR_INSERT_IMPL_SPECIALIZATION_DECL(Index, _1, _2)                                                  \
		template<A3D_PP_RANGE_PREFIX(typename T, 0, Index, (, )), typenameprefix,... TArgs>                          \
		struct VectorInsertImpl<Vector_<A3D_PP_RANGE_PREFIX(T, 0, Index, (,)), TArgsprefix,...>, A3D_PP_ADD1(Index)> \
		{                                                                                                     \
			template<typename U, typenameprefix,... UArgs>                                                           \
			struct Apply                                                                                      \
			{                                                                                                 \
				static_assert(Index + sizeofprefix,...(TArgs) + 2 + sizeofprefix,...(UArgs) < A3D_MPL_VECTOR_CAPACITY,      \
					"VectorInsert vector capacity is full.");                                                 \
				typedef Vector_<A3D_PP_RANGE_PREFIX(typename T, 0, Index, (,)),                               \
					U, UArgsprefix,..., TArgsprefix,...> type;                                                              \
			};                                                                                                \
		};
		A3D_PP_RANGE_CALL(0, A3D_PP_SUB1(A3D_MPL_VECTOR_CAPACITY), 1, MPL_VECTOR_INSERT_IMPL_SPECIALIZATION_DECL, _)
#undef  MPL_VECTOR_INSERT_IMPL_SPECIALIZATION_DECL


		//main vector insert at Pos [0, size]
		template<typename S, int Pos, typename U, typenameprefix,... UArgs>
		struct VectorInsert:public VectorInsertImpl<S,Pos>::template Apply<U,UArgsprefix,...>{};
	}
}
