#pragma once

#include<core/type.h>
#if defined(AURORA3D_SSE)
#include<immintrin.h>  //for __m256 and function
#include<core/math/vector4.h>

namespace Aurora3D
{
	namespace math
	{
		union alignas(32) Vector4x2
		{
		public:
			__m256   vec4x2;
			Vector4  vec_array[2];

			Vector4x2(float F) :vec_array{ {F},{F} } {}
			Vector4x2(int32 U) :vec_array{ {U},{U} } {}
			Vector4x2(const Vector4& inv0, const Vector4& inv1):vec_array {inv0,inv1} {}
			Vector4x2(const Vector4x2& v):vec4x2(v.vec4x2) {}
			Vector4x2(const __m256& v) :vec4x2(vec4x2) {}
			__m256 operator+(const Vector4x2& v) { return _mm256_add_ps(vec4x2, v.vec4x2); }
			__m256 operator-(const Vector4x2& v) { return _mm256_sub_ps(vec4x2, v.vec4x2); }
			__m256 operator*(const Vector4x2& v) { return _mm256_mul_ps(vec4x2, v.vec4x2); }
			__m256 operator/(const Vector4x2& v) { return _mm256_div_ps(vec4x2, v.vec4x2); }
			__m256 operator%(const Vector4x2& v) { return _mm256_sub_ps(vec4x2, v.vec4x2); }
			__m256 operator&(const Vector4x2& v) { return _mm256_and_ps(vec4x2, v.vec4x2); }
			__m256 operator|(const Vector4x2& v) { return _mm256_or_ps(vec4x2, v.vec4x2); }
			__m256 operator^(const Vector4x2& v) { return _mm256_xor_ps(vec4x2, v.vec4x2); }
			__m256 operator!() { return _mm256_andnot_ps(vec4x2, vec4x2); }
			__m256 AndNot(const Vector4x2& v) { return _mm256_andnot_ps(vec4x2, v.vec4x2); }
			__m256 operator>(const Vector4x2& v) { return _mm256_cmp_ps(vec4x2, v.vec4x2, 0); }

		};
	}
}

#endif