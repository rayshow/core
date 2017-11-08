#pragma once

namespace Aurora3D
{
	namespace mpl
	{
		//T* => volatile T* 
		//T  => volatile T
		//default volatile + T& => T&
		template<typename T> struct AddVolatile { typedef T volatile type; };
		template<typename T> using AddVolatileT = typename AddVolatile<T>::type;

	}
}

