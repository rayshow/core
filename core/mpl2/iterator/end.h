#pragma once

#include<core/mpl/int_.h>
#include<core/mpl/category.h>

namespace Aurora3D
{
	namespace mpl
	{
		//for type trait
		template<typename S> struct End :public S::end {};
	}
}