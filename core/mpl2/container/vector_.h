#pragma once

#include<core/preprocessor/range_call.h>
#include<core/preprocessor/range_prefix.h>
#include<core/preprocessor/range_declare.h>
#include<core/preprocessor/uint8_add_one.h>
#include<core/preprocessor/uint8_sub_one.h>
#include<core/mpl/int_.h>
#include<core/mpl/null_.h>
#include<core/mpl/add.h>
#include<core/mpl/sub.h>
#include<core/mpl/logic_and.h>
#include<core/mpl/logic_equal.h>
#include<core/mpl/category.h>
#include<core/mpl/type_traits/is_same.h>
#include<core/mpl/container/vector_decl.h>
#include<core/mpl/short_inner_type_decl.h>

namespace Aurora3D
{
	namespace mpl
	{
		//vector
		template<typenameprefix,... TArgs> struct Vector_ {};

		//empty Vector
		template<>
		struct Vector_<>
		{
			typedef RandomCategoryTag tag;
			static const int length = 0;
			typedef Vector_<> type;
			typedef Vector_<> reverse;
			typedef VectorIterator<type, 0>        begin;
			typedef VectorIterator<type, length>   end;
			typedef VectorIterator<type, length-1> rbegin;
			typedef VectorIterator<type, -1>       rend;
		};

		//vector size 1 ~ A3D_MPL_VECTOR_CAPACITY-1
#define MPL_VECTOR_SEPCIALIZATION_DECL(Index, _1,_2)                       \
		template<A3D_PP_RANGE_PREFIX(typename T, 0, Index, (,) )>          \
		struct Vector_<A3D_PP_RANGE_PREFIX(T, 0, Index, (, ))>             \
		{                                                                  \
			typedef RandomCategoryTag tag;                                 \
			static constexpr int length = A3D_PP_ADD1(Index);              \
			typedef Vector_<A3D_PP_RANGE_PREFIX(T, 0, Index, (,))> type;   \
			typedef Vector_<A3D_PP_RANGE_PREFIX(T, Index, 0, (,))> reverse;\
			A3D_PP_RANGE_DECLARE(typedef T,,t,0,Index,(;))                 \
			typedef T ## Index                              back;          \
			typedef VectorIterator<type, 0>                 begin;         \
			typedef VectorIterator<type, length>            end;           \
			typedef VectorReverseIterator<type, length - 1> rbegin;        \
			typedef VectorReverseIterator<type, -1>         rend;          \
		};
		A3D_PP_RANGE_CALL(0, A3D_PP_SUB1( A3D_MPL_VECTOR_CAPACITY), 1, MPL_VECTOR_SEPCIALIZATION_DECL, _)
#undef  MPL_VECTOR_SEPCIALIZATION_DECL

		template<typename S, int P>
		struct VectorIterator
		{
			typedef RandomCategoryTag tag;
			typedef S base;
			typedef VectorIterator<S, P> type;
			static constexpr int pos = P;

			//it ++
			struct next :public VectorIterator<S, P+1>
			{
				static_assert(P < LengthV(S), "move forward will be out of range.");
			};

			//it += n
			template<int N>
			struct add :public VectorIterator<S, P+N>
			{
				static_assert(P+N <= LengthV(S), "move forward will be out of range.");
			};

			// it--
			struct prior :public VectorIterator<S,P-1>
			{
				static_assert(P>0, "move backward will be out of range.");
			};

			// it -= n
			template<int N>
			struct sub :public VectorIterator<S, P-N>
			{
				static_assert(P > 0, "move backward will be out of range.");
			};

			// it1 - it2
			template<typename It>
			struct distance :public Int_<P - It::pos>
			{
				static_assert(IsSameV(Base<It>, S), "iterator base type not same.");
			};

			// *it
			struct deref :public VectorAt<S,P> 
			{
				static_assert( P < LengthV(S) && P>=0, "invalid vactor iterator.");
			};

		};

		//same as VectorIterator
		template<typename S, int P>
		struct VectorReverseIterator
		{
			typedef RandomCategoryTag tag;
			typedef S base;
			static constexpr int pos = P;
			typedef VectorReverseIterator<S, P> type;

			//it++
			struct next :public VectorReverseIterator<S, P-1>
			{
				static_assert(P < LengthV(S), "move forward will be out of range.");
			};

			//it += n
			template<int N>
			struct add :public VectorIterator<S, P-N>
			{
				static_assert(P + N <= LengthV(S), "move forward will be out of range.");
			};

			// it--
			struct prior :public VectorIterator<S, P+1>
			{
				static_assert(P >0, "move backward will be out of range.");
			};

			// it -= n
			template<int N>
			struct sub :public VectorIterator<S, P+N>
			{
				static_assert(P >0, "move backward will be out of range.");
			};

			// it1 - it2
			template<typename It>
			struct distance :public Int_<It::pos - P>
			{
				static_assert(IsSameV(Base<It>, S), "iterator base type not same.");
			};

			// *it
			struct deref :public VectorAt<S, P> {};

			// it1 == it2
			template<typename It>
			struct equal : public And< IsSame<S, Base<It>>, Bool_<P == It::pos> > {};
		};

	}
}