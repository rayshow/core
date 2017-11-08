#pragma once

#include<core/mpl/bool_.h>
#include<core/mpl/type_traits/is_same.h>
#include<core/mpl/type_traits/remove_cv.h>
namespace Aurora3D
{
	namespace mpl
	{
		//ingore const volatile and &,&&
		template<typename A, typename B> struct IsBaseSame : public IsSame< typename RemoveCV<A>::type &, typename RemoveCV<B>::type &> {};

#define IsBaseSameV(A,B)  (IsBaseSame<A,B>::value)
#define NotBaseSameV(A,B) (!IsBaseSame<A,B>::value)

	}
}