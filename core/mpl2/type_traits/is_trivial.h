#pragma once

#include<core/mpl/bool_.h>

namespace Aurora3D
{
	namespace mpl
	{
		// Is Trivial
		//1. every copy/move constructor or copy/move assignment is trivial or deleted(default constructor is trivial)
		//2. at least one copy/move constructor ¡¢copy/move assignment is't deleted
		//3. trivial destructor(not virtual,not user-provided(implicitly or explicitly defaulted))
		template<typename T> struct IsTrivial :public Bool_<__is_trivial(T)> {};
		
#define IsTrivialV(T)   (IsTrivial<T>::value)
#define NotTrivialV(T)  (!IsTrivial<T>::value)
	}
}