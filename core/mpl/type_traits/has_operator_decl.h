#pragma once

#include<core/compile.h>
#include<core/preprocessor/seq_foreach_item.h>
#include<core/mpl/bool_.h>
#include<core/mpl/null_.h>
#include<core/mpl/logic_and.h>
#include<core/mpl/logic_not.h>
#include<core/mpl/type_traits/is_void.h>
#include<core/mpl/type_traits/declval.h>
#include<core/mpl/type_traits/has_operator_type.h>
#include<core/mpl/type_traits/remove_cv.h>
#include<core/mpl/type_traits/remove_pointer.h>
#include<core/mpl/type_traits/remove_ref.h>

#if defined(AURORA3D_COMPILER_MSVC)
#pragma warning(push)
//4244: ReturnConvert<Ret>::Convert(T) precision loss, e.g.   float convert to int  
//4913: exists user-define operator,(T1,T2), but not match all parameters, and use default inner operator,(prefix,...)
//4505: un-reference local function had been removed
#pragma warning(disable:4244 4913 4505)
#endif

namespace Aurora3D
{
	namespace mpl
	{
		//ensure ImplicitConverted not pollute stand namespace
		namespace has_operation_detail
		{
			//Left op Right is forbidden
			template<typename Left, typename Right = Left> struct CheckParameterHelper
			{
				typedef typename RemoveCVT<RemoveRefT<Left>>   left_nocv_t;
				typedef typename RemoveCVT<RemoveRefT<Right>>  right_nocv_t;
			};

			//both parameter can't be void for unary or binary operation
			template<typename Left, typename Right = Left> struct CheckParameterNotVoid :public CheckParameterHelper<Left, Right>
			{
				static constexpr bool value = !IsVoidV(left_nocv_t) && !IsVoidV(right_nocv_t);
			};

			//if Operation::Op() not defined, will implicitly convert to ImplicitConverted and do operation
			struct ImplicitConverted { template <class T> ImplicitConverted(T) {}; };
#define BINARY_OPERATION(Op, prefix,...) static NoOperation operator Op(const ImplicitConverted&, const ImplicitConverted&) { return Declval<NoOperation>(); }
#define UNARY_PRE_OPERATION(Op, prefix,...) static NoOperation operator Op(const ImplicitConverted&){ return Declval<NoOperation>(); }
			//binary operator  + - * / % & | ^ >> <<  += -= *= /= %= |= &= ^= >>= <<= && || > >= < <= == != (except = refer to has assigner)
			A3D_PP_FOREACH_ITEM(BINARY_OPERATION, (+, -, *, / , %, &, | , ^, +=, -=, *=, /=, %=, &=, |=, ^=, << , >> , >>=, <<=, &&, || , <, <= , >, >= , == , != ));
			// pre-unary operator --T ++T -T +T *T !T ~T
			A3D_PP_FOREACH_ITEM(UNARY_PRE_OPERATION, (++, --, !, ~, *, -, +));
			//post operator []  ()  new  new[]  delete  delete[] ->  ->*  ,  T--  T++
			//not allowd . .* :: ?:
#undef  BINARY_OPERATION
#undef  UNARY_PRE_OPERATION

			//test return type
			template<typename T> struct ReturnConvert
			{
				//match every type except NoOperation,HasOperation
				static  constexpr HasOperation Convert(const T&) { return Declval<HasOperation>(); };

				//match void
				static  constexpr NoOperation Convert(const HasAnyReturn&) { return Declval<HasOperation>(); };

				//match NoOperation
				static  constexpr NoOperation  Convert(const NoOperation&) { return Declval<NoOperation>(); };
			};

			//if   Left op Right exists,  BinaryOp::Op() return Non-NoOperation type, GET HasOperation type
			//else return NoOperation,  do [ operator,(NoOperation, HasOperation) ] and GET NoOperation type
			template<typename Operation>
			struct CheckHasOperation :public Bool_< HasOperationValue((Operation::Op(), Declval<HasOperation>())) > {};

			//if    BinaryOp::Op() exists and return Void, (void, HasAnyReturn) return HasAnyReturn =>NoOperation
			//else  BinaryOp::Op() exists and return Non-NoOperation type, (T, HasAnyReturn) return T
			//else  BinaryOp::Op() not exists(imply convert to ImplicitConverted op ImplicitConverted, return NoOperation)
			//                     (NoOperation,HasAnyReturn) return NoOperation
			//if    return type is ingored always true
			template<typename Operation, typename Ret> struct CheckHasReturn :
				public Bool_< HasOperationValue( ReturnConvert<Ret>::Convert((Operation::Op(), Declval<HasAnyReturn>())) ) > { };
			template<typename Operation> struct CheckHasReturn<Operation, Null_> : public True_ {};

