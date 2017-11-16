#pragma once

#include<array>
#include<string>
#include<cassert>
#include<iostream>
#include<initializer_list>
#include<core/math/math_impl.h>
#include<core/auxililty/ingore_unused.h>

#pragma warning(disable:4996)

namespace Aurora3D
{
	namespace math
	{
		//void _mm_prefetch (char const* p, int i)
		//void _mm_sfence (void)
		//float128 wrap type

		CCDK_API union alignas(16) Vector4
		{
		public:
			float  fArray[4];
			int32  iArray[4];
#if defined(CCDK_SSE) || defined(CCDK_NEON) 
			float128 vec;
#endif
			//constexpr 
			static constexpr struct InitPoint2D {}   Point2D{};
			static constexpr struct InitLine2D {}    Line2D{};
			static constexpr struct InitPoint3D {}   Point3D{};
			static constexpr struct InitLine3D {}    Line3D{};

			/**  construction*/
			
			// (0.0f, 0.0f, 0.0f, 0.0f)
			CCDK_FORCEINLINE constexpr Vector4() : vec{ math_impl::kVectorZero } {}

			// (inx, iny, inz, inw), general construction
			CCDK_FORCEINLINE constexpr Vector4(float inx, float iny, float inz = 0.0f, float inw = 0.0f) : fArray{ inx, iny, inz, inw } {}

			// (inux, inuy, inuz, inuw), uint32 construction
			CCDK_FORCEINLINE constexpr Vector4(int32 inux, int32 inuy, int32 inuz = 0u, int32 inuw = 0u) : iArray{ inux, inuy, inuz, inuw } {}

			// (inx, iny, inz, 1.0f)
			CCDK_FORCEINLINE constexpr Vector4(InitPoint3D, float inx, float iny, float inz) :Vector4(inx, iny, inz, 1.0f) {}

			// (inx, iny, inz, 0.0f), avoid to be a divend, or will cause a div zero error
			CCDK_FORCEINLINE constexpr Vector4(InitLine3D, float inx, float iny, float inz) : Vector4(inx, iny, inz, 0.0f) {}

			// (inx, inty, 1.0f, 1.0f) do 2d vector caculation,set w 0 to avoid div zero
			CCDK_FORCEINLINE constexpr Vector4(InitPoint2D, float inx, float iny) : Vector4(inx, iny, 1.0f, 1.0f) {}

			// (inx, inty, 0.0f, 1.0f) do 2d vector caculation, avoid to be a divend
			CCDK_FORCEINLINE constexpr Vector4(InitLine2D, float inx, float iny) : Vector4(inx, iny, 0.0f, 1.0f) {}

			// (F, F, F, F), make float implcitly convert to a Vector4
			CCDK_FORCEINLINE constexpr Vector4(float  F) : Vector4(F, F, F, F) {};

			// (mask, mask, mask, mask), element is uint, make uint32 implcitly convert to a Vector4
			CCDK_FORCEINLINE constexpr Vector4(int32 I) :Vector4(I,I,I,I) {};

			// copy constructor
			CCDK_FORCEINLINE constexpr Vector4(const Vector4&  v) : fArray{ v.fArray[0], v.fArray[1], v.fArray[2],v.fArray[3] } { }

			// construct from simd float128
			CCDK_FORCEINLINE constexpr  Vector4(const float128& v) : vec(v) { }

			// assign from simd float128 bit
			CCDK_FORCEINLINE constexpr Vector4& operator=(const float128& v) { vec = v; return *this; }

			// assign from other Vector4
			CCDK_FORCEINLINE constexpr Vector4& operator=(const Vector4& v) { vec = v.vec; return *this; }

			/**
			 *   elements function
			 */
			// normal Vector4 variant get nth float data, nth <=3
			CCDK_FORCEINLINE float&  operator[](uint32 inIndex) { assert(inIndex <= 3); return fArray[inIndex]; }

			// const Vector4 variant get nth float data, nth <=3
			CCDK_FORCEINLINE float   operator[](uint32 inIndex) const { assert(inIndex <= 3); return fArray[inIndex]; }

			// re-order or repeat elements 
			template<uint32 p1, uint32 p2, uint32 p3, uint32 p4> CCDK_FORCEINLINE Vector4 Reorder() const
			{
				static_assert(p1 < 4 && p2 < 4 && p3 < 4 && p4 < 4, "index out of range.");
				return Vector4{ fArray[p1], fArray[p2], fArray[p3], fArray[p4] };
			}

			// re-order with other Vector
			template<uint32 v00 = 0, uint32 v01 = 1, uint32 v10 = 2, uint32 v11 = 3>
			CCDK_FORCEINLINE Vector4 PickCompose(const Vector4& v) const
			{
				static_assert(v00 < 4 && v01 < 4 && v10 < 4 && v11 < 4, "index out of range.");
				return Vector4{ fArray[v00], fArray[v01], v.fArray[v10], v.fArray[v11] };
			}

			//return a copy of this vector
			CCDK_FORCEINLINE Vector4 Copy() const { return Vector4{ vec }; }

			/** element reorder like shader vector4 */
			CCDK_FORCEINLINE Vector4 xxxx() const { float repeat = fArray[0]; return Vector4{ repeat,repeat ,repeat ,repeat }; }
			CCDK_FORCEINLINE Vector4 yyyy() const { float repeat = fArray[1]; return Vector4{ repeat,repeat ,repeat ,repeat }; }
			CCDK_FORCEINLINE Vector4 zzzz() const { float repeat = fArray[2]; return Vector4{ repeat,repeat ,repeat ,repeat }; }
			CCDK_FORCEINLINE Vector4 wwww() const { float repeat = fArray[3]; return Vector4{ repeat,repeat ,repeat ,repeat }; }
			CCDK_FORCEINLINE Vector4 xywz() const { return Reorder<0, 1, 3, 2>(); }
			CCDK_FORCEINLINE Vector4 xzyw() const { return Reorder<0, 2, 1, 3>(); }
			CCDK_FORCEINLINE Vector4 xzwy() const { return Reorder<0, 2, 3, 1>(); }
			CCDK_FORCEINLINE Vector4 xwyz() const { return Reorder<0, 3, 1, 2>(); }
			CCDK_FORCEINLINE Vector4 xwzy() const { return Reorder<0, 3, 2, 1>(); }
			CCDK_FORCEINLINE Vector4 yxzw() const { return Reorder<1, 0, 2, 3>(); }
			CCDK_FORCEINLINE Vector4 yxwz() const { return Reorder<1, 0, 3, 2>(); }
			CCDK_FORCEINLINE Vector4 yzxw() const { return Reorder<1, 2, 0, 3>(); }
			CCDK_FORCEINLINE Vector4 yzwx() const { return Reorder<1, 2, 3, 0>(); }
			CCDK_FORCEINLINE Vector4 ywxz() const { return Reorder<1, 3, 0, 2>(); }
			CCDK_FORCEINLINE Vector4 ywzx() const { return Reorder<1, 3, 2, 0>(); }
			CCDK_FORCEINLINE Vector4 zxyw() const { return Reorder<2, 0, 1, 3>(); }
			CCDK_FORCEINLINE Vector4 zxwy() const { return Reorder<2, 0, 3, 1>(); }
			CCDK_FORCEINLINE Vector4 zyxw() const { return Reorder<2, 1, 0, 3>(); }
			CCDK_FORCEINLINE Vector4 zywx() const { return Reorder<2, 1, 3, 0>(); }
			CCDK_FORCEINLINE Vector4 zwxy() const { return Reorder<2, 3, 0, 1>(); }
			CCDK_FORCEINLINE Vector4 zwyx() const { return Reorder<2, 3, 1, 0>(); }
			CCDK_FORCEINLINE Vector4 wxyz() const { return Reorder<3, 0, 1, 2>(); }
			CCDK_FORCEINLINE Vector4 wxzy() const { return Reorder<3, 0, 2, 1>(); }
			CCDK_FORCEINLINE Vector4 wyxz() const { return Reorder<3, 1, 0, 2>(); }
			CCDK_FORCEINLINE Vector4 wyzx() const { return Reorder<3, 1, 2, 0>(); }
			CCDK_FORCEINLINE Vector4 wzxy() const { return Reorder<3, 2, 0, 1>(); }
			CCDK_FORCEINLINE Vector4 wzyx() const { return Reorder<3, 2, 1, 0>(); }

			// (-x, -y, -z, -w)
			CCDK_FORCEINLINE Vector4 operator-() const;

			// for i:0-3  result[i] = (*this)[i] + v[i]
			CCDK_FORCEINLINE Vector4 operator+(const Vector4& v) const;

			// for i:0-3  result[i] = (*this)[i] - v[i]
			CCDK_FORCEINLINE Vector4 operator-(const Vector4& v) const;

			// for i:0-3  result[i] = (*this)[i] * v[i]
			CCDK_FORCEINLINE Vector4 operator*(const Vector4& v) const;

			// for i:0-3  result[i] = (*this)[i] / v[i]
			CCDK_FORCEINLINE Vector4 operator/(const Vector4& v) const;

			// for i:0-3  result[i] = (*this)[i] % v[i]
			CCDK_FORCEINLINE Vector4 operator%(const Vector4& v) const { return Copy().AssignMod(v); }

			// for i:0-3  result[i] = (*this)[i] | v[i]
			CCDK_FORCEINLINE Vector4 operator|(const Vector4& v) const;

			// for i:0-3  result[i] = (*this)[i] & v[i]
			CCDK_FORCEINLINE Vector4 operator&(const Vector4& v) const;

			// for i:0-3  result[i] = (*this)[i] ^ v[i]
			CCDK_FORCEINLINE Vector4 operator^(const Vector4& v) const;

			// for i:0-3  result[i] = ~(*this)[i];
			CCDK_FORCEINLINE Vector4 operator~() const { return Copy().AssignBitNot(); }

			// for i:0-3  result[i] = (~(*this)[i]) ^ v[i]
			CCDK_FORCEINLINE Vector4 BitNotAnd(const Vector4& v) const;

			// for i:0-3  result[i] = (*this)[i] > v[i] ? 0xffffffff : 0
			CCDK_FORCEINLINE Vector4 operator>(const Vector4& v) const;

			// for i:0-3  result[i] = (*this)[i] >= v[i] ? 0xffffffff : 0
			CCDK_FORCEINLINE Vector4 operator>=(const Vector4& v) const;

			// for i:0-3  result[i] = (*this)[i] < v[i] ? 0xffffffff : 0
			CCDK_FORCEINLINE Vector4 operator<(const Vector4& v) const;

			// for i:0-3  result[i] = (*this)[i] <= v[i] ? 0xffffffff : 0
			CCDK_FORCEINLINE Vector4 operator<=(const Vector4& v) const;

			// for i:0-3  result[i] = (*this)[i] == v[i] ? 0xffffffff : 0
			CCDK_FORCEINLINE Vector4 operator==(const Vector4& v) const;

			// for i:0-3  result[i] = (*this)[i] != v[i] ? 0xffffffff : 0
			CCDK_FORCEINLINE Vector4 operator!=(const Vector4& v) const;

			//integral part of each component
			CCDK_FORCEINLINE Vector4 IntPart() const;

			//fraction part of each component
			CCDK_FORCEINLINE Vector4 FracPart() const { return Copy().AssignFracPart(); }

			CCDK_FORCEINLINE Vector4 Sign() const { return Copy().AssignSign(); };

			//convert to nearly int
			CCDK_FORCEINLINE Vector4 Round() const;

			// for i:0-3  result[i] = min( (*this)[i], v[i] )
			CCDK_FORCEINLINE Vector4 Min(const Vector4& v) const;

			// for i:0-3  result[i] = max( (*this)[i], v[i] )
			CCDK_FORCEINLINE Vector4 Max(const Vector4& v) const;

			//accuracy lost less then 0.00036 with AccurateRcp, 1/2 Latency of AccurateRcp
			CCDK_FORCEINLINE Vector4 FastRcp()const;

			//(1/x,1/y,1/z,1/w)
			CCDK_FORCEINLINE Vector4 AccurateRcp()const { return Copy().AssignAccurateRcp(); }

			//(x^0.5,w^0.5,w^0.5,w^0.5)
			CCDK_FORCEINLINE float128 Sqrt()const;

			//accuracy lost less then 0.00036 with AccurateRcpSqrt, 1/5 Latency of AccurateRcpSqrt
			CCDK_FORCEINLINE Vector4 FastRcpSqrt()const;

			//no accuracy lose return 1/v^0.5
			CCDK_FORCEINLINE Vector4 AccurateRcpSqrt()const { return Copy().AssignAccurateRcpSqrt(); }

			// for bit:0-127 if bit ==1 get v1's bit,else get v2's
			CCDK_FORCEINLINE Vector4 Select(const Vector4& v1, const Vector4& v2) const { return Copy().AssignSelect(v1, v2); }

			// (|x|,|y|,|z|,|w|)
			CCDK_FORCEINLINE Vector4 Abs() const { return Copy().AssignAbs(); }

			// for i:0-3  result[i] = *this[i]>v[i]? 1.0f : 0.0f;
			CCDK_FORCEINLINE Vector4 Step(const Vector4& v) const { return Copy().AssignStep(v); }

			// for i:0-3  result[i] = *this[i]<v[i]? 1.0f : 0.0f;
			CCDK_FORCEINLINE Vector4 RStep(const Vector4& v) const { return Copy().AssignRStep(v); }

			// for i:0-3  result[i] = *this[i]>1.0f ? 1.0f : 0.0f;
			CCDK_FORCEINLINE Vector4 StepOne() const { return Copy().AssignStepOne(); }

			// for i:0-3  result[i] = *this[i]<1.0f ? 1.0f : 0.0f;
			CCDK_FORCEINLINE Vector4 RStepOne() const { return Copy().AssignRStepOne(); }

			//convert to lower int
			CCDK_FORCEINLINE Vector4 Floor() const { return Copy().AssignFloor(); }

			//convert to higner int
			CCDK_FORCEINLINE Vector4 Ceil() const { return Copy().AssignCeil(); }
			
			//each element clamped within the range of 0 to 1.0.
			CCDK_FORCEINLINE Vector4 Saturate() const { return Copy().AssignSaturate(); }

			// result.xy = x+y
			CCDK_FORCEINLINE Vector4 Sumup2() const { return Copy().AssignSumup2(); }

			// result.xyz = x+y+z
			CCDK_FORCEINLINE Vector4 Sumup3() const { return Copy().AssignSumup3(); }

			// result.xyzw = x+y+z+w
			CCDK_FORCEINLINE Vector4 Sumup4() const { return Copy().AssignSumup4(); }

			// assume stored radian and convert to degree
			CCDK_FORCEINLINE Vector4 ToDegree() const { return Copy().AssignToDegree(); }

			// assume stored degree and convert to radian
			CCDK_FORCEINLINE Vector4 ToRadian() const { return Copy().AssignToRadian(); }

			// do 2d normalize, result result.xy =  xy / (x^2 + y^2)^0.5
			CCDK_FORCEINLINE Vector4 Normalize2() const { return Copy().AssignNormalize2(); }

			// do 3d normalize, result result.xy =  xyz / (x^2 + y^2 + z^2)^0.5
			CCDK_FORCEINLINE Vector4 Normalize3() const { return Copy().AssignNormalize3(); }

			// do 4d normalize, result result.xy =  xyzw / (x^2 + y^2 + z^2 + w^2)^0.5
			CCDK_FORCEINLINE Vector4 Normalize4() const { return Copy().AssignNormalize4(); }

			// return sin(this->xyzw)
			CCDK_FORCEINLINE Vector4 Sin() const { return Copy().AssignSin(); }

			// return cos(this->xyzw)
			CCDK_FORCEINLINE Vector4 Cos() const { return Copy().AssignCos(); }

			// return sin(this->xyzw)
			CCDK_FORCEINLINE Vector4 Dot2(const  Vector4& v) const { return Copy().AssignDot2(v); }
			CCDK_FORCEINLINE Vector4 Dot3(const  Vector4& v) const { return Copy().AssignDot3(v); }
			CCDK_FORCEINLINE Vector4 Dot4(const  Vector4& v) const { return Copy().AssignDot4(v); }
			CCDK_FORCEINLINE Vector4 Reflect3(const Vector4& normal) const { return Copy().AssignReflect3(normal); }
			CCDK_FORCEINLINE Vector4 Reflect2(const Vector4& normal) const { return Copy().AssignReflect2(normal); }
			CCDK_FORCEINLINE Vector4 LengthSQ4() const { return Copy().AssignLengthSQ4(); }
			CCDK_FORCEINLINE Vector4 LengthSQ3() const { return Copy().AssignLengthSQ3(); }
			CCDK_FORCEINLINE Vector4 LengthSQ2() const { return Copy().AssignLengthSQ2(); }
			CCDK_FORCEINLINE Vector4 Length4() const { return Copy().AssignLength4(); }
			CCDK_FORCEINLINE Vector4 Length3() const { return Copy().AssignLength3(); }
			CCDK_FORCEINLINE Vector4 Length2() const { return Copy().AssignLength2(); }
			CCDK_FORCEINLINE Vector4 DistanceSQ4(const Vector4& v) const { return Copy().AssignDistanceSQ4(v); }
			CCDK_FORCEINLINE Vector4 DistanceSQ3(const Vector4& v) const { return Copy().AssignDistanceSQ3(v); }
			CCDK_FORCEINLINE Vector4 DistanceSQ2(const Vector4& v) const { return Copy().AssignDistanceSQ2(v); }
			CCDK_FORCEINLINE Vector4 Distance4(const Vector4& v) const { return Copy().AssignDistance4(v); }
			CCDK_FORCEINLINE Vector4 Distance3(const Vector4& v) const { return Copy().AssignDistance3(v); }
			CCDK_FORCEINLINE Vector4 Distance2(const Vector4& v) const { return Copy().AssignDistance2(v); }
			CCDK_FORCEINLINE Vector4 NearlyEquals(const Vector4& v, float tolerence = kfMiddleEpiside) const { Copy().AssignNearlyEqual(v, tolerence); }
			CCDK_FORCEINLINE Vector4 Lerp(const Vector4& to, float t) const { return Copy().AssignLerp(to, t); }
			CCDK_FORCEINLINE Vector4 Cross(const Vector4& v) const { return Copy().AssignCross(v); }
			CCDK_FORCEINLINE Vector4 Clamp(const Vector4& min, const Vector4& max) const { return Copy().AssignClamp(min, max); }

			//chain operations, no temp Vector4 object generated, efficient then no-assign function when equal is complex
			CCDK_FORCEINLINE  Vector4& Assign(const  Vector4& v) { vec = v.vec; return *this; }
			CCDK_FORCEINLINE  Vector4& AssignNegate() { vec = (-*this).vec; return *this; }
			CCDK_FORCEINLINE  Vector4& AssignSign();
			CCDK_FORCEINLINE  Vector4& AssignAdd(const  Vector4& v) { vec = (*this + v).vec; return *this; }
			CCDK_FORCEINLINE  Vector4& AssignSub(const  Vector4& v) { vec = (*this - v).vec; return *this; }
			CCDK_FORCEINLINE  Vector4& AssignMul(const  Vector4& v) { vec = (*this * v).vec; return *this; }
			CCDK_FORCEINLINE  Vector4& AssignDiv(const  Vector4& v) { vec = (*this / v).vec; return *this; }
			CCDK_FORCEINLINE  Vector4& AssignMod(const  Vector4& v) { return AssignSub(Copy().AssignDiv(v).AssignIntPart().AssignMul(v)); }
			CCDK_FORCEINLINE  Vector4& AssignBitAnd(const Vector4& v) { vec = (*this & v).vec; return *this; }
			CCDK_FORCEINLINE  Vector4& AssignBitOr(const  Vector4& v) { vec = (*this | v).vec; return *this; }
			CCDK_FORCEINLINE  Vector4& AssignBitXor(const Vector4& v) { vec = (*this ^ v).vec; return *this; }
			CCDK_FORCEINLINE  Vector4& AssignBitNot();
			CCDK_FORCEINLINE  Vector4& AssignBitNotAnd(const Vector4& v) { vec = BitNotAnd(v).vec; return *this; }
			CCDK_FORCEINLINE  Vector4& operator+=(const Vector4& v) { return AssignAdd(v); }
			CCDK_FORCEINLINE  Vector4& operator-=(const Vector4& v) { return AssignSub(v); }
			CCDK_FORCEINLINE  Vector4& operator*=(const Vector4& v) { return AssignMul(v); }
			CCDK_FORCEINLINE  Vector4& operator/=(const Vector4& v) { return AssignDiv(v); }
			CCDK_FORCEINLINE  Vector4& operator%=(const Vector4& v) { return AssignMod(v); }
			CCDK_FORCEINLINE  Vector4& operator|=(const Vector4& v) { return AssignBitOr(v); }
			CCDK_FORCEINLINE  Vector4& operator&=(const Vector4& v) { return AssignBitAnd(v); }
			CCDK_FORCEINLINE  Vector4& operator^=(const Vector4& v) { return AssignBitXor(v); }
			CCDK_FORCEINLINE  Vector4& AssignAbs();
			CCDK_FORCEINLINE  Vector4& AssignIntPart() { vec = IntPart().vec; return *this; }
			CCDK_FORCEINLINE  Vector4& AssignFracPart() { Vector4 cp{ vec }; return AssignSub(cp.AssignIntPart()); }
			CCDK_FORCEINLINE  Vector4& AssignRound() { vec = Round().vec; return *this; }
			CCDK_FORCEINLINE  Vector4& AssignFloor() { Vector4 copy{ vec };  return AssignIntPart().AssignSub(copy.AssignRStepOne()); }
			CCDK_FORCEINLINE  Vector4& AssignCeil() { Vector4 copy{ vec };  return AssignIntPart().AssignAdd(copy.AssignStepOne()); };
			CCDK_FORCEINLINE  Vector4& AssignFastRcp() { vec = FastRcp().vec; return *this; }
			CCDK_FORCEINLINE  Vector4& AssignAccurateRcp();
			CCDK_FORCEINLINE  Vector4& AssignSqrt() { vec = Sqrt(); return *this; }
			CCDK_FORCEINLINE  Vector4& AssignFastRcpSqrt() { vec = FastRcpSqrt().vec; return *this; }
			CCDK_FORCEINLINE  Vector4& AssignAccurateRcpSqrt();
			CCDK_FORCEINLINE  Vector4& AssignMin(const Vector4& v) { vec = Min(v).vec; return *this; }
			CCDK_FORCEINLINE  Vector4& AssignMax(const Vector4& v) { vec = Max(v).vec; return *this; }
			CCDK_FORCEINLINE  Vector4& AssignGreater(const  Vector4& v) { vec = (*this > v).vec; return *this; }
			CCDK_FORCEINLINE  Vector4& AssignGreaterEqual(const  Vector4& v) { vec = (*this >= v).vec; return *this; }
			CCDK_FORCEINLINE  Vector4& AssignLess(const  Vector4& v) { vec = (*this < v).vec; return *this; }
			CCDK_FORCEINLINE  Vector4& AssignLessEqual(const  Vector4& v) { vec = (*this <= v).vec; return *this; }
			CCDK_FORCEINLINE  Vector4& AssignAbsoluteEqual(const  Vector4& v) { vec = (*this == v).vec; return *this; }
			CCDK_FORCEINLINE  Vector4& AssignNotEqual(const  Vector4& v) { vec = (*this != v).vec; return *this; }
			CCDK_FORCEINLINE  Vector4& AssignNearlyEqual(const  Vector4& v, float tolerence = kfMiddleEpiside) { return  AssignSub(v).AssignAbs().AssignLess(tolerence); }
			CCDK_FORCEINLINE  Vector4& AssignDot2(const  Vector4& v) { return AssignMul(v).AssignSumup2(); }
			CCDK_FORCEINLINE  Vector4& AssignDot3(const  Vector4& v) { return AssignMul(v).AssignSumup3(); }
			CCDK_FORCEINLINE  Vector4& AssignDot4(const  Vector4& v) { return AssignMul(v).AssignSumup4(); };
			CCDK_FORCEINLINE  Vector4& AssignCross(const Vector4& v) { return Assign(yzxw() * v.zxyw() - zxyw() * v.yzxw()); }
			CCDK_FORCEINLINE  Vector4& AssignLengthSQ4() { return AssignDot4(*this); }
			CCDK_FORCEINLINE  Vector4& AssignLengthSQ3() { return AssignDot3(*this); }
			CCDK_FORCEINLINE  Vector4& AssignLengthSQ2() { return AssignDot2(*this); }
			CCDK_FORCEINLINE  Vector4& AssignLength4() { return AssignDot4(*this).AssignSqrt(); }
			CCDK_FORCEINLINE  Vector4& AssignLength3() { return AssignDot3(*this).AssignSqrt(); }
			CCDK_FORCEINLINE  Vector4& AssignLength2() { return AssignDot2(*this).AssignSqrt(); }
			CCDK_FORCEINLINE  Vector4& AssignDistanceSQ4(const Vector4& v) { return AssignSub(v).AssignLengthSQ4(); }
			CCDK_FORCEINLINE  Vector4& AssignDistanceSQ3(const Vector4& v) { return AssignSub(v).AssignLengthSQ3(); }
			CCDK_FORCEINLINE  Vector4& AssignDistanceSQ2(const Vector4& v) { return AssignSub(v).AssignLengthSQ2(); }
			CCDK_FORCEINLINE  Vector4& AssignDistance4(const Vector4& v) { return AssignDistanceSQ4(v).AssignSqrt(); }
			CCDK_FORCEINLINE  Vector4& AssignDistance3(const Vector4& v) { return AssignDistanceSQ3(v).AssignSqrt(); }
			CCDK_FORCEINLINE  Vector4& AssignDistance2(const Vector4& v) { return AssignDistanceSQ2(v).AssignSqrt(); }
			CCDK_FORCEINLINE  Vector4& AssignSumup2() { return AssignAdd(yxwz()); }
			CCDK_FORCEINLINE  Vector4& AssignSumup3() { return AssignAdd(yzxw() + zxyw()); }
			CCDK_FORCEINLINE  Vector4& AssignSumup4() { AssignAdd(yxwz()); return AssignAdd(zwxy()); }
			CCDK_FORCEINLINE  Vector4& AssignToDegree();
			CCDK_FORCEINLINE  Vector4& AssignToRadian();
			CCDK_FORCEINLINE  Vector4& AssignNormalize2() { return AssignMul(LengthSQ2().AssignAccurateRcpSqrt()); }
			CCDK_FORCEINLINE  Vector4& AssignNormalize3() { return AssignMul(LengthSQ3().AssignAccurateRcpSqrt()); }
			CCDK_FORCEINLINE  Vector4& AssignNormalize4() { return AssignMul(LengthSQ4().AssignAccurateRcpSqrt()); }
			CCDK_FORCEINLINE  Vector4& AssignSelect(const Vector4& v1, const Vector4& v2) { return AssignBitAnd(v1.Copy().AssignBitXor(v2)).AssignBitXor(v2); }
			CCDK_FORCEINLINE  Vector4& AssignSaturate();
			CCDK_FORCEINLINE  Vector4& AssignClamp(const Vector4& min, const Vector4& max) { return AssignMax(min).AssignMin(max); }
			//        3D ray reflect
			//      incident   reflect
			//          \  |  /|
			//           \ | /
			//     _____ _\|/_________
			//     incident - 2 * dot(incident, normal)*normal 
			CCDK_FORCEINLINE  Vector4& AssignReflect3(const Vector4& normal);
			CCDK_FORCEINLINE  Vector4& AssignReflect2(const Vector4& normal);
			CCDK_FORCEINLINE  Vector4& AssignLerp(const Vector4& to, float t) { Vector4 cp{ vec }; return AssignSub(to).AssignMul(t).AssignAdd(cp); }
			CCDK_FORCEINLINE  Vector4& AssignStepOne();
			CCDK_FORCEINLINE  Vector4& AssignRStepOne();
			CCDK_FORCEINLINE  Vector4& AssignStep(const Vector4& v);
			CCDK_FORCEINLINE  Vector4& AssignRStep(const Vector4& v);
			CCDK_FORCEINLINE  Vector4& AssignSin() {}
			CCDK_FORCEINLINE  Vector4& AssignCos() {}

			CCDK_FORCEINLINE void SinCos(Vector4& inSin, Vector4& inCos) const { IngoreUnused2(inSin,inCos); }
			CCDK_FORCEINLINE void GetPerpendicularAxis(const Vector4& inx, Vector4& iny) const { IngoreUnused2(inx, iny); }

			// judgement
			CCDK_FORCEINLINE bool True4() const;
			CCDK_FORCEINLINE bool AnyTrue4() const;
			CCDK_FORCEINLINE bool AnyTrue3() const;
			CCDK_FORCEINLINE bool AnyTrue2() const;
			CCDK_FORCEINLINE bool False4() const;
			CCDK_FORCEINLINE bool AnyFalse4() const;
			CCDK_FORCEINLINE bool AnyFalse3() const;
			CCDK_FORCEINLINE bool AnyFalse2() const;

			template<uint32 p0 = 0, uint32 p1 = 1, uint32 p2 = 2>
			CCDK_FORCEINLINE bool True3() const
			{
				static_assert(p0 < 4 && p1 < 4 && p2 < 4, "index must be 0,1,2,3, out of range");
				return  iArray[p0] == kfAllOneMask && iArray[p1] == kfAllOneMask && iArray[p2] == kfAllOneMask;
			}

			template<uint32 p0 = 0, uint32 p1 = 1, uint32 p2 = 2>
			CCDK_FORCEINLINE bool False3() const
			{
				static_assert(p0 < 4 && p1 < 4 && p2 < 4, "index must be 0,1,2,3, out of range");
				return iArray[p0] == 0 && iArray[p1] == 0 && iArray[p2] == 0;
			}

			template<uint32 p0 = 0, uint32 p1 = 1>
			CCDK_FORCEINLINE bool True2() const
			{
				static_assert(p0 < 4 && p1 < 4, "index must be 0,1,2,3, out of range");
				return iArray[p0] == kfAllOneMask && iArray[p1] == kfAllOneMask;
			}

			template<uint32 p0 = 0, uint32 p1 = 1> CCDK_FORCEINLINE bool False2() const { return iArray[p0] == 0 && iArray[p1] == 0; }
			template<uint32 p = 0> CCDK_FORCEINLINE bool True1() const { return  iArray[p] == kfAllOneMask; }
			template<uint32 p = 0> CCDK_FORCEINLINE bool False1() const { return iArray[p] == 0; }

			// x^2+y^2 nearly equals 1
			CCDK_FORCEINLINE bool IsUnit2(float tolerence = kfMiddleEpiside) const;
			// x^2+y^2+z^2 nearly equals 1
			CCDK_FORCEINLINE bool IsUnit3(float tolerence = kfMiddleEpiside) const;
			// x^2+y^2+z^2+w^2 nearly equals 1
			CCDK_FORCEINLINE bool IsUnit4(float tolerence = kfMiddleEpiside) const;

			//  min.xy <= this.xy <= max.xy
			CCDK_FORCEINLINE bool IsInBound2(const Vector4& min, const Vector4& max) const { return (*this >= min).True2() && (*this <= max).True2(); }
			//  min.xyz <= this.xyz <= max.xyz
			CCDK_FORCEINLINE bool IsInBound3(const Vector4& min, const Vector4& max) const { return (*this >= min).True3() && (*this <= max).True3(); }
			//  min.xyzw <= this.xyzw <= max.xyzw
			CCDK_FORCEINLINE bool IsInBound4(const Vector4& min, const Vector4& max) const { return (*this >= min).True4() && (*this <= max).True4(); }

			//test one of x,y,z,w is nearly 0
			CCDK_FORCEINLINE bool ContainZero4(float tolerence = kfMiddleEpiside) const { return Abs().AssignLess(tolerence).AnyTrue4(); }
			//test one of x,y,z is nearly 0
			CCDK_FORCEINLINE bool ContainZero3(float tolerence = kfMiddleEpiside) const { return Abs().AssignLess(tolerence).AnyTrue3(); }
			//test one of x,y is nearly 0
			CCDK_FORCEINLINE bool ContainZero2(float tolerence = kfMiddleEpiside) const { return Abs().AssignLess(tolerence).AnyTrue2(); }

			//test one of x,y,z,w is NaN
			CCDK_FORCEINLINE bool IsNaN4() const { return (*this != *this).AnyTrue4(); }
			//test one of x,y,z is NaN
			CCDK_FORCEINLINE bool IsNaN3() const { return (*this != *this).AnyTrue3(); }
			//test one of x,y is NaN
			CCDK_FORCEINLINE bool IsNaN2() const { return (*this != *this).AnyTrue2(); }
			//test one of x,y,z,w is +INF or -INF
			CCDK_FORCEINLINE bool IsInfinite4()const;
			//test one of x,y,z is +INF or -INF
			CCDK_FORCEINLINE bool IsInfinite3()const;
			//test one of x,y is +INF or -INF
			CCDK_FORCEINLINE bool IsInfinite2()const;


			CCDK_FORCEINLINE std::string ToString() const
			{
				//CheckError();
				std::array<char, 64> buffer;
				sprintf(buffer.data(), "[%.7f,%.7f,%.7f,%.7f]", fArray[0], fArray[1], fArray[2], fArray[3]);
				buffer[63] = '\0';
				return std::string{ buffer.data() };
			}

			CCDK_FORCEINLINE std::string ToStringAsInt() const
			{
				std::array<char, 64> buffer;
				sprintf(buffer.data(), "[%x,%x,%x,%x]", iArray[0], iArray[1], iArray[2], iArray[3]);
				return std::string{ buffer.data() };
			}

			///helper function
			CCDK_FORCEINLINE void CheckError()
			{
				assert(!IsNaN4());
				assert(!IsInfinite4());
			}
		};

