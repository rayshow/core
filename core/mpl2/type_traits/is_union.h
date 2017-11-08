#pragma once

#include<core/mpl/bool_.h>

namespace Aurora3D
{
	namespace mpl
	{
		template<typename T> struct IsUnion : Bool_<__is_union(T)> {};
		template<typename T> using IsUnion_t = typename IsUnion<T>::type;
#define IsUnionV(T)  (IsUnion<T>::value)
#define NotUnitV(T)  (!IsUnion<T>::value)
	}
}