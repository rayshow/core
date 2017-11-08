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
			
			A3D_FORCEINLINE Euler();
			explicit A3D_FORCEINLINE Euler(float p, float y, float r);
			explicit A3D_FORCEINLINE Euler(const Quaternion& quat);

			A3D_FORCEINLINE Euler operator+(const Euler& o) const;
			A3D_FORCEINLINE Euler operator+=(const Euler& o);
			A3D_FORCEINLINE Euler operator-(const Euler& o) const;
			A3D_FORCEINLINE Euler operator-=(const Euler& o);
			A3D_FORCEINLINE Euler operator*(float o) const;
			A3D_FORCEINLINE Euler operator*=(float o);
			A3D_FORCEINLINE Euler operator==(const Euler& o) const;
			A3D_FORCEINLINE Euler operator!=(const Euler& o) const;

			A3D_FORCEINLINE bool ContainNaN() const;
			A3D_FORCEINLINE bool IsNearlyZero(float tolerance = kfMiddleEpiside) const;
			A3D_FORCEINLINE bool IsZero() const;
			A3D_FORCEINLINE bool Equals(const Euler& o,float tolerance = kfMiddleEpiside) const;
			A3D_FORCEINLINE Euler Add(float dp, float dy, float dr);
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