		namespace kVector4
		{
			constexpr Vector4 One{ 1.0f };
			constexpr Vector4 Zero{ 0.0f };
			constexpr Vector4 NegtiveOne{ -1.0f };
			constexpr Vector4 Half{ 0.5f };
			constexpr Vector4 Two{ 2.0f };
			constexpr Vector4 OneOver2Pi{ kfOneOver2Pi };
			constexpr Vector4 OneOver2PiSquare{ kf4OverPiSQ };
			constexpr Vector4 TwoPi{ kf2Pi };
			constexpr Vector4 Pi{ kfPi };
			constexpr Vector4 HalfPi{ kfHalfPi };
			constexpr Vector4 QuaterPi{ kfQuarterPi };
			constexpr Vector4 _180OverPi{ kf180OverPi };
			constexpr Vector4 PiOver180{ kfPiOver180 };
			constexpr Vector4 Right{ 1.0f, 0.0f, 0.0f, 0.0f };
			constexpr Vector4 Left{ -1.0f, 0.0f, 0.0f, 0.0f };
			constexpr Vector4 Up{ 0.0f, 1.0f, 0.0f, 0.0f };
			constexpr Vector4 Down{ 0.0f, -1.0f, 0.0f, 0.0f };
			constexpr Vector4 Front{ 0.0f, 0.0f, 1.0f, 0.0f };
			constexpr Vector4 Back{ 0.0f, 0.0f, -1.0f, 0.0f };
			constexpr Vector4 WOne{ 0.0f, 0.0f, 0.0f, 1.0f };
			constexpr Vector4 OddNegtive{ 1.0f, -1.0f, 1.0f, -1.0f };
			constexpr Vector4 EvenNegtive{ -1.0f, 1.0f, -1.0f, 1.0f };
			constexpr Vector4 AllOneMask{ kfAllOneMask };
			constexpr Vector4 XYZAllOneMask{ kfAllOneMask,kfAllOneMask,kfAllOneMask,0u };
			constexpr Vector4 XYAllOneMask{ kfAllOneMask,kfAllOneMask,0u,0u };
			constexpr Vector4 XAllOneMask{ kfAllOneMask,0u,0u,0u };
			constexpr Vector4 YAllOneMask{ 0u,kfAllOneMask,0u,0u };
			constexpr Vector4 ZAllOneMask{ 0u,0u,kfAllOneMask,0u };
			constexpr Vector4 WAllOneMask{ 0u,0u,0u,kfAllOneMask };
			constexpr Vector4 AbsMask{ kiAbsMask };
			constexpr Vector4 SignMask{ kiSignMask };
			constexpr Vector4 PositiveInf{ kfPositiveInf };
			constexpr Vector4 NegativeInf{ kfNegativeInf };
		}
		CCDK_FORCEINLINE  Vector4& Vector4::AssignAbs() { return AssignBitAnd(kVector4::AbsMask); }
		CCDK_FORCEINLINE  Vector4& Vector4::AssignSign() { return AssignBitAnd(kVector4::SignMask); }
		CCDK_FORCEINLINE bool Vector4::IsUnit2(float tolerence) const { return LengthSQ2().AssignSub(kVector4::One).AssignAbs()[0] < tolerence; }
		CCDK_FORCEINLINE bool Vector4::IsUnit3(float tolerence) const { return LengthSQ3().AssignSub(kVector4::One).AssignAbs()[0] < tolerence; }
		CCDK_FORCEINLINE bool Vector4::IsUnit4(float tolerence) const { return LengthSQ4().AssignSub(kVector4::One).AssignAbs()[0] < tolerence; }
		CCDK_FORCEINLINE bool Vector4::IsInfinite4() const { return (*this == kVector4::PositiveInf).AnyTrue4() || (*this == kVector4::NegativeInf).AnyTrue4(); }
		CCDK_FORCEINLINE bool Vector4::IsInfinite3() const { return (*this == kVector4::PositiveInf).AnyTrue3() || (*this == kVector4::NegativeInf).AnyTrue3(); }
		CCDK_FORCEINLINE bool Vector4::IsInfinite2() const { return (*this == kVector4::PositiveInf).AnyTrue2() || (*this == kVector4::NegativeInf).AnyTrue2(); }
		CCDK_FORCEINLINE Vector4& Vector4::AssignStep(const Vector4& v) { return AssignGreater(v).AssignSelect(kVector4::One, kVector4::Zero); }
		CCDK_FORCEINLINE Vector4& Vector4::AssignRStep(const Vector4& v) { return AssignLess(v).AssignSelect(kVector4::One, kVector4::Zero); }
		CCDK_FORCEINLINE Vector4& Vector4::AssignStepOne() { return AssignStep(kVector4::One); }
		CCDK_FORCEINLINE Vector4& Vector4::AssignRStepOne() { return AssignRStep(kVector4::One); }

