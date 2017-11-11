#pragma once

#include<core/mpl/type_traits/is_pointer.h>
#include<core/mpl/type_traits/is_fundamental.h>
#include<core/mpl/type_traits/is_base_of.h>
#include<core/mpl/type_traits/is_integer.h>
#include<core/mpl/type_traits/is_convertible.h>

namespace core
{
	namespace mpl
	{
		//non-void-pointer can't op with non-int type

		//forbidden op + - += -=
		//fundemental op void*
		//fundemental(no integral) op pointer
		//pointer + pointer
#define A3D_TT_ADD_SUB_NOT_MATCH(L, R, LNP, RNP)                  \
		( ( is_pointer_v<L> && is_pointer_v<R> )                  \
	    || ( is_pointer_v<L> &&  is_fundamental_v<R> )            \
		|| ( is_pointer_v<R> &&  is_fundamental_v<L> ) )


		//forbidden op * / % *= /= %/
		//T* op P* or fundemental
#define A3D_TT_MUL_DIV_MOD_NOT_MATCH(L,R)                                   \
		(( is_pointer_v<L> && (is_pointer_v<R> || is_fundamental_v<R>)) ||  \
		 ( is_pointer_v<R> && (is_pointer_v<L> || is_fundamental_v<L>)) )


		//forbindden op < <= == >= >
		//T* op P* (and T and P no derive reletion)
		//T* op fundamental
#define A3D_TT_CMP_NOT_MATCH(L,R, LNP, RNP) \
		( ( is_pointer_v<L> && is_fundamental_v<R> ) ||                    \
		  ( is_pointer_v<R> && is_fundamental_v<L> ) ||                    \
		  ( is_pointer_v<L> && is_pointer_v<R> &&                          \
		   !(is_base_of_v< LNP, RNP > || is_base_of_v< RNP, LNP >)         \
		  ))

		// forbidden op & | ^ >> << |= &= ^= >>= <<=
		// T* op P* 
		// T* op fundamental
		// integer op no-integer fundamental
#define A3D_PP_BIT_OP_NOT_MATCH(L,R)  \
		( ( is_pointer_v<L> && (is_pointer_v<R> || is_fundamental_v<R> ))  ||  \
		  ( is_pointer_v<L> && (is_pointer_v<R> || is_fundamental_v<R>))   ||  \
		  ( is_integer_v<L> && is_fundamental_v<R> && !is_integer_v<R> )   ||  \
		  ( is_integer_v<R> && is_fundamental_v<L> && !is_integer_v<L> ))

#define A3D_PP_LOGIC_NOT_MATCH(L,R) \
		( (is_pointer_v<L> && !is_convertible_v<R, bool> ) || \
		  (is_pointer_v<R> && !is_convertible_v<L, bool>))
	}
}

