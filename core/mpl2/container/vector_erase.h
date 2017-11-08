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
		//main vector erase at Pos [0, size)
		template<typename S, int P>
		struct VectorEraseAt
		{
			static_assert(P <= LengthV(S) && P >= 0, "VectorErase Pos out of range.");
			static_assert(LengthV(S) != 0, "VectorErase vector size need be greater than 0.");
		};

		//erase 0
		template<typename T0, typenameprefix,... TArgs>
		struct VectorEraseAt< Vector_<T0, TArgsprefix,...>, 0>
		{
			typedef Vector_<TArgsprefix,...> type;
		};

		//erase 1~ A3D_MPL_VECTOR_CAPACITY-1
#define MPL_VECTOR_ERASE_ONE_SEPCIALIZATION_DECL(Index, _1,_2)                                        \
		template<A3D_PP_RANGE_PREFIX(typename T, 0, Index, (,)), typenameprefix,... TArgs>                   \
		struct VectorEraseAt< Vector_<A3D_PP_RANGE_PREFIX(T, 0, Index, (,)), TArgsprefix,...>, Index>     \
		{ typedef Vector_<A3D_PP_RANGE_PREFIX(T, 0, A3D_PP_SUB1(Index), (,)), TArgsprefix,...> type; };  
		A3D_PP_RANGE_CALL(1, A3D_PP_SUB1(A3D_MPL_VECTOR_CAPACITY), 1, MPL_VECTOR_ERASE_ONE_SEPCIALIZATION_DECL, (, ))
#undef  MPL_VECTOR_ERASE_ONE_SEPCIALIZATION_DECL

		namespace detail
		{
			//len >=3
			template<typename S, int Begin, int End, int Len> 
			struct VectorEraseImpl :
				VectorAppend<
				typename VectorForePart<S,Begin>::type,
				typename VectorPostPart<S,End>::type > {};
			
			//len =2
			template<typename S, int Begin, int End>
			struct VectorEraseImpl<S,Begin,End,2>
				:public VectorEraseAt<
					typename VectorEraseAt<S,Begin>::type, Begin>{};

			//len = 1
			template<typename S, int Begin, int End>
			struct VectorEraseImpl<S, Begin, End, 1>
				:public VectorEraseAt<S, Begin> {};
		}

		//erase element at [Begin, end)
		template<typename S, int Begin, int End>
		struct VectorErase: public detail::VectorEraseImpl<S,Begin,End,End-Begin>
		{
			static_assert(End >= Begin+1, "VectorErase End need greater than start.");
			static_assert(End <= S::length && Begin >= 0, "VectorErase Pos out of range.");
		};

		
	}
}
