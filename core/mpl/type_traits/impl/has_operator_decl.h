#pragma once

#include<core/compile.h>
#include<core/preprocessor/seq_foreach_item.h>
#include<core/mpl/base/bool_.h>
#include<core/mpl/base/null_.h>
#include<core/mpl/base/and_.h>
#include<core/mpl/base/not_.h>
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

namespace core
{
	namespace mpl
	{
		//ensure ImplicitConverted not pollute stand namespace
		namespace op_detail
		{
			//Left op Right is forbidden
			template<typename L, typename R = L> struct parameter_extracter
			{
				typedef remove_cv_t<remove_ref_t<L>>  left_nocv_t;
				typedef remove_cv_t<remove_ref_t<R>>  right_nocv_t;
			};

			//both parameter can't be void for unary or binary operation
			template<typename L, typename R = L>
			struct check_parameter_not_void : public parameter_extracter<L, R>
			{
				static constexpr bool value = !is_void_v<left_nocv_t> && !is_void_v<right_nocv_t>;
			};

			//if OP::invoke() is ill-formed , T will implicitly convert to anything
			struct anything { template <class T> anything(T) {}; };

			// anything op anything => no_op like: anything+anything => no_op
#define BINARY_OPERATION(sign, ...) static no_op operator sign(const anything&, const anything&) { return declval<no_op>(); }

			// op anything => no_op like: +anything => no_op
#define UNARY_PRE_OPERATION(sign, ...) static no_op operator sign(const anything&){ return declval<no_op>(); }

			//binary operator  + - * / % & | ^ >> <<  += -= *= /= %= |= &= ^= >>= <<= && || > >= < <= == != (except = refer to has assigner)
			A3D_PP_FOREACH_ITEM(BINARY_OPERATION, (+, -, *, / , %, &, | , ^, +=, -=, *=, /=, %=, &=, |=, ^=, << , >> , >>=, <<=, &&, || , <, <= , >, >= , == , != ));
			// pre-unary operator --T ++T -T +T *T !T ~T
			A3D_PP_FOREACH_ITEM(UNARY_PRE_OPERATION, (++, --, !, ~, *, -, +));
			//post operator []  ()  new  new[]  delete  delete[] ->  ->*  ,  T--  T++
			//not allowd . .* :: ?:
#undef  BINARY_OPERATION
#undef  UNARY_PRE_OPERATION

			//test return type
			template<typename T> struct ret_convert
			{
				//match every type except NoOperation,HasOperation
				static  constexpr has_op convert(const T&) { return declval<has_op>(); };

				//match void
				static  constexpr no_op convert(const has_any&) { return declval<no_op>(); };

				//match NoOperation
				static  constexpr no_op convert(const no_op&) { return declval<no_op>(); };
			};

			//if   Left op Right exists,  BinaryOp::Op() return HasOperation
			//else return NoOperation,  do [ operator,(NoOperation, HasOperation) ] and GET NoOperation type
			template<typename OP>
			struct check_has_operation :public bool_< HasOperationValue((  OP::invoke(), declval<has_op>()  )) > {};

			//if    OP::invoke() valid and return void, (void, has_any) return HasAnyReturn =>NoOperation
			//else  OP::invoke() exists and return Non-NoOperation type, (T, HasAnyReturn) return T
			//else  OP::invoke() not exists(imply convert to ImplicitConverted op ImplicitConverted, return NoOperation)
			//                     (NoOperation,HasAnyReturn) return NoOperation
			//if    return type is ingored always true
			template<typename OP, typename Ret>
			struct check_has_ret :
				public bool_< HasOperationValue( ret_convert<Ret>::convert((  OP::invoke(), declval<has_any>()  )) ) > { };

			template<typename OP> 
			struct check_has_ret<OP, null_> : public true_ {};

