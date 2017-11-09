#pragma once

#include<core/mpl/type_traits/is_pointer.h>
#include<core/mpl/type_traits/remove_pointer.h>
#include<core/mpl/type_traits/is_fundamental.h>

namespace Aurora3D
{
	namespace mpl
	{
		//non-void-pointer can't op with non-int type

		//forbidden
		//fundemental op void*
		//fundemental(no integral) op pointer
		//pointer + pointer
		//+,-
#define A3D_TT_ADD_SUB_NOT_MATCH(L, R)                  \
		( ( is_pointer_v<L> && is_pointer_v<R> )        \
	    || ( is_pointer_v<L> &&  is_fundamental_v<R>  && ( is_void_v< remove_pointer_t<L>> || !is_integer_v<R> ) ) \
		|| ( is_pointer_v<R> &&  is_fundamental_v<L>  && ( is_void_v< remove_pointer_t<R>> || !is_integer_v<L> ) ) )
		 
		//forbidden
		//fundemental op pointer
		//fundemental op fundemetal (nor one is integral)
		//pointer op pointer
		//*,/,%,>,>=,<,<=,==
//#define BinaryNotMatchArithmeticLogicV(p,t)   ( IsPointerV(p) && IsPointerV(t) )                  \
//			|| (IsFundamentalV(p) && (IsPointerV(t) || (IsFundamentalV(t) && NotIntegralV(t)) )) \
//			|| (IsFundamentalV(t) && (IsPointerV(p) || (IsFundamentalV(p) && NotIntegralV(p)) ))
	}
}

