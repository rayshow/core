#pragma once

namespace core
{
	namespace mpl
	{
		//same as bottom-const
		template<typename T> struct add_cv { typedef T const volatile type; };
		template<typename T> using add_cv_t = typename add_cv<T>::type;
	}
}

