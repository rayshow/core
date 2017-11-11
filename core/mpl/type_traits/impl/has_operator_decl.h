#pragma once

#include<core/compile.h>
#include<core/preprocessor/seq_foreach_item.h>
#include<core/mpl/base/bool_.h>
#include<core/mpl/base/null_.h>
#include<core/mpl/base/and_.h>
#include<core/mpl/base/not_.h>
#include<core/mpl/type_traits/is_void.h>
#include<core/mpl/type_traits/is_const.h>
#include<core/mpl/type_traits/is_rref.h>
#include<core/mpl/type_traits/declval.h>
#include<core/mpl/type_traits/remove_cv.h>
#include<core/mpl/type_traits/remove_pointer.h>
#include<core/mpl/type_traits/remove_ref.h>
#include<core/mpl/type_traits/impl/has_operator_type.h>

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
				typedef remove_pointer_t<L>           left_noptr_t;
				typedef remove_pointer_t<R>           right_noptr_t;
				typedef remove_cv_t<remove_ref_t<L>>  left_nocv_t;
				typedef remove_cv_t<remove_ref_t<R>>  right_nocv_t;

			};

			//both parameter can't be void for unary or binary operation
			template<typename L, typename R = L>
			struct is_parameter_valid : public parameter_extracter<L, R>
			{
				static constexpr bool value = !is_void_v<left_nocv_t> && !is_void_v<right_nocv_t>;
			};

			//if OP::invoke() is ill-formed , T will implicitly convert to anything
			struct anything { template <class T> anything(T) {}; };

			// anything op anything => no_op like: anything+anything => no_op
#define BINARY_OPERATION(sign, ...) static no_op operator sign(const anything&, const anything&) { return declval<no_op>(); }

			// op anything => no_op like: +anything => no_op
#define UNARY_PRE_OPERATION(sign, ...) static no_op operator sign(const anything&){ return declval<no_op>(); }

#define UNARY_POST_OPERATION(sign, ...) static no_op operator sign(const anything&, int){ return declval<no_op>(); }

			//binary operator  + - * / % & | ^ >> <<  += -= *= /= %= |= &= ^= >>= <<= && || > >= < <= == != (except = refer to has assigner)
			A3D_PP_FOREACH_ITEM(BINARY_OPERATION, (+, -, *, / , %, &, | , ^, +=, -=, *=, /=, %=, &=, |=, ^=, << , >> , >>=, <<=, &&, || , <, <= , >, >= , == , != ));
			// pre-unary operator --T ++T -T +T *T !T ~T
			A3D_PP_FOREACH_ITEM(UNARY_PRE_OPERATION, (++, --, !, ~, *, -, +));

			A3D_PP_FOREACH_ITEM(UNARY_POST_OPERATION, (++, --));
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
				//static  constexpr no_op convert(const has_any&) { return declval<no_op>(); };

				static  constexpr no_op convert(...) { return declval<no_op>(); };

				//match NoOperation
				//static  constexpr no_op convert(const no_op&) { return declval<no_op>(); };
			};

			//if   Left op Right exists,  OP::invoke() return HasOperation
			//else return NoOperation,  do [ operator,(NoOperation, HasOperation) ] and GET NoOperation type
			template<typename OP>
			struct is_operation_valid :public bool_< A3D_TT_HAS_OP((  OP::invoke(), declval<has_op>()  )) > {};

			//if    OP::invoke() valid and return void, (void, has_any) return HasAnyReturn =>NoOperation
			//else  OP::invoke() exists and return Non-NoOperation type, (T, HasAnyReturn) return T
			//else  OP::invoke() not exists(imply convert to ImplicitConverted op ImplicitConverted, return NoOperation)
			//                     (NoOperation,HasAnyReturn) return NoOperation
			//if    return type is ingored always true
			template<typename OP, typename Ret, bool isConstOrRValue = or_v< is_const<Ret>, is_rref<Ret>> >
			struct is_ret_valid :
				public bool_< A3D_TT_HAS_OP( ret_convert<Ret>::convert((  OP::invoke()/**, declval<has_any>()*/  )) ) > { };

			// Ret == null_ need ingore ret check
			template<typename OP>  struct is_ret_valid<OP, null_, false> : public true_ {};

			// Ret is const or rvalue, can't change
			template<typename OP, typename T>  struct is_ret_valid<OP, T, true> : public false_ {};

			//if    BinaryOp::Op() return Non-NoOperation type,  GET NoOperation
			//else  BinaryOp::Op() return NoOperation type, GET HasVoidReturn
			//if    return type is ingored always true
			template<typename OP> struct is_void_ret_valid :
				public bool_< A3D_TT_HAS_OP((  OP::invoke(), declval<has_void>()  ))> {};

			//for binary operation
			//operation overload contained in class(member function) left imply type is a left value reference, so Left can't be const T 
			//Left or Right type qualified with & and && passed to operation will miss
			template<typename OP, typename L, typename R, typename Ret, bool isVoid = is_void_v<Ret> >
			struct has_binary_operation :public and_<
				is_parameter_valid<L, R>,
				is_operation_valid<OP>,
				is_ret_valid<OP, Ret> > {};

			//return void
			template <typename OP, typename L, typename R, typename Ret>
			struct has_binary_operation<OP, L, R, Ret, true> :public and_<
				is_parameter_valid<L, R>,
				is_operation_valid<OP>,
				is_void_ret_valid<OP > > {};


			//for unary Operation
			template<typename OP, typename T, typename Ret, bool isVoid = is_void_v<Ret> >
			struct has_unary_operation :public and_<
				is_parameter_valid<T>,
				is_operation_valid<OP>,
				is_ret_valid<OP, Ret>> {};

			//return is void
			template<typename OP, typename T, typename Ret>
			struct has_unary_operation<OP, T, Ret, true> :public and_<
				is_parameter_valid<T>,
				is_operation_valid<OP>,
				is_void_ret_valid<OP> >{};

			// operation check has three state: 
			// 1. op exists and is public access, test parameter and ret if op is will-formed return true
			// 2. op exists and is not public, static assert failed and cause a compile error(need imporve)
			// 3. op not exists return false

			//for binary operation declare,  Left OP Right 
