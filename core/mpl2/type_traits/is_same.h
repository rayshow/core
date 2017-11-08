#pragma once

#include<core/mpl/bool_.h>

namespace Aurora3D
{
	namespace mpl
	{
		//ingore const volatile
		template<typename A,typename B> struct IsSame : public False_ {};
		template<typename T> struct IsSame<T, T> :public True_ {};
#define IsSameV(A,B)   (IsSame<A,B>::value)
#define NotSameV(A,B)  (!IsSame<A,B>::value)
		
	}
}