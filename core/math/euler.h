#pragma once

#include<core/type.h>
#include<core/math/math_fwd.h>
#include<core/math/float32.h>

namespace Aurora3D
{
	namespace math
	{
		struct Euler
		{
		public:
			float pitch;
			float yaw;
			float roll;
			
			CCDK_FORCEINLINE Euler();
			explicit CCDK_FORCEINLINE Euler(float p, float y, float r);
			explicit CCDK_FORCEINLINE Euler(const Quaternion& quat);

			CCDK_FORCEINLINE Euler operator+(const Euler& o) const;
			CCDK_FORCEINLINE Euler operator+=(const Euler& o);
			CCDK_FORCEINLINE Euler operator-(const Euler& o) const;
			CCDK_FORCEINLINE Euler operator-=(const Euler& o);
			CCDK_FORCEINLINE Euler operator*(float o) const;
			CCDK_FORCEINLINE Euler operator*=(float o);
			CCDK_FORCEINLINE Euler operator==(const Euler& o) const;
			CCDK_FORCEINLINE Euler operator!=(const Euler& o) const;

			CCDK_FORCEINLINE bool ContainNaN() const;
			CCDK_FORCEINLINE bool IsNearlyZero(float tolerance = kfMiddleEpiside) const;
			CCDK_FORCEINLINE bool IsZero() const;
			CCDK_FORCEINLINE bool Equals(const Euler& o,float tolerance = kfMiddleEpiside) const;
			CCDK_FORCEINLINE Euler Add(float dp, float dy, float dr);
			Vector3 ToVector3() const;
			Vector4 ToVector4() const;
			Quaternion ToQuaternion() const;
			Euler GetInverse() const;
			Vector3 RotateVector(const Vector3& v) const;
			Vector3 InverseRotateVector(const Vector3& v) const;
			Euler Clamp() const;
			Euler GetNormalized() const;
			void Normalize() const;
		};
	}
}