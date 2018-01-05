#pragma once

#include<ccdk/mpl/base/null_.h>
#include<ccdk/mpl/base/bool_.h>
#include<ccdk/mpl/base/logic_.h>
#include<ccdk/mpl/type_traits/declval.h>
#include<ccdk/mpl/type_traits/is_rref.h>



//+ - * / % & | ^ && || >> << == !=  > < >= <=
#define CCDK_TT_HAS_NO_SIDE_EFFECT_BINARY_OP(opsign, opname)					     \
		namespace operator_detail                                                    \
		{                                                                            \
			template<typename L, typename R, typename Ret>                           \
			struct has_ ## opname ## _ret                                            \
			{                                                                        \
				template<typename T, typename U, typename P,                         \
				typename = decltype(makeval<P>() = declval<T>() opsign declval<U>())>\
					static constexpr bool sfinae(int) { return true; }               \
				template<typename T, typename U, typename P>                         \
				static constexpr bool sfinae(...) { return false; }                  \
				static constexpr bool value = sfinae<L, R, Ret>(0);                  \
			};                                                                       \
		}                                                                            \
		template<typename L, typename R = L, typename Ret = L>                       \
		struct has_ ## opname : public                                               \
			operator_detail::has_ ## opname ##_ret<L, R, Ret> {};                    \
		template<typename L, typename R = L, typename Ret = L>                       \
		static constexpr bool has_ ## opname ## _v = has_ ## opname<L, R, Ret>::value;



//TODO: clang  int+=int* will not report error, need to fix
//+= -= *= /= %= &= |= ^= >>== <<== 
#define CCDK_TT_HAS_SIDE_EFFECT_BINARY_OP(opsign, opname)							 \
		namespace operator_detail                                                    \
		{                                                                            \
			template<typename L, typename R>                                         \
			struct has_ ## opname ## _ret                                            \
			{                                                                        \
				template<typename U, typename P,                                     \
				typename = decltype(makeval<U>() opsign declval<P>())>               \
					static constexpr bool sfinae(int) { return true; }               \
				template<typename U, typename P>                                     \
				static constexpr bool sfinae(...) { return false; }                  \
				static constexpr bool value = sfinae<L, R>(0);                       \
			};                                                                       \
		}                                                                            \
		template<typename L, typename R = L >                                        \
		struct has_ ## opname : public                                               \
			operator_detail:: has_ ## opname ##_ret<L, R> {};                        \
		template<typename L, typename R = L>                                         \
		static constexpr bool has_ ## opname ## _v = has_ ## opname<L,R>::value;

//! ~ * - + T
#define CCDK_TT_HAS_RET_UNARY_OP(opsign, opname)									 \
		namespace operator_detail                                                    \
		{                                                                            \
			template<typename T, typename Ret>                                       \
			struct has_ ## opname ## _ret                                            \
			{                                                                        \
				template<typename U, typename P,                                     \
				typename = decltype(makeval<P>() = opsign declval<U>())>             \
				static constexpr bool sfinae(int) { return true; }                   \
				template<typename U, typename P>                                     \
				static constexpr bool sfinae(...) { return false; }                  \
				static constexpr bool value = sfinae<T, Ret>(0);                     \
			};                                                                       \
		}                                                                            \
		template<typename T, typename Ret = T>                                       \
		struct has_ ## opname:and_<not_<is_rref<Ret>>,                               \
			operator_detail::has_ ## opname ## _ret<T, Ret>> {};                     \
		template<typename T, typename Ret = T>                                       \
		static constexpr bool has_ ## opname ## _v = has_ ## opname<T, Ret>::value;


//++ T, -- T
#define CCDK_TT_HAS_PRE_UNARY_OP(opsign, opname)									 \
		namespace operator_detail                                                    \
		{                                                                            \
			template<typename T, typename Ret>                                       \
			struct has_ ## opname ## _ret                                            \
			{                                                                        \
				template<typename U, typename P,                                     \
				typename = decltype(makeval<P>() = opsign makeval<U>())>             \
				static constexpr bool sfinae(int) { return true; }                   \
				template<typename U, typename P>                                     \
				static constexpr bool sfinae(...) { return false; }                  \
				static constexpr bool value = sfinae<T, Ret>(0);                     \
			};                                                                       \
		}                                                                            \
		template<typename T, typename Ret = T>                                       \
		struct has_ ## opname:and_<not_<is_rref<Ret>>,                               \
			operator_detail::has_ ## opname ## _ret<T, Ret>> {};                     \
		template<typename T, typename Ret = T>                                       \
		static constexpr bool has_ ## opname ## _v = has_ ## opname<T, Ret>::value;