#define  HAS_BINARY_OPERATION_DECL(sign, opname, check_LR_fn )                                         \
			namespace op_detail                                                                        \
			{                                                                                          \
				template<typename L, typename R>                                                       \
					struct opname ## _operation                                                        \
				{                                                                                      \
					static constexpr decltype(auto) invoke()                                           \
					{                                                                                  \
						return makeval<remove_const_t<L>>() sign makeval<R>();                         \
					}                                                                                  \
				};                                                                                     \
				template<typename left, typename right> struct check_ ## opname ## _parameter :        \
					public parameter_extracter<left, right>                                            \
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
			struct has_## opname<L,R,Ret,true> : public false_ {};                                     \
                                                                                                       \
			template<typename L, typename R, typename Ret = null_>                                     \
			constexpr bool has_ ## opname ## _v = has_ ## opname<L,R,Ret>::value
		

		//for pre-unary operation declare, OP T like -1 
#define HAS_FRONT_UNARY_OPERATION_DECL(sign, opname, check_T_fn)                                 \
			namespace op_detail                                                                  \
			{                                                                                    \
				template<typename T>                                                             \
				struct opname ## _operation                                                      \
				{                                                                                \
					static constexpr decltype(auto) invoke()                                     \
					{                                                                            \
						return sign makeval<T>();                                                \
					}                                                                            \
				};                                                                               \
				template<typename T>                                                             \
				struct check_ ## opname ## _parameter :public parameter_extracter<T>             \
				{                                                                                \
					static const bool value = check_T_fn;                                        \
				};                                                                               \
			}                                                                                    \
			template<typename T, typename Ret = null_,                                           \
				bool invalid = op_detail::check_ ## opname ## _parameter<T>::value>              \
			struct has_ ## opname :public op_detail::has_unary_operation<                        \
				op_detail::opname ## _operation<T>,T,Ret> {};                                    \
                                                                                                 \
			template<typename T, typename Ret>                                                   \
			struct has_ ## opname <T, Ret, true> :false_ {};                                     \
			template<typename T, typename Ret = null_>                                           \
			constexpr bool has_ ## opname ## _v = has_ ## opname<T,Ret>::value 

		} //namespace op_detail
	}//namespace mpl
}
#if defined(AURORA3D_COMPILER_MSVC)
#pragma warning(pop)
#endif
