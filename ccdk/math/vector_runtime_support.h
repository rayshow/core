#pragma once

#include<ccdk/type.h>

namespace Aurora3D
{
	namespace math
	{
		namespace VectorSupport
		{
			enum class VectorLevel :uint8
			{
				NotSupport,    //for some phone, use FPU instead
				SSE,           //support SSE 1,2
				SSE4,          //support SSE 4.1, 8 float16 support, 16 byte support
				AVX,           //8 float operation support
				AVX2,          //32 uint8, 16 float16 support
			};
			//runtime support check
			CCDK_API static VectorLevel SupportLevel = VectorLevel::SSE;
			CCDK_API static bool  SupportOpenCL = false;

		}
	}
}