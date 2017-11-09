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
#define BinaryNotMatchAddSubV(p, t)      false //\ 
		/*( is_pointer_v(p) && is_pointer_v(t))  \
	    || ( is_pointer_v(p) &&  IsFundamentalV(t)  && ( IsVoidV(RemovePointerT<p>)|| NotIntegralV(t)) ) \
		|| ( is_pointer_v(t) &&  IsFundamentalV(p)  && ( IsVoidV(RemovePointerT<t>)|| NotIntegralV(p)) )*/
		 
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