		CCDK_FORCEINLINE Vector4& Vector4::AssignReflect3(const Vector4& normal) { return AssignSub(Vector4{ vec }.AssignDot3(normal).AssignMul(kVector4::Two).AssignMul(normal)); }
		CCDK_FORCEINLINE Vector4& Vector4::AssignReflect2(const Vector4& normal) { return AssignSub(Vector4{ vec }.AssignDot2(normal).AssignMul(kVector4::Two).AssignMul(normal)); }
		CCDK_FORCEINLINE Vector4& Vector4::AssignSaturate() { return AssignMax(kVector4::Zero).AssignMin(kVector4::One); }
		CCDK_FORCEINLINE Vector4& Vector4::AssignToDegree() { return AssignMul(kVector4::_180OverPi); }
		CCDK_FORCEINLINE Vector4& Vector4::AssignToRadian() { return AssignMul(kVector4::PiOver180); }
		CCDK_FORCEINLINE Vector4& Vector4::AssignBitNot() { return AssignBitNotAnd(kVector4::AllOneMask); };
		CCDK_FORCEINLINE Vector4& Vector4::AssignAccurateRcp() { vec = ( kVector4::One / *this ).vec; return *this; }
		CCDK_FORCEINLINE Vector4& Vector4::AssignAccurateRcpSqrt() { vec = (kVector4::One / AssignSqrt() ).vec; return *this; }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////  Base SIMD Operation ////////////////////////////////////////////////////
#if defined(CCDK_SSE) || defined(CCDK_NEON)
		CCDK_FORCEINLINE Vector4 Vector4::operator-() const { return VectorSub(math_impl::kVectorZero, vec); }
		CCDK_FORCEINLINE Vector4 Vector4::operator+(const Vector4& v) const { return VectorAdd(vec, v.vec); }
		CCDK_FORCEINLINE Vector4 Vector4::operator-(const Vector4& v) const { return VectorSub(vec, v.vec); }
		CCDK_FORCEINLINE Vector4 Vector4::operator*(const Vector4& v) const { return VectorMul(vec, v.vec); }		
		CCDK_FORCEINLINE Vector4 Vector4::operator/(const Vector4& v) const { return VectorDiv(vec, v.vec); }
		CCDK_FORCEINLINE Vector4 Vector4::operator&(const Vector4& v) const { return VectorAnd(vec, v.vec); }
		CCDK_FORCEINLINE Vector4 Vector4::operator|(const Vector4& v) const { return VectorOr(vec, v.vec); }
		CCDK_FORCEINLINE Vector4 Vector4::operator^(const Vector4& v) const { return VectorXor(vec, v.vec); }
		CCDK_FORCEINLINE Vector4 Vector4::BitNotAnd(const Vector4& v) const { return VectorNotAnd(vec, v.vec); }
		CCDK_FORCEINLINE Vector4 Vector4::operator>(const Vector4& v) const { return VectorGreater(vec, v.vec); }
		CCDK_FORCEINLINE Vector4 Vector4::operator>=(const Vector4& v) const { return VectorGreaterEqual(vec, v.vec); }
		CCDK_FORCEINLINE Vector4 Vector4::operator<(const Vector4& v) const { return VectorLess(vec, v.vec); }
		CCDK_FORCEINLINE Vector4 Vector4::operator<=(const Vector4& v) const { return VectorLessEqual(vec, v.vec); }
		CCDK_FORCEINLINE Vector4 Vector4::operator==(const Vector4& v) const { return VectorEquals(vec, v.vec); }
		CCDK_FORCEINLINE Vector4 Vector4::operator!=(const Vector4& v) const { return VectorNotEquals(vec, v.vec); }
		CCDK_FORCEINLINE Vector4 Vector4::IntPart() const { return VectorIntPart(vec); }
		CCDK_FORCEINLINE Vector4 Vector4::Round() const { return VectorRound(vec); }
		CCDK_FORCEINLINE Vector4 Vector4::Min(const Vector4& v) const { return VectorMin(vec, v.vec); }
		CCDK_FORCEINLINE Vector4 Vector4::Max(const Vector4& v) const { return VectorMax(vec, v.vec); }
		CCDK_FORCEINLINE Vector4 Vector4::FastRcp() const { return VectorReciprocalApproximate(vec); }
		CCDK_FORCEINLINE float128 Vector4::Sqrt() const { return VectorSqrt(vec); }
		CCDK_FORCEINLINE Vector4 Vector4::FastRcpSqrt() const { return VectorReciprocalSqrtApproximate(vec); }
		CCDK_FORCEINLINE bool Vector4::True4() const { return VectorTrue4(vec); }
		CCDK_FORCEINLINE bool Vector4::AnyTrue4() const { return VectorAnyTrue4(vec); }
		CCDK_FORCEINLINE bool Vector4::AnyTrue3() const { return VectorAnyTrue3(vec); }
		CCDK_FORCEINLINE bool Vector4::AnyTrue2() const { return VectorAnyTrue2(vec); }
		CCDK_FORCEINLINE bool Vector4::False4() const { return VectorFalse4(vec); }
		CCDK_FORCEINLINE bool Vector4::AnyFalse4() const { return VectorAnyFalse4(vec); }
		CCDK_FORCEINLINE bool Vector4::AnyFalse3() const { return VectorAnyFalse3(vec); }
		CCDK_FORCEINLINE bool Vector4::AnyFalse2() const { return VectorAnyFalse2(vec); }
#else //FPU implements
#endif
	
	}
}