			//if    BinaryOp::Op() return Non-NoOperation type,  GET NoOperation
			//else  BinaryOp::Op() return NoOperation type, GET HasVoidReturn
			//if    return type is ingored always true
			template<typename Operation> struct check_has_ret_void :
				public bool_< HasOperationValue((  Operation::invoke(), declval<has_void>()  ))> {};

			//for binary operation
			//operation overload contained in class(member function) left imply type is a left value reference, so Left can't be const T 
			//Left or Right type qualified with & and && passed to operation will miss
			template<typename OP, typename L, typename R, typename Ret, bool isVoid = is_void_v<Ret> >
			struct has_binary_operation :public and_<
				check_parameter_not_void<L, R>,
				check_has_operation<OP>,
				check_has_ret<OP, Ret> > {};

			//return void
			template <typename OP, typename L, typename R, typename Ret>
			struct has_binary_operation<OP, L, R, Ret, true> :public and_<
				check_parameter_not_void<L, R>,
				check_has_operation<OP>,
				check_has_ret_void<OP > > {};


			//for unary Operation
			template<typename OP, typename T, typename Ret, bool isVoid = is_void_v<Ret> >
			struct has_unary_operation :public and_<
				check_parameter_not_void<T>,
				check_has_operation<OP>,
				check_has_ret<OP, Ret>> {};

			//return is void
			template<typename OP, typename T, typename Ret>
			struct has_unary_operation<OP, T, Ret, true> :public and_<
				check_parameter_not_void<T>,
				check_has_operation<OP>,
				check_has_ret_void<OP> >{};

			//for binary operation declare,  Left OP Right 
#define  HAS_BINARY_OPERATION_DECL(sign, opname, check_LR_fn )                                         \
			namespace op_detail                                                                        \
			{                                                                                          \
				template<typename L, typename R>                                                       \
					struct opname ## _operation                                                        \
				{                                                                                      \
					static constexpr auto invoke()                                                     \
						->decltype(makeval<L>() sign declval<R>());                                    \
				};                                                                                     \
				template<typename L, typename R> struct check_ ## opname ## _parameter :               \
					public parameter_extracter<L, R>                                                   \
				{                                                                                      \
					static constexpr bool value = check_LR_fn;                                         \
				};                                                                                     \
			}                                                                                          \
			template<typename L, typename R = L, typename Ret = null_,                                 \
				bool invalid = op_detail::check_ ## opname ## _parameter<L, R>::value >                \
			struct has_## opname : public op_detail::has_binary_operation<                             \
				op_detail::opname ## _operation<L, R>, L, R, Ret> {};                                  \
                                                                                                       \
			template<typename L, typename R, typename Ret>                                             \
			struct has_## opname<L,R,Ret,true> : public false_ {};
		

		//for pre-unary operation declare, OP T like -1
#define HAS_FRONT_UNARY_OPERATION_DECL(sign, opname, check_LR_fn)                                \
			namespace op_detail                                                                  \
			{                                                                                    \
				template<typename T>                                                             \
				struct opname ## _operation                                                      \
				{                                                                                \
					static constexpr auto invoke()->decltype(sign makeval<T>());                 \
				};                                                                               \
				template<typename T>                                                             \
				struct check_ ## opname ## _parameter :public parameter_extracter<T>             \
				{                                                                                \
					static const bool value = check_LR_fn;                                       \
				};                                                                               \
			}                                                                                    \
			template<typename T, typename Ret = Null_,                                           \
				bool invalid = op_detail::check_ ## opname ## _parameter<T>::value>              \
			struct has_ ## opName :public has_operation_detail::has_unary_operation<             \
				op_detail::opname ## _operation<T>,T,Ret> {};                                    \
                                                                                                 \
			template<typename T, typename Ret>                                                   \
			struct has_ ## opname <T, Ret, true> :false_ {};

		} //namespace op_detail
	}//namespace mpl
}
#if defined(AURORA3D_COMPILER_MSVC)
#pragma warning(pop)
#endif
