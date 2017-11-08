#pragma once

#include<core/math/math_fwd.h>
#include<core/math/vector4.h>

namespace Aurora3D
{
	namespace math
	{
		//_MM_TRANSPOSE4_PS (__m128 row0, __m128 row1, __m128 row2, __m128 row3)
		struct alignas(16) Matrix4
		{
		public:
			Vector4 m0{ 1.0f,0.0f,0.0f,0.0f };
			Vector4 m1{ 0.0f,1.0f,0.0f,0.0f };
			Vector4 m2{ 0.0f,0.0f,1.0f,0.0f };
			Vector4 m3{ 0.0f,0.0f,0.0f,1.0f };

			//default 
			Matrix4() = default;
			A3D_FORCEINLINE Matrix4(int row,const Vector4& v){ (&m0)[row] = v; };
			A3D_FORCEINLINE Matrix4(const Vector4& r0, const Vector4& r1, const Vector4& r2, const Vector4& r3): m0(r0),m1(r1),m2(r2),m3(r3){}
			A3D_FORCEINLINE const Vector4& operator[](uint32 index) { return (&m0)[index]; }

			std::string ToString()
			{
				return m0.ToString() + "\n";// +m1.ToString() + "\n" + m2.ToString() + "\n" + m3.ToString() + "\n";
			}

		};
	}
	



}