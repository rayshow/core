#pragma once

#include<core/type.h>
#include"vector4.h"

namespace Aurora3D
{
	namespace math
	{
		struct Color
		{
		public:
			uint8 r, g, b, a;
			static const Color Zero;
			A3D_FORCEINLINE const uint8* GetBuffer() const;
			A3D_FORCEINLINE uint8 GetBuffer();
		};

		class LinearColor4 
		{
		public:

		};
	}
	
}