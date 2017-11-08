#pragma once

#include<core/mpl/bool_.h>
#include<core/mpl/type_traits/remove_cv.h>

namespace Aurora3D
{
	namespace mpl
	{
		namespace detail
		{
			template<typename T> struct IsPointerHelper :public False_ {};
			template<typename T> struct IsPointerHelper<T*> :public True_ {};
		}

		//for non-member-pointer
		template<typename T> struct IsPointer :public detail::IsPointerHelper< typename RemoveCV<T>::type> {};
#define IsPointerV(T)    (IsPointer<T>::value)
#define NotPointerV(T)   (!IsPointer<T>::value)
		
	}
}