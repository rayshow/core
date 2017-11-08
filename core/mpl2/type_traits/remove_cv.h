#pragma once
#include"remove_const.h"
#include"remove_volatile.h"

namespace Aurora3D
{
	namespace mpl
	{
		namespace detail
		{
			template<typename T> struct RemoveCVHelper :public RemoveVolatile<typename T::type> {};
		}

		//lazy
		//remove all bottom-level const/volatile qualifers
		template<typename T> struct RemoveCV :public detail::RemoveCVHelper<RemoveConst<T>>{};
		template<typename T> using RemoveCVT = typename RemoveCV<T>::type;

	}
}
