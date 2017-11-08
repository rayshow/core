#pragma once

//vector forward declare
#include<Core/mpl/container/vector_decl.h>
#include<Core/mpl/container/set_decl.h>

namespace Aurora3D
{
	namespace mpl
	{
		//depends on Vector_
		template<typename... Args> struct Set_:public Vector_<Args...>{};
	}
}