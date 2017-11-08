#pragma once

#include<core/mpl/logic_or.h>
#include<core/mpl/type_traits/is_lvalue_ref.h>
#include<core/mpl/type_traits/is_rvalue_ref.h>

namespace Aurora3D
{
	namespace mpl
	{
		//lazy 
		template<typename T> struct IsRef :public Or<IsLValueRef<T>, IsRValueRef<T>> {};
#define IsRefV(T)   (IsRef<T>::value)
#define NotRefV(T)  (!IsRef<T>::value)
	}
}