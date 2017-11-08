#pragma once

#include<core/mpl/bool_.h>
#include<core/mpl/type_traits/remove_cv.h>

namespace Aurora3D
{
	namespace mpl
	{
		namespace detail
		{
			template<typename T> struct IsMemberPtrHelper :public False_ {};
			template<typename T, typename C> struct IsMemberPtrHelper<T C::*> :public True_ {};
		}
		template<typename T> struct IsMemberPtr :public detail::IsMemberPtrHelper<RemoveCVT<T>> {};
		
#define IsMemberPtrV(T)   (IsMemberPtr<T>::value)
#define NotMemberPtrV(T)  (!IsMemberPtr<T>::value)
	}
}