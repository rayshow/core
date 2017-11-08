#pragma once

#include<core/mpl/bool_.h>

namespace Aurora3D
{
	namespace mpl
	{
		template<typename T> struct IsVoid: public False_{};
		template<> struct IsVoid<void>:public True_{};
		template<> struct IsVoid<const void> :public True_ {};
		template<> struct IsVoid<volatile void> :public True_ {};
		template<> struct IsVoid<const volatile void> :public True_ {};


#define IsVoidV(T)   (IsVoid<T>::value)
#define NotVoidV(T)  (!IsVoid<T>::value)
	}
}