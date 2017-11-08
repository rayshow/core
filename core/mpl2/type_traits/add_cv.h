#pragma once

namespace Aurora3D
{
	namespace mpl
	{
		template<typename T> struct AddCV { typedef T const volatile type; };
		template<typename T> using AddCVT = typename AddCV<T>::type;
	}
}

