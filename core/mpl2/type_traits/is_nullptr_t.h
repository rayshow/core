#pragma once

#include<core/mpl/bool_.h>

namespace Aurora3D
{
	namespace mpl
	{
		template<typename T> struct IsNullptrType :public False_ {};
		template<> struct IsNullptrType<std::nullptr_t> :public True_ {};
		template<> struct IsNullptrType<const std::nullptr_t> :public True_ {};
		template<> struct IsNullptrType<volatile std::nullptr_t> :public True_ {};
		template<> struct IsNullptrType<const volatile std::nullptr_t> :public True_ {};
#define IsNullptrTypeV(T)   (IsNullptrType<T>::value)
#define NotNullptrTypeV(T)  (!IsNullptrType<T>::value)
	}
}