// T--, T++
#define CCDK_TT_HAS_POST_UNARY_OP(opsign, opname)									 \
		namespace operator_detail                                                    \
		{                                                                            \
			template<typename T, typename Ret>                                       \
			struct has_ ## opname ## _ret                                            \
			{                                                                        \
				template<typename U, typename P,                                     \
				typename = decltype(makeval<P>() =  makeval<U>() opsign)>            \
				static constexpr bool sfinae(int) { return true; }                   \
				template<typename U, typename P>                                     \
				static constexpr bool sfinae(...) { return false; }                  \
				static constexpr bool value = sfinae<T, Ret>(0);                     \
			};                                                                       \
		}                                                                            \
		template<typename T, typename Ret = T>                                       \
		struct has_ ## opname:and_<not_<is_rref<Ret>>,                               \
			operator_detail::has_ ## opname ## _ret<T, Ret>> {};                     \
		template<typename T, typename Ret = T>                                       \
		static constexpr bool has_ ## opname ## _v = has_ ## opname<T, Ret>::value;


// Ret = T.operator {-> / [] / () } ( Args... ) 
#define HAS_MEMBER_FN_DECL(sign, opname )                                                                           \
		namespace detail                                                                                            \
		{                                                                                                           \
			template<typename T, typename Ret, typename... Args>                                                    \
			struct has_ ## opname ## _helper                                                                        \
			{                                                                                                       \
				template<typename U, typename = decltype( makeval<Ret>() = makeval<U>().sign(declval<Args>()...) )> \
				static constexpr bool sfinae(int){ return true;}                                                    \
				template<typename U>                                                                                \
				static constexpr bool sfinae(...) { return false;}                                                  \
				static constexpr bool value = sfinae<T>(0);                                                         \
			};                                                                                                      \
			template<typename T, typename P, typename... Args>                                                      \
			struct has_ ## opname ## _helper<T,null_,P, Args...>                                                    \
			{                                                                                                       \
				template<typename U, typename = decltype( makeval<U>().sign(declval<P>(), declval<Args>()...) )>    \
				static constexpr bool sfinae(int){ return true;}                                                    \
				template<typename U>                                                                                \
				static constexpr bool sfinae(...) { return false;}                                                  \
				static constexpr bool value = sfinae<T>(0);                                                         \
			};                                                                                                      \
			template<typename T>                                                                                    \
			struct has_ ## opname ## _helper<T,null_>                                                               \
			{                                                                                                       \
				template<typename U, typename = decltype( &U::sign )>                                               \
				static constexpr bool sfinae(int){ return true;}                                                    \
				template<typename U>                                                                                \
				static constexpr bool sfinae(...) { return false;}                                                  \
				static constexpr bool value = sfinae<T>(0);                                                         \
			};                                                                                                      \
		}                                                                                                           \
		template<typename T,typename Ret = null_, typename... Args>                                                 \
		struct has_ ## opname :and_<not_<is_rref<Ret>>,                                                             \
			bool_< detail::has_ ## opname ## _helper <T, Ret, Args...>::value >> {};                                \
		template<typename T, typename Ret = null_, typename... Args>                                                \
		static constexpr bool has_ ## opname ## _v = has_ ## opname<T, Ret, Args...>::value;


// void* = new(Args...) / new[](Args...) T;  void = delete(Args...) / delete[](Args...) T
#define HAS_NEW_DELETE_DECL(sign, opname, ret, ... )                                          \
		namespace  detail                                                                     \
		{                                                                                     \
			template<typename T,typename... Args>                                             \
			struct has_ ## opname ## _helper                                                  \
			{                                                                                 \
				template<typename U, typename= fn_sfinae<ret (*)( Args...), &U::sign >>       \
				static constexpr bool choose(int) {return true;}                              \
				template<typename U>                                                          \
				static constexpr bool choose(...) {return false;}                             \
				static constexpr bool value = choose<T>(0);                                   \
			};                                                                                \
		}                                                                                     \
		template<typename T,typename... Args>                                                 \
		struct has_ ## opname:public bool_<detail::has_ ## opname ## _helper<T                \
			, __VA_ARGS__, Args...>::value>{};                                                \
		template<typename T,typename... Args>                                                 \
		static constexpr bool has_ ## opname ## _v = has_ ## opname<T,Args...>::value;

// T::type
#define CCDK_TT_HAS_INNER_DECL(name, inner_name)										   \
		namespace detail                                                                   \
		{                                                                                  \
			template<typename T>                                                           \
			struct has_inner_ ## name ## _helper                                           \
			{                                                                              \
				template<typename U, typename = typename U:: inner_name>                   \
				constexpr static  bool sfinae(int) { return true; };                       \
				template<typename U>                                                       \
				constexpr static  bool  sfinae(...){ return false; };                      \
				static constexpr bool value = sfinae<T>(0);                                \
			};                                                                             \
		}                                                                                  \
		template<typename T> struct has_inner_ ## name:                                    \
			public bool_< detail::has_inner_ ## name ## _helper<T>::value >{};             \
		template<typename T>                                                               \
			static constexpr bool has_inner_ ## name ## _v = has_inner_ ## name<T>::value;

namespace ccdk
{
	namespace mpl
	{
		template<typename T, T t> struct fn_sfinae {};
	}
}