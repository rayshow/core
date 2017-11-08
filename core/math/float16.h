#pragma once

#include<core/type.h>


namespace Aurora3D
{
	namespace math
	{
		//half precision float
		union Float16
		{
			struct {
				uint32 tail : 10;
				uint32 exp :  5;
				uint32 sign : 1;
			} component;
			int16 iValue;
		};
	}
}