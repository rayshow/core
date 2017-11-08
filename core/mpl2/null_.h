#pragma once

#include<core/mpl/bool_.h>

namespace Aurora3D
{
	namespace mpl
	{
		struct Null_ {};
		template<typename T> struct IsNull_ :public False_ {};
		template<>struct IsNull_<Null_> :public True_ {};

#define IsNullV(T)  (IsNone<T>::value)
#define NotNullV(T) (!IsNone<T>::value)
	}
}