			//if    BinaryOp::Op() return Non-NoOperation type,  GET NoOperation
			//else  BinaryOp::Op() return NoOperation type, GET HasVoidReturn
			//if    return type is ingored always true
			template<typename Operation, typename Ret> struct CheckHasReturnVoid :
				public Bool_< HasOperationValue((Operation::Op(), Declval<HasVoidReturn>()))> {};

			//for binary operation
			//operation overload contained in class(member function) left imply type is a left value reference, so Left can't be const T 
			//Left or Right type qualified with & and && passed to operation will miss
			template<typename BinaryOp, typename Left, typename Right, typename Ret, bool isVoid = IsVoidV(Ret) >
			struct HasBinaryOp :public And<
				CheckParameterNotVoid<Left, Right>,
				CheckHasOperation<BinaryOp>,
				CheckHasReturn<BinaryOp, Ret> > {};

			//return void
			template <typename BinaryOp, typename Left, typename Right, typename Ret>
			struct HasBinaryOp<BinaryOp, Left, Right, Ret, true> :public And<
				CheckParameterNotVoid<Left, Right>,
				CheckHasOperation<BinaryOp>,
				CheckHasReturnVoid<BinaryOp, Ret> > {};


			//for unary Operation
			template<typename UnaryOp, typename OpType, typename Ret, bool isVoid = IsVoidV(Ret)>
			struct HasUnaryOp :public And<
				CheckParameterNotVoid<OpType>,
				CheckHasOperation<UnaryOp>,
				CheckHasReturn<UnaryOp, Ret>> {};

			//return is void
			template<typename UnaryOp, typename OpType, typename Ret>
			struct HasUnaryOp<UnaryOp, OpType, Ret, true> :public And<
				CheckParameterNotVoid<OpType>,
				CheckHasOperation<UnaryOp>,
				CheckHasReturnVoid<UnaryOp, Ret>>{};

			//for binary operation declare,  Left OP Right
#define  HAS_BINARY_OPERATION_DECL(OpSign, OpName, Judgement )                                         \
			namespace has_operation_detail                                                             \
			{                                                                                          \
				template<typename Left, typename Right>                                                \
					struct OpName##Operation                                                           \
				{                                                                                      \
					static constexpr auto Op()->decltype(Make<Left>() OpSign Declval<Right>());        \
				};                                                                                     \
				template<typename Left, typename Right> struct Check##OpName##Parameter :              \
					public CheckParameterHelper<Left, Right>                                           \
				{                                                                                      \
					static constexpr bool value = Judgement;                                           \
				};                                                                                     \
			}                                                                                          \
			template<typename Left, typename Right = Left, typename Ret = Null_,                    \
				bool forbidden = has_operation_detail::Check##OpName##Parameter<Left, Right>::value >  \
			struct Has##OpName : public has_operation_detail::HasBinaryOp<                             \
				has_operation_detail::OpName##Operation<Left, Right>, Left, Right, Ret> {};            \
			template<typename Left, typename Right, typename Ret>                                      \
			struct Has##OpName<Left,Right,Ret,true> : public False_ {};
		

		//for pre-unary operation declare, OP T
#define HAS_FRONT_UNARY_OPERATION_DECL(OpSign, OpName, Judgement)                                \
			namespace has_operation_detail                                                       \
			{                                                                                    \
				template<typename OpType>                                                        \
				struct OpName##Operation                                                         \
				{                                                                                \
					static constexpr auto Op()->decltype(OpSign Make<OpType>());                 \
				};                                                                               \
				template<typename OpType>                                                        \
				struct Check##OpName##Parameter :public CheckParameterHelper<OpType>             \
				{                                                                                \
					static const bool value = Judgement;                                         \
				};                                                                               \
			}                                                                                    \
			template<typename OpType, typename Ret = Null_,                                   \
				bool forbidden = has_operation_detail::Check##OpName##Parameter<OpType>::value>  \
			struct Has##OpName :public has_operation_detail::HasUnaryOp<                         \
				has_operation_detail::OpName##Operation<OpType>,OpType,Ret> {};                  \
			template<typename OpType, typename Ret>                                              \
			struct Has##OpName<OpType, Ret, true> :False_ {};
		} //namespace has_operation_detail
	}//namespace mpl
}
#if defined(AURORA3D_COMPILER_MSVC)
#pragma warning(pop)
#endif
