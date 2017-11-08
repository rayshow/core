#pragma once

#include<core/mpl/bool_.h>
#include<core/mpl/logic_or.h>
#include<core/mpl/type_traits/is_fundamental.h>
#include<core/mpl/type_traits/is_pointer.h>
#include<core/mpl/type_traits/is_member_ptr.h>
#include<core/mpl/type_traits/is_enum.h>
namespace Aurora3D
{
	namespace mpl
	{
		template<typename T> struct IsScalar :public Or< IsFundamental<T>, IsPointer<T>, IsMemberPtr<T>, IsEnum<T>> {};
#define IsScalarV(T)   (IsScalar<T>::value)
#define NotScalarV(T)  (!IsScalar<T>::value)
	}
} 