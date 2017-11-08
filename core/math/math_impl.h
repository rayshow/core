#pragma once

#include<cmath>
#include<cassert>
#include<core/type.h>
#include<core/math/float32.h>
#include<core/math/math_fwd.h>

#if defined(AURORA3D_SSE)
#	include<emmintrin.h>
#elif defined(AURORA3D_NEON)
#	include <arm_neon.h>
#endif

	
namespace Aurora3D
{
	namespace math
	{
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////  Float / Vector/ Matrix Operation Declare //////////////////////////////////////////////////////////
#if 1
#if defined(AURORA3D_SSE)
		// float128   HIGH 3 2 1 0 LOW
		// float[4]      w z y x  
		typedef __m128  float128;
		typedef __m128i int128;

		union misc128
		{
			float128  f128;
			int128	  i128;
		};

		struct float128x4
		{
			float128 V[4];
			float128& operator[](int i) { return V[i]; }
			const float128& operator[](int i) const { return V[i]; }
		};
#elif defined(AURORA3D_NEON)
		typedef float32x4_t A3D_GCC_ALIGH(16) float128;
		typedef float32x2_t A3D_GCC_ALIGH(16) float64;
		typedef int32x4_t  A3D_GCC_ALIGH(16) uint128;

		struct float128x4 {
			float128 V[4];
			float128& operator[](int i) {
				return V[i];
			}
			const float128& operator[](int i) const {
				return V[i];
			}
		};
#endif 

#if defined(AURORA3D_SSE) || defined(AURORA3D_NEON)

		//xyzw = reinterpret_cast<float>(u)
		float128 VectorLoad(int32 u);

		//xyzw = (F,F,F,F)
		float128 VectorLoad(float F);

		//xyzw = (x,y,z,w)
		float128 VectorLoad(float x, float y, float z = 0.0f, float w = 0.0f);

		//xyzw = reinterpret_cast<float>((x,y,z,w))
		float128 VectorLoad(int32 x, int32 y, int32 z = 0u, int32 w = 0u);

		namespace math_impl
		{
			//common constant
			constexpr float128 kVectorOne = { 1.0f, 1.0f, 1.0f, 1.0f };
			constexpr float128 kVectorZero = { 0.0f, 0.0f, 0.0f, 0.0f };
			constexpr float128 kVectorNegtiveOne = { -1.0f,-1.0f,-1.0f,-1.0f };
			constexpr float128 kVectorHalf = { 0.5f, 0.5f, 0.5f, 0.5f };
			constexpr float128 kVectorTwo = { 2.0f,2.0f,2.0f,2.0f };
			constexpr float128 kVectorXOne = { 1.0f, 0.0f, 0.0f, 0.0f };
			constexpr float128 kVectorYOne = { 0.0f, 1.0f, 0.0f, 0.0f };
			constexpr float128 kVectorZOne = { 0.0f, 0.0f, 1.0f, 0.0f };
			constexpr float128 kVectorWOne = { 0.0f, 0.0f, 0.0f, 1.0f };
			constexpr float128 kVectorOddNegtive = { 1.0f, -1.0f, 1.0f, -1.0f };
			constexpr float128 kVectorEvenNegtive = { -1.0f, 1.0f, -1.0f, 1.0f };
			constexpr float128 kVectorOneOver2PI = { kfOneOver2Pi ,kfOneOver2Pi ,kfOneOver2Pi ,kfOneOver2Pi };
			constexpr float128 kVector2PI = { kf2Pi ,kf2Pi ,kf2Pi ,kf2Pi };
			constexpr float128 kVectorPI = { kfPi ,kfPi ,kfPi ,kfPi };
			constexpr float128 kVectorHalfPI = { kfHalfPi ,kfHalfPi ,kfHalfPi ,kfHalfPi };
			constexpr float128 kVector180OverPI = { kf180OverPi,kf180OverPi ,kf180OverPi,kf180OverPi };
			constexpr float128 kVectorPIOver180 = { kfPiOver180,kfPiOver180 ,kfPiOver180 ,kfPiOver180 };
			static const float128 kVectorSignMask = VectorLoad(kiSignMask);
			static const float128 kVectorInfinte = VectorLoad(kfPositiveInf);
			static const float128 kVectorAllOneMask = VectorLoad(kfAllOneMask);
			static const float128 kVectorXYZMask = VectorLoad(kfAllOneMask, kfAllOneMask, kfAllOneMask, kfAllOneMask);
			static const float128 kVectorAbsMask = VectorLoad(kiAbsMask);
		}
		
		//xyzw = 0.0f
		float128 VectorZero();

		//xyzw = (v[0],v[1],0.0f,0.0f), v no need aligned
		float128 VectorLoad2Z0(const float *v);

		//xyzw = (v[0],v[1],1.0f,0.0f), v no need aligned
		float128 VectorLoad2Z1(const float *v);

		//xyzw = (v[0],v[1],v[2],0.0f), v no need aligned
		float128 VectorLoad3W0(const float *v);

		//xyzw = (v[0],v[1],v[2],1.0f), v no need aligned
		float128 VectorLoad3W1(const float *v);

		//xyzw = (v[0],v[1],v[2],v[3]), v no need aligned
		float128 VectorLoad4(const float *v);

		//xyzw = (v[0],v[1],v[2],v[3]), v need aligned
		float128 VectorLoad4Aligned(const float *v);

		//v[i] = F
		void VectorSet(float128& v, int i, float F);

		//v[i] = U
		void VectorSet(float128& v, int i, int32 U);

		//v.w = 0.0f
		void VectorSetW0(float128& v);

		//v.w = 1.0f
		void VectorSetW1(float128& v);

		// ret = v[i]
		float VectorGetFloat(const float128& v, int i);

		// ret = v[i]
		int32 VectorGetInt32(const float128& v, int i = 0);

		// ret = v[0] 
		float VectorGetFirst(const float128& v);

		//v.xy = (m[0],m[1])£¬m no need aligned
		void  VectorStore2(const float128& v, float* m);

		//v.xy = (m[0],m[1],m[2]),m no  need aligned
		void  VectorStore3(const float128& v, float* m);

		//v.xyzw = (m[0],m[1],m[2],m[3]), m no  need aligned
		void  VectorStore4(const float128& v, float *m);

		//v.xyzw = (m[0],m[1],m[2],m[3]), m need aligned
		void  VectorStore4Aligned(const float128& v, float *m);

		// v1 = v2
		void  VectorStore4Aligned(const float128& v, float *m);

		// v.xyzw = (m[0],m[1],m[2],m[3]), m need aligned, don't change cache
		void VectorStore4AlignedNoCache(float128& v, float *m);

		// v.xyzw = reinterpret_cast<float>(m[0],m[1],m[2],m[3])
		void VectorStore4(const float128& v, int32 *m);

		// ret.xyzw = (v[p],v[p],v[p],v[p]£©
		template<unsigned p> 
		float128 VectorReplicate(const float128& v);

		// ret.xyzw = (v[p0],v[p1],v[p2],v[p3])
		template<unsigned p0, unsigned p1, unsigned p2, unsigned p3>
		float128 VectorShuffle(const float128& v);

		// ret.xyzw = (v1[p0],v1[p1],v2[p2],v2[p3])
		template<unsigned p0, unsigned p1, unsigned p2, unsigned p3>
		float128 VectorShuffle(const float128& v1, const float128& v2);

		// ret.xyzw = (v1[0], v2[0], v1[1], v2[1])
		float128 VectorInterleaveXY(const float128& v1 , const float128& v2);

		// ret.xyzw = (v1[2], v2[2], v1[3], v2[3])
		float128 VectorInterleaveZW(const float128& v1 , const float128& v2);

		// ret.xyzw = ( v.x>0?1.0f:-1.0f, v.y>0?1.0f:-1.0f,v.z>0?1.0f:-1.0f,v.w>0?1.0f:-1.0f)
		float128 VectorSign(const float128& v);

		// ret.xyzw = v1.xyzw + v2.xyzw
		float128 VectorAdd(const float128& v1, const float128& v2);

		// ret.xyzw = v1.xyzw + scale.xxxx
		float128 VectorAdd(const float128& v1, float scale);

		// v1 = v1 + v2
		void VectorAddAssign(float128& v1, const float128& v2);

		// ret = v1 + v2 + v3
		float128 VectorAddTwice(const float128& v1, const float128& v2, const float128& v3);

		// ret = v1 + v2 - v3
		float128 VectorAddSub(const float128& v1, const float128& v2, const float128& v3);

		// ret = (v1+v2) * v3
		float128 VectorAddMutiply(const float128& v1, const float128& v2, const float128& v3);

		// ret = (v1+v2) / v3
		float128 VectorAddDiv(const float128& v1, const float128& v2, const float128& v3);

		// ret.xyzw = v1.xyzw - v2.xyzw
		float128 VectorSub(const float128& v1, const float128& v2);

		// ret.xyzw = v1.xyzw - (scale,scale,scale,scale)
		float128 VectorSub(const float128& v1, float scale);

		// v1 = v1 - v2
		void VectorSubAssign(float128& v1, const float128 v2);

		// ret = v1 - v2 - v3
		float128 VectorSubTwice(const float128& v1, const float128& v2, const float128& v3);

		// ret = v1 - v2 + v3 
		float128 VectorSubAdd(const float128& v1, const float128& v2, const float128& v3);

		// ret = (v1 - v2)*v3
		float128 VectorSubMutiply(const float128& v1, const float128& v2, const float128& v3);

		// ret (v1-v2)/v3
		float128 VectorSubDiv(const float128& Sub1, const float128& Sub2, const float128& div);

		// ret.xyzw = v1.xyzw * v2.xyzw
		float128 VectorMul(const float128& v1, const float128& v2);

		// ret = v.xyzw * scale
		float128 VectorMul(const float128& v, float scale);

		// v1 = v1*v2
		void VectorMulAssign(float128& v1, const float128& v2);

		// v.xyzw = v.xyzw *scale
		void VectorMulAssign(float128& v, float scale);

		// ret = v1 * v2 * v3
		float128 VectorMulTwice(const float128& v1, const float128& v2, const float128& v3);

		// ret = v1*v2 +v3
		float128 VectorMulAdd(const float128& v1, const float128& v2, const float128& v3);

		// ret = v1*v2 - v3
		float128 VectorPreMulSub(const float128& v1, const float128& v2, const float128& v3);

		// ret = v1 - v1*v2
		float128 VectorPostMulSub(const float128& v1, const float128& v2, const float128& v3);

		// ret = v1*v2/v3 
		float128 VectorPreMulDiv(const float128& v1, const float128& v2, const float128& v3);

		// ret = v1/(v2*v3)
		float128 VectorPostMulDiv(const float128& v1, const float128& v2, const float128& v3);

		// ret.xyzw = v1.xyzw / v2.xyzw
		float128 VectorDiv(const float128& v1, const float128& v2);

		// ret.xyzw = v1.xyzw / s
		float128 VectorDiv(const float128& v1, float s);

		// v1.xyzw = v1.xyzw/s
		float128 VectorDivAssign(const float128& v1, float s);

		// v1 = v1 / v2;
		float128 VectorDivAssign(const float128& v1, const float128& v2);

		// ret = v1 / (v2+v3)
		float128 VectorDivAdd(const float128& div1, const float128& div2, const float128& add);

		// ret = v1 / v2 - v3
		float128 VectorPreDivSub(const float128& v1, const float128& v2, const float128& v3);

		// ret = v1 - v2 / v3
		float128 VectorPostDivSub(const float128& v1, const float128& v2, const float128& v3);

		//return mod1 - (int)(mod1/mod2)*mod2
		float128 VectorMod(const float128& mod1, const float128& mod2);

		// 11/10/01 -> 1 00->0
		// ret.xyzw = (v1.x|v2.x, v1.y|v2.y, v1.z|v2.z, v1.w|v2.w)
		float128 VectorOr(const float128& v1, const float128& v2);

		// 11 -> 1 01/10/00 -> 0
		// ret.xyzw = (v1.x&v2.x, v1.y&v2.y, v1.z&v2.z, v1.w&v2.w)
		float128 VectorAnd(const float128& v1, const float128& v2);

		// 11 ->0  01/10/00 -> 1
		// ret.xyzw = ( !v1.x&v2.x, !v1.y&v2.y, !v1.z&v2.z, !v1.w&v2.w)
		float128 VectorNotAnd(const float128& v1, const float128& v2);

		// 11/00 -> 0  01/10 -> 1
		//ret.xyzw = ( v1.x^v2.x, v1.y^v2.y, v1.z^v2.z, v1.w^v2.w)
		float128 VectorXor(const float128& v1, const float128& v2);

		// true if v.xy == (0xffffffff,0xffffffff),else false
		bool VectorTrue2(const float128& v);

		// true if v.xyz == (0xffffffff,0xffffffff,0xffffffff),else false
		bool VectorTrue3(const float128& v);

		// true if v.xyzw == (0xffffffff,0xffffffff,0xffffffff,0xffffffff),else false
		bool VectorTrue4(const float128& v);

		// true if v.x or v.y == 0xffffffff
		bool VectorAnyTrue2(const float128& v);

		// true if v.x or v.y or v.z == 0xffffffff
		bool VectorAnyTrue3(const float128& v);

		// true if v.x or v.y or v.z or v.w == 0xffffffff
		bool VectorAnyTrue4(const float128& v);

		// true if v.xy == (0, 0),else false
		bool VectorFalse2(const float128& v);

		// true if v.xyz == (0, 0, 0),else false
		bool VectorFalse3(const float128& v);

		// true if v.xyz == (0, 0, 0, 0),else false
		bool VectorFalse4(const float128& v);

		// true if v.x or v.y == 0
		bool VectorAnyFalse2(const float128& v);

		// true if v.x or v.y or v.z == 0
		bool VectorAnyFalse3(const float128& v);

		// true if v.x or v.y or v.z or v.w == 0
		bool VectorAnyFalse4(const float128& v);

		// ret.xyzw = (v1.x == v2.x? 0xffffffff:0, v1.y == v2.y? 0xffffffff:0,
		// v1.z == v2.z? 0xffffffff:0, v1.w == v2.w? 0xffffffff:0)
		float128 VectorEquals(const float128& v1, const float128& v2);

		// Abs( v1.xyzw - v2.xyzw ) <= (Epside,Epside,Epside,Epside)
		float128 VectorNearlyEquals(const float128& v1, const float128& v2, float epside = kfMiddleEpiside);

		// ret.xyzw = (v1.x != v2.x? 0xffffffff:0, v1.y != v2.y? 0xffffffff:0,
		// v1.z != v2.z? 0xffffffff:0, v1.w != v2.w? 0xffffffff:0)
		float128 VectorNotEquals(const float128& v1, const float128& v2);

		// ret.xyzw = (v1.x > v2.x? 0xffffffff:0, v1.y > v2.y? 0xffffffff:0,
		// v1.z > v2.z? 0xffffffff:0, v1.w > v2.w? 0xffffffff:0)
		float128 VectorGreater(const float128& v1, const float128& v2);

		// ret.xyzw = (v1.x >= v2.x? 0xffffffff:0, v1.y >= v2.y? 0xffffffff:0,
		// v1.z >= v2.z? 0xffffffff:0, v1.w >= v2.w? 0xffffffff:0)
		float128 VectorGreaterEqual(const float128& v1, const float128& v2);

		// ret.xyzw = (v1.x < v2.x? 0xffffffff:0, v1.y < v2.y? 0xffffffff:0,
		// v1.z < v2.z? 0xffffffff:0, v1.w < v2.w? 0xffffffff:0)
		float128 VectorLess(const float128& v1, const float128& v2);

		// ret.xyzw = (v1.x <= v2.x? 0xffffffff:0, v1.y <= v2.y? 0xffffffff:0,
		// v1.z <= v2.z? 0xffffffff:0, v1.w <= v2.w? 0xffffffff:0)
		float128 VectorLessEqual(const float128& v1, const float128& v2);

		// true if abs(v.xy ) < bound.xy
		bool VectorInBound2(const float128& v, const float128& bound);

		// true if abs(v.xyz ) < bound.xyz
		bool VectorInBound3(const float128&v, const float128& bound);

		// true if abs(v.xyzw ) < bound.xyzw
		bool VectorInBound4(const float128&v, const float128& bound);

		//ret.xyzw = ( max(v1.x,v2.x), max(v1.y,v2.y), max(v1.z,v2.z), max(v1.w,v2.w) )
		float128 VectorMax(const float128& v1, const float128& v2);

		//ret.xyzw = ( min(v1.x,v2.x), min(v1.y,v2.y), min(v1.z,v2.z), min(v1.w,v2.w) )
		float128 VectorMin(const float128& v1, const float128& v2);

		// true if  is v.x or v.y is Nan
		bool VectorIsNaN2(const float128& v);

		// true if is v.x or v.y or v.z is Nan
		bool VectorIsNaN3(const float128& v);

		// true if  v.x or v.y or v.z or v.w is Nan
		bool VectorIsNaN4(const float128& v);

		// true if v.x or v.y is INF
		bool VectorIsInfinite2(const float128& v);

		// true if v.x or v.y or v.z is INF
		bool VectorIsInfinite3(const float128& v);

		// true if v.x or v.y or v.z or v.w is INF
		bool VectorIsInfinite4(const float128& v);

		//  ret.xy = v1.x*v2.x+v1.y*v2.y
		float128 VectorDot2(const float128& v1, const float128& v2);

		//  ret.xyz = v1.x*v2.x+v1.y*v2.y+v1.z*v2.z
		float128 VectorDot3(const float128& v1, const float128& v2);

		//float dot = dot(v1.xyzw,v2.xyzw), ret.xyzw = (dot,dot,dot,dot)
		float128 VectorDot4(const float128& v1, const float128& v2);

		// 
		float128 VectorCrossProduct3(const float128& v1, const float128& v2);

		// ret.xy = sqrt( (v1.x-v2.x)^2 + (v1.y-v2.y)^2 )
		float128 VectorDistance2(const float128& v1, const float128& v2);

		// ret.xyz = sqrt( (v1.x-v2.x)^2 + (v1.y-v2.y)^2 + (v1.z-v2.z)^2 )
		float128 VectorDistance3(const float128& v1, const float128& v2);

		// ret.xy = sqrt( v.x^2 + v.y^2 )
		float128 VectorLength2(const float128& v);

		// ret.xyz = sqrt( v.x^2 + v.y^2 + v.z^2 )
		float128 VectorLength3(const float128& v);

		// ret.xy = v.xy * 1 / sqrt(v.x^2 + v.y^2)
		float128 VectorNormalize2(const float128& v);

		// ret.xy = v.xyz * 1 / sqrt(v.x^2 + v.y^2 + v.z^2)
		float128 VectorNormalize3(const float128& v);

		// ret.xy = v.xyzw * 1 / sqrt(v.x^2 + v.y^2 + v.z^2 + v.w^2)
		float128 VectorNormalize4(const float128& v);

		
		// 3D ray reflect
		//      incident   reflect
		//          \  |  /|
		//           \ | /
		//     _____ _\|/_________
		//     incident - 2 * dot(incident, normal)*normal 
		float128 VectorReflect2(const float128 incident, const float128& normal);
		float128 VectorReflect3(const float128 incident, const float128& normal);

		// ret.xy = v.x+v.y
		float128 VectorSumUp2(const float128& v);

		// ret.xyz = v.x + v.y + v.z
		float128 VectorSumUp3(const float128& v);

		//float sumup = v.x+v.y+v.z+v.w, ret.xyzw = (sumup,sumup,sumup,sumup)
		float128 VectorSumUp4(const float128& v);

		//for bit from 0 to 128 ,if mask[bit] == 1? v1[bit] : v2[bit]
		float128 VectorSelect(const float128& v1, const float128& v2, const float128& mask);
		
		// ret.xyzw = (int) v.xyzw
		float128 VectorIntPart(const float128& v);

		// ret.xyzw = v.xyzw - (int)v.xyzw
		float128 VectorFracPart(const float128& v);

		// ret.xyzw = (round(v.x),round(v.y), round(v.z), round(v.w)
		float128 VectorRound(const float128& v);

		// ret = ceil(v.xyzw)
		float128 VectorCeil(const float128& v);

		// ret = floor(v.xyzw)
		float128 VectorFloor(const float128& v);

		// ret.xyzw = -v.xyzw
		float128 VectorNegate(const float128& v);

		// ret.xyzw = abs(v.xyzw)
		float128 VectorAbs(const float128& v);

		// ret.xyzw = Approx( 1/v.xyzw ),  max error approximation less than 0.00036621
		float128 VectorReciprocalApproximate(const float128& v1);

		// ret.xyzw = 1/v.xyzw
		float128 VectorReciprocal(const float128& v);

		// ret.xyzw = sqrt( v.xyzw )
		float128 VectorSqrt(const float128& v);

		// ret.xyzw = Approx( 1/ sqrt(v.xyzw) ),  max error approximation less than 0.00036621
		float128 VectorReciprocalSqrtApproximate(const float128& v);

		// ret.xyzw = 1/sqrt( v.xyzw )
		float128 VectorReciprocalSqrt(const float128& v);

		// ret.xyzw = start.xyzw  + (end.xyzw -start.xyzw)*t
		float128 VectorLerp(const float128& start, const float128& end, float t);

		// ret.xyzw = (v.x>0?1.0f:0.0f,v.y>0?1.0f:0.0f,v.z>0?1.0f:0.0f,v.w>0?1.0f:0.0f)
		float128 VectorStep(const float128& v);

		// ret.xyzw = (v.x>0?0.0f:1.0f,v.y>0?0.0f:1.0f,v.z>0?0.0f:1.0f,v.w>0?0.0f:1.0f)
		float128 VectorReverseStep(const float128& v);
		
		// for i from 0 to 3,  
		// if(v[i]< = min[i]) ret[i] = min[i]  
		// if(v[i] >= max[i]) ret[i] = max[i]
		// else remain unchange
		float128 VectorClamp(const float128& v, const float128& min, const float128& max);


		// for i from 0 to 3,  
		// if(v[i]< = 0.0f) ret[i] = 0.0f
		// if(v[i] >= 1.0f) ret[i] = 1.0f;
		// else remain unchange
		float128 VectorSaturate(const float128& v);

		// tranform v to degree angle
		float128 VectorRadianToDegree(const float128& v);

		// tranform v to radian angle
		float128 VectorDegreeToRadian(const float128& v);

		// ret = sin(v.xyzw ) , v.xyzw is radian angle, Max error of 0.001091
		float128 VectorSinRadianFast(const float128& v);

		// ret = sin(v.xyzw ) , v.xyzw is degree angle, Max error of 0.001091
		float128 VectorSinDegreeFast(const float128& v);

		// ret = cos(v.xyzw ) , v.xyzw is radian angle, Max error of 0.001091
		float128 VectorCosRadianFast(const float128& v);

		// ret = cos(v.xyzw ) , v.xyzw is degree angle, Max error of 0.001091
		float128 VectorCosDegreeFast(const float128& v);

		// computer sin and cos at some time, fast then sperate compute, radian angles
		void VectorSinAndCosFast(float128& sin, float128& cos, const float128& radians);

		// true if M is Identity matrix
		bool MatrixIsIdentity(const float128x4& M);

		// true if one element of M is NaN
		bool MatrixIsNaN(const float128x4& M);

		// true if one element of M is INF
		bool MatrixIsInInfinite(const float128x4& M);

		// C = A * B, No copy
		void MatrixMulNoCopy(const float128x4& A, const float128x4& B, float128x4& C);

		// ret = A * B, copy matrix
		float128x4 MatrixMul(const float128x4& A, const float128x4& B);

		// ret.xyzw = (det,det,det,det), det = Determinant(M)
		float128 MatrixDeterminant(const float128x4& M);

		// transpose A to B, no copy
		void MatrixTransposeNoCopy(const float128x4& A, float128x4& B);

		// ret = transpose(A)
		float128x4 MatrixTranspose(const float128x4& A);

		// inverse Matrix, no copy
		float128 MatrixInverse(const float128x4& original, float128x4& inverse);

		// ret = M * v
		float128 MatrixTransformVector(const float128x4& M, const float128& v);

#endif  //#if defined(AURORA3D_SSE) || defined(AURORA3D_NEON)
#endif  //Float / Vector/ Matrix Operation Declare

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////// Vector Matrix Operation Implementation //////////////////////////////////////////////////////
#if defined(AURORA3D_SSE) || defined(AURORA3D_NEON)

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////// Vector Operation Base Implements ////////////////////////////////////////////////////////
	#if defined(AURORA3D_SSE)

#define SHUFFLE_MASK(p0, p1, p2, p3) ( (p0) | ((p1) << 2) | ((p2) <<4) | ((p3) <<6) )

		A3D_FORCEINLINE float128 VectorZero()
		{
			return _mm_setzero_ps();
		}

		//load replicate int32
		A3D_FORCEINLINE float128 VectorLoad(int32 u)
		{
			return _mm_castsi128_ps(_mm_setr_epi32(u, u, u, u));
		}

		//load replicate float
		A3D_FORCEINLINE float128 VectorLoad(float F)
		{
			return _mm_load_ps1(&F);
		}

		//load 2,3,4 float
		A3D_FORCEINLINE float128 VectorLoad(float x, float y, float z, float w)
		{
			return _mm_setr_ps(x, y, z, w);
		}

		//load 2,3,4 int32
		A3D_FORCEINLINE float128 VectorLoad(int32 x, int32 y, int32 z, int32 w)
		{
			return _mm_castsi128_ps(_mm_setr_epi32(x, y, z, w));
		}


		//load two components with z = 0.0f, w= 0.0f
		A3D_FORCEINLINE float128 VectorLoad2Z0(const float *v)
		{
			return _mm_setr_ps(v[0], v[1], 0.0f, 0.0f);
		}

		//load two components with z = 1.0f, w = 0.0f
		A3D_FORCEINLINE float128 VectorLoad2Z1(const float *v)
		{
			return _mm_setr_ps(v[0], v[1], 1.0f, 0.0f);
		}

		//load three components with w = 0.0f
		A3D_FORCEINLINE float128 VectorLoad3W0(const float *v)
		{
			return _mm_setr_ps(v[0], v[1], v[2], 0.0f);
		}

		//load three component with w = 1.0f
		A3D_FORCEINLINE float128 VectorLoad3W1(const float *v)
		{
			return _mm_setr_ps(v[0], v[1], v[2], 1.0f);
		}

		A3D_FORCEINLINE float128 VectorLoad4(const float *v)
		{
			return _mm_setr_ps(v[0], v[1], v[2], v[3]);
		}

		A3D_FORCEINLINE float128 VectorLoad4Aligned(const float *v)
		{
			return _mm_load_ps(v);
		}

		A3D_FORCEINLINE void VectorSet(float128& v, int i, float F)
		{
			assert(i >= 0 && i <= 3);
			v.m128_f32[i] = F;
		}

		A3D_FORCEINLINE void VectorSet(float128& v, int i, int32 F)
		{
			assert(i >= 0 && i <= 3);
			v.m128_i32[i] = F;
		}

		A3D_FORCEINLINE float VectorGetFloat(const float128& v, int i)
		{
			assert(i >= 0 && i <= 3);
			return v.m128_f32[i];
		}

		A3D_FORCEINLINE int32 VectorGetint32(const float128& v, int i)
		{
			assert(i >= 0 && i <= 3);
			return v.m128_i32[i];
		}

		A3D_FORCEINLINE float VectorGetFirst(const float128& v)
		{
			return v.m128_f32[0];
		}

		A3D_FORCEINLINE void VectorStore2(const float128& v, float* m)
		{
			m[0] = v.m128_f32[0];
			m[1] = v.m128_f32[1];
		}

		A3D_FORCEINLINE void VectorStore3(const float128& v, float* m)
		{
			m[0] = v.m128_f32[0];
			m[1] = v.m128_f32[1];
			m[2] = v.m128_f32[2];
		}

		A3D_FORCEINLINE void VectorStore4(const float128& v, float *m)
		{
			_mm_storeu_ps(m, v);
		}

		A3D_FORCEINLINE void VectorStore4(const float128& v, int32 *m)
		{
			m[0] = v.m128_i32[0];
			m[1] = v.m128_i32[1];
			m[2] = v.m128_i32[2];
			m[3] = v.m128_i32[3];
		}

		A3D_FORCEINLINE void VectorStore4Aligned(const float128& v, float *m)
		{
			_mm_store_ps(m, v);
		}

		A3D_FORCEINLINE void VectorStore4AlignedNoCache(
			float128& v, float *m)
		{
			_mm_stream_ps(m, v);
		}


		//latency 1
		template<unsigned p>
		A3D_FORCEINLINE float128 VectorReplicate(const float128& v)
		{
			return _mm_shuffle_ps(v, v, SHUFFLE_MASK(p, p, p, p));
		}

		//latency 1 return ( v[p0], v[p1], v[p2], v[p3] )
		template<unsigned p0, unsigned p1, unsigned p2, unsigned p3>
		A3D_FORCEINLINE float128 VectorShuffle(const float128& v)
		{
			return _mm_shuffle_ps(v, v, SHUFFLE_MASK(p0, p1, p2, p3));
		}

		//latency 1, return ( v1[p0], v1[p1], v2[p2], v2[p3] )
		template<unsigned p0, unsigned p1, unsigned p2, unsigned p3>
		A3D_FORCEINLINE float128 VectorShuffle(const float128& v1, const float128& v2)
		{
			return _mm_shuffle_ps(v1, v2, SHUFFLE_MASK(p0, p1, p2, p3));
		}

		//latency 3
		//return add1 + add2 
		A3D_FORCEINLINE float128 VectorAdd(const float128& v1, const float128& v2)
		{
			return _mm_add_ps(v1, v2);
		}

		//latency 3
		//return add1 + add2 
		A3D_FORCEINLINE float128 VectorAdd(const float128& v1, float scale)
		{
			return _mm_add_ps(v1, _mm_load_ps1(&scale));
		}


		//latency 3
		//return Sub1 - Sub2
		A3D_FORCEINLINE float128 VectorSub(const float128& v1, const float128& v2)
		{
			return _mm_sub_ps(v1, v2);
		}

		//
		A3D_FORCEINLINE float128 VectorSub(const float128& v1, float scale)
		{
			return _mm_sub_ps(v1,_mm_load_ps1(&scale));
		}


		//Latency 4-5
		//return mul1 * mul2
		A3D_FORCEINLINE float128 VectorMul(const float128& v1, const float128& v2)
		{
			return _mm_mul_ps(v1, v2);
		}

		//Latency 11~14
		// return div1 / div2
		// 2~3 time mul
		A3D_FORCEINLINE float128 VectorDiv(const float128& div1, const float128& div2)
		{
			return _mm_div_ps(div1, div2);
		}

		// if v1[i] == v2[i] return 0xffffffff
		//          !=       return 0x00000000
		// latency 3
		A3D_FORCEINLINE float128 VectorEquals(const float128& v1, const float128& v2)
		{
			return _mm_castsi128_ps( _mm_cmpeq_epi32(_mm_castps_si128(v1), _mm_castps_si128(v2)));
		}

		// if v1[i] != v2[i] return 0xffffffff 
		//          ==       return 0x00000000
		// latency 3
		A3D_FORCEINLINE float128 VectorNotEquals(const float128& v1, const float128& v2)
		{
			return _mm_cmpneq_ps(v1, v2);
		}

		// if v1[i] >  v2[i] return 0xffffffff 
		//          <=       return 0x00000000
		// latency 3
		A3D_FORCEINLINE float128 VectorGreater(const float128& v1, const float128& v2)
		{
			return _mm_cmpgt_ps(v1, v2);
		}

		// if v1[i] <  v2[i] return 0xffffffff 
		//          >=       return 0x00000000
		// latency 3
		A3D_FORCEINLINE float128 VectorLess(const float128& v1, const float128& v2)
		{
			return _mm_cmplt_ps(v1, v2);
		}

		// if v1[i] >= v2[i] return 0xffffffff 
		//          <        return 0x00000000
		// latency 3
		A3D_FORCEINLINE float128 VectorGreaterEqual(const float128& v1, const float128& v2)
		{
			return _mm_cmpge_ps(v1, v2);
		}

		// if v1[i] <= v2[i] return 0xffffffff 
		//          >        return 0x00000000
		// latency 3
		A3D_FORCEINLINE float128 VectorLessEqual(const float128& v1, const float128& v2)
		{
			return _mm_cmple_ps(v1, v2);
		}

		//return v1 | v2
		//Latency 1
		A3D_FORCEINLINE float128 VectorOr(const float128& v1, const float128& v2)
		{
			return _mm_or_ps(v1, v2);
		}

		// 1 -> 0  0 -> 1

		//Latency 1
		//return v1 & v2
		A3D_FORCEINLINE float128 VectorAnd(const float128& v1, const float128& v2)
		{
			
			return _mm_and_ps(v1, v2);
		}

		//Latency 1
		//return (~v1 & v2)
		A3D_FORCEINLINE float128 VectorNot(const float128& v)
		{
			return _mm_andnot_ps(v, math_impl::kVectorAllOneMask);
		}

		//Latency 1
		//return (~v1 & v2)
		A3D_FORCEINLINE float128 VectorNotAnd(const float128& v1, const float128& v2)
		{
			return _mm_andnot_ps(v1, v2);
		}

		//Latency 1
		//return v1 ^ v2
		A3D_FORCEINLINE float128 VectorXor(const float128& v1, const float128& v2)
		{
			return _mm_xor_ps(v1, v2);
		}

		//Latency 3
		//return max( v1, v2)
		A3D_FORCEINLINE float128 VectorMax(const float128& v1, const float128& v2)
		{
			return _mm_max_ps(v1, v2);
		}

		//Latency 3
		//return min(v1, v2)
		A3D_FORCEINLINE float128 VectorMin(const float128& v1, const float128& v2)
		{
			return _mm_min_ps(v1, v2);
		}

		// v[0,1] = 0xffffffff
		A3D_FORCEINLINE bool VectorTrue2(const float128& v)
		{
			return 0x03 == (_mm_movemask_ps(v) & 0x03);
		}

		//v[0,1,2] = 0xffffffff
		A3D_FORCEINLINE bool VectorTrue3(const float128& v)
		{
			return 0x07 == (_mm_movemask_ps(v) & 0x07);
		}

		// v[0,1,2,3] == 0xffffffff
		A3D_FORCEINLINE bool VectorTrue4(const float128& v)
		{
			return 0x0f == _mm_movemask_ps(v);
		}

		// v[0,1] == 0xffffffff
		A3D_FORCEINLINE bool VectorFalse2(const float128& v)
		{
			return 0 == (_mm_movemask_ps(v) & 0x03);
		}

		// v[0,1,2] == 0xffffffff
		A3D_FORCEINLINE bool VectorFalse3(const float128& v)
		{
			return 0 == (_mm_movemask_ps(v) & 0x07);
		}

		// v[0,1,2,3] = 0
		A3D_FORCEINLINE bool VectorFalse4(const float128& v)
		{
			return 0 == _mm_movemask_ps(v);
		}

		// least one of v[i] = 0xffffffff
		A3D_FORCEINLINE bool VectorAnyTrue2(const float128& v)
		{
			return 0 != (_mm_movemask_ps(v) & 0x03);
		}

		// least one of v[i] = 0xffffffff
		A3D_FORCEINLINE bool VectorAnyTrue3(const float128& v)
		{
			return 0 != (_mm_movemask_ps(v) & 0x07);
		}

		// least one of v[i] = 0xffffffff
		A3D_FORCEINLINE bool VectorAnyTrue4(const float128& v)
		{
			return 0 != _mm_movemask_ps(v);
		}

		// least one of v[i] = 0
		A3D_FORCEINLINE bool VectorAnyFalse2(const float128& v)
		{
			return 0x03 != (_mm_movemask_ps(v) & 0x03);
		}

		// least one of v[i] = 0
		A3D_FORCEINLINE bool VectorAnyFalse3(const float128& v)
		{
			return 0x07 != (_mm_movemask_ps(v) & 0x07);
		}

		// least one of v[i] = 0
		A3D_FORCEINLINE bool VectorAnyFalse4(const float128& v)
		{
			return 0x0f != _mm_movemask_ps(v);
		}

		// F = v1.x*v2.x + v1.y*v2.y + v1.z*v2.z + v1.w*v2.w
		// return (F,F,F,F )
		A3D_FORCEINLINE float128 VectorDot4(const float128& v1, const float128& v2)
		{
			float128 multi, rotate;
			multi = _mm_mul_ps(v1, v2);  // x,y,z,w
			rotate = VectorShuffle<2, 3, 0, 1>(multi);  //z,w,x,y
			multi = _mm_add_ps(multi, rotate); //x+z, y+w, x+z, y+w
			rotate = VectorShuffle<1, 0, 3, 2>(multi); //y+w, x+z, y+w, x+z
			return _mm_add_ps(multi, rotate); //x+y+z+w, x+y+z+w, prefix,...
		}

		// F = x+ y + z + w
		// return (F,F,F,F)
		A3D_FORCEINLINE float128 VectorSumUp4(const float128& v)
		{
			float128 rotate = VectorShuffle<2, 3, 0, 1>(v);  //z,w,x,y
			rotate = _mm_add_ps(rotate, v);                  //x+z, y+w, x+z, y+w
			return _mm_add_ps(VectorShuffle<1, 0, 3, 2>(rotate), rotate);
		}

		//for i:0~128 ,if mask[i] == 1? v1[i] : v2[i]
		A3D_FORCEINLINE float128 VectorSelect(const float128& v1,
			const float128& v2, const float128& mask)
		{
			// v1^v2^v2 = v1, so only select mask bit to do 2 times xor
			//     0^v2 = v2, let not select bit be 0
			return _mm_xor_ps(v2, _mm_and_ps(mask, _mm_xor_ps(v1, v2)));
		}

		//return int( v[i] )
		A3D_FORCEINLINE float128 VectorIntPart(const float128& v)
		{
			return _mm_cvtepi32_ps(_mm_cvttps_epi32(v));
		}

		//Latency 6
		//return round( v[i] )
		A3D_FORCEINLINE float128 VectorRound(const float128& v)
		{
			return _mm_cvtepi32_ps(_mm_cvtps_epi32(v));
		}

		namespace SSEConstant
		{
			static const float128 AbsMask = VectorLoad(0x7fffffff);
			static const float128 One = VectorLoad(1.0f);
			static const float128 Zero = VectorLoad(0.0f);
		}

		// return -v
		A3D_FORCEINLINE float128 VectorNegate(const float128& v)
		{
			return VectorSub(SSEConstant::Zero, v);
		}

		// Latency 1
		// return abs( v[i] )
		A3D_FORCEINLINE float128 VectorAbs(const float128& v)
		{
			return _mm_and_ps(v, SSEConstant::AbsMask);
		}

		//Latency 3~5
		// return Approx( 1 / v[i]), max error approximation less than 0.00036621
		A3D_FORCEINLINE float128 VectorReciprocalApproximate(const float128& v1)
		{
			return _mm_rcp_ps(v1);
		}

		//Latency 13 ~ 14
		// return 1 / v[i]
		A3D_FORCEINLINE float128 VectorReciprocal(const float128& v)
		{
			return _mm_div_ps(SSEConstant::One, v);
		}

		//Latency 12~25
		//return v[i] ^ 1/2
		A3D_FORCEINLINE float128 VectorSqrt(const float128& v)
		{
			return _mm_sqrt_ps(v);
		}

		//Latency 12~25
		//return v[i] ^ 1/2
		A3D_FORCEINLINE float128 VectorSqrtApprixmate(const float128& v)
		{
			return _mm_sqrt_ps(v);
		}

		//Latency 3~5
		// return Approx(1/sqrt(v[i])), max error approximation less than 0.00036621
		A3D_FORCEINLINE float128 VectorReciprocalSqrtApproximate(const float128& v)
		{
			return _mm_rsqrt_ps(v);
		}

		//26~39  latency
		// return 1 / sqrt(v[i])
		A3D_FORCEINLINE float128 VectorReciprocalSqrt(const float128& v)
		{
			return _mm_div_ps(SSEConstant::One, _mm_sqrt_ps(v));
		}

		//1 Latency
		//return (v1[0], v2[0], v1[1], v2[1])
		A3D_FORCEINLINE float128 VectorInterleaveXY(const float128& v1
			, const float128& v2)
		{
			return _mm_unpacklo_ps(v1, v2);
		}

		//1 Latency
		//return (v1[2], v2[2], v1[3], v2[3])
		A3D_FORCEINLINE float128 VectorInterleaveZW(const float128& v1
			, const float128& v2)
		{
			return _mm_unpackhi_ps(v1, v2);
		}

		//return start + t*(end -start)
		A3D_FORCEINLINE float128 VectorLerp(const float128& start, const float128& end, float t)
		{
			return _mm_add_ps(start, _mm_mul_ps(_mm_sub_ps(end, start), _mm_set_ps1(t)));
		}

	#elif defined(AURORA3D_NEON)
		A3D_FORCEINLINE float128 VectorZero()
		{
			return vdupq_n_f32(0);
		}

		A3D_FORCEINLINE float128 VectorLoad(float x, float y, float z, float w)
		{
			float128 f;
			f.n128_f32[0] = x;
			f.n128_f32[1] = y;
			f.n128_f32[2] = z;
			f.n128_f32[3] = w;
			return f;
		}

		A3D_FORCEINLINE float128 VectorLoad(int32 x, int32 y, int32 z, int32 w)
		{
			float128 f;
			f.n128_i32[0] = x;
			f.n128_i32[1] = y;
			f.n128_i32[2] = z;
			f.n128_i32[3] = w;
			return f;
		}

		A3D_FORCEINLINE float128 VectorLoad(int32 U)
		{
			return vdupq_n_u32(U);
		}

		A3D_FORCEINLINE float128 VectorLoad(float F)
		{
			return vdupq_n_f32(F);
		}

		A3D_FORCEINLINE float128 VectorLoad3W0(const float *v)
		{
			return VectorLoad(v[0], v[1], v[2], 0.0f);
		}

		A3D_FORCEINLINE float128 VectorLoad3W1(const float *v)
		{
			return VectorLoad(v[0], v[1], v[2], 1.0f);
		}

		A3D_FORCEINLINE float128 VectorLoad4(const float *v)
		{
			return vld1q_f32(v);
		}

		A3D_FORCEINLINE float128 VectorLoad4Aligned(const float *v)
		{
			return vld1q_f32(v);
		}

		A3D_FORCEINLINE void VectorSet(float128& v, int i, float F)
		{
			v = vsetq_lane_f32(F, v, i);
		}

		A3D_FORCEINLINE void VectorSet(float128& v, int i, int32 U)
		{
			v = vsetq_lane_u32(U, v, i);
		}


		A3D_FORCEINLINE float VectorGetFloat(const float128& v, int i)
		{
			return v.n128_f32[i];
		}

		A3D_FORCEINLINE float VectorGetFirst(const float128& v)
		{
			return v.n128_f32[0];
		}

		A3D_FORCEINLINE float VectorGetint32(const float128& v, int i)
		{
			return v.n128_u32[i];
		}



		A3D_FORCEINLINE void VectorStore3(const float128& v, float* m)
		{
			m[0] = v.n128_f32[0];
			m[1] = v.n128_f32[1];
			m[2] = v.n128_f32[2];
		}

		A3D_FORCEINLINE void VectorStore4(const float128& v, float *m)
		{
			vst1q_f32(m, v);
		}

		A3D_FORCEINLINE void VectorStore4(const float128& v, int32 *m)
		{
			vst1q_u32(m, v);
		}

		A3D_FORCEINLINE void VectorStore4Aligned(const float128& v, float *m)
		{
			vst1q_f32(m, v);
		}

		A3D_FORCEINLINE void VectorStoreAlignedNoCache4(const float128& v, float *m)
		{
			vst1q_u32(m, v);
		}

		template<unsigned p>
		A3D_FORCEINLINE float128 VectorReplicate(const float128& v)
		{
			return vdupq_lane_f32(v, p);
		}

		template<unsigned p0, unsigned p1, unsigned p2, unsigned p3>
		A3D_FORCEINLINE float128 VectorShuffle(const float128& v)
		{
		#ifdef __clang__
			return __builtin_shufflevector(v, v, p0, p1, p2, p3);
		#else
			float128 result;
			result.n128_f32[0] = v.n128_f32[p0];
			result.n128_f32[1] = v.n128_f32[p1];
			result.n128_f32[2] = v.n128_f32[p2];
			result.n128_f32[3] = v.n128_f32[p3];
		#endif
		}

		template<unsigned p0, unsigned p1, unsigned p2, unsigned p3>
		A3D_FORCEINLINE float128 VectorShuffle(const float128& v1, const float128& v2)
		{
		#ifdef __clang__
			return __builtin_shufflevector(v, v, p0, p1, p2, p3);
		#else
			float128 result;
			result.n128_f32[0] = v1.n128_f32[p0];
			result.n128_f32[1] = v1.n128_f32[p1];
			result.n128_f32[2] = v2.n128_f32[p2];
			result.n128_f32[3] = v2.n128_f32[p3];
			return result;
		#endif
		}

		//latency 3
		A3D_FORCEINLINE float128 VectorAdd(const float128& v1, const float128& v2)
		{
			return vaddq_f32(v1, v2);
		}

		A3D_FORCEINLINE float128 VectorSub(const float128& v1, const float128& v2)
		{
			return vsubq_f32(v1, v2);
		}

		A3D_FORCEINLINE float128 VectorMul(const float128& v1, const float128& v2)
		{
			return vmulq_f32(v1, v2);
		}

		A3D_FORCEINLINE float128 VectorDiv(const float128& v1, const float128& v2)
		{
			return VectorMul(v1, VectorReciprocal(v2));
		}

		A3D_FORCEINLINE float128 VectorEquals(const float128& v1, const float128& v2)
		{
			return vceqq_f32(v1, v2);
		}

		A3D_FORCEINLINE float128 VectorNotEquals(const float128& v1, const float128& v2)
		{
			return vmvnq_u32(vceqq_f32(v1, v2));
		}

		A3D_FORCEINLINE float128 VectorGreater(const float128& v1, const float128& v2)
		{
			return vcgtq_f32(v1, v2);
		}

		A3D_FORCEINLINE float128 VectorLess(const float128& v1, const float128& v2)
		{
			return vcleq_f32(v1, v2);
		}

		A3D_FORCEINLINE float128 VectorGreaterOrEqual(const float128& v1, const float128& v2)
		{
			return vcgeq_f32(v1, v2);
		}

		A3D_FORCEINLINE float128 VectorLessOrEqual(const float128& v1, const float128& v2)
		{
			return vcltq_f32(v1, v2);
		}

		A3D_FORCEINLINE float128 VectorOr(const float128& v1, const float128& v2)
		{
			return (float128)vorrq_u32((uint128)v1, (uint128)v2);
		}

		A3D_FORCEINLINE float128 VectorAnd(const float128& v1, const float128& v2)
		{
			return (float128)vmvnq_u32(vandq_u32((uint128)v1, (uint128)v2));
		}

		A3D_FORCEINLINE float128 VectorAndNot(const float128& v1, const float128& v2)
		{
			return (float128)vmvnq_u32(vandq_u32((uint128)v1, (uint128)v2));
		}

		A3D_FORCEINLINE float128 VectorXor(const float128& v1, const float128& v2)
		{
			return (float128)veorq_u32((uint128)v1, (uint128)v2);
		}

		A3D_FORCEINLINE float128 VectorMax(const float128& v1, const float128& v2)
		{
			return vmaxq_f32(v1, v2);
		}

		A3D_FORCEINLINE float128 VectorMin(const float128& v1, const float128& v2)
		{
			return vminq_f32(v1, v2);
		}



		//all is bigger than 0
		A3D_FORCEINLINE bool VectorTrue(const float128& v)
		{
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vTemp1), vget_high_u8(vTemp1));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			return 0xFFFFFFFFu == vget_lane_u32(vTemp.val[1], 1);
		}

		//all is zero
		A3D_FORCEINLINE bool VectorFalse(const float128& v)
		{
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vTemp1), vget_high_u8(vTemp1));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			return 0 == vget_lane_u32(vTemp.val[1], 1);
		}

		//some is bigger than 0, and some is 0
		A3D_FORCEINLINE bool VectorAnyTrue(const float128& v)
		{
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vTemp1), vget_high_u8(vTemp1));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			return 0 != vget_lane_u32(vTemp.val[1], 1);
		}

		//some is bigger than 0, and some is 0
		A3D_FORCEINLINE bool VectorAnyFalse(const float128& v)
		{
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vTemp1), vget_high_u8(vTemp1));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			return 0xFFFFFFFFu != vget_lane_u32(vTemp.val[1], 1);
		}

		A3D_FORCEINLINE float128 VectorDot4(const float128& v1, const float128& v2)
		{
			float128 multi = vmulq_f32(v1, v2);  //x,y,z,w  
			float64 sum = vpadd_f32(vget_low_f32(multi), vget_high_f32(multi)); //x+z, y+w
			sum = vpadd_f32(sum, sum);
			return vdupq_lane_f32(sum, 0);
		}

		A3D_FORCEINLINE float128 VectorSumUp(const float128& v)
		{
			float64 sum = vpadd_f32(vget_low_f32(v), vget_high_f32(v));
			sum = vpadd_f32(sum, sum);
			return vdupq_lane_f32(sum, 0);
		}

		A3D_FORCEINLINE float128 VectorSelect(const float128& v1,
			const float128& v2, const float128& mask)
		{
			return vbslq_f32(mask, v1, v2);
		}

		A3D_FORCEINLINE float128 VectorIntPart(const float128& v)
		{
		#ifdef _M_ARM64
			return vrndq_f32(v);
		#else
			float128 int_part;
			int_part.n128_f32[0] = (int)v.n128_f32[0];
			int_part.n128_f32[1] = (int)v.n128_f32[1];
			int_part.n128_f32[2] = (int)v.n128_f32[2];
			int_part.n128_f32[3] = (int)v.n128_f32[3];
			return int_part;
		#endif
		}

		//  float int_part = (int) v[i];
		//  float floor = int_part - ( v[i] > 0 ? 0: 1 )   
		//  float round = floor + ( v[i] - int_part > 0.5 ? 1.0:0.0f );
		A3D_FORCEINLINE float128 VectorRound(const float128& v)
		{
			static const float128 kZero = VectorLoad(0.0f);
			static const float128 kHalf = VectorLoad(0.5f);
			static const float128 kOne = VectorLoad(1.0f);
			float128 int_part = VectorIntPart(v);
			float128 floor = VectorSelect(VectorGreater(v, kZero), kOne, kZero);
			float128 add = VectorSelect(VectorGreater(VectorSub(v, floor), kHalf), kOne, kZero);
			return VectorAdd(floor, add);
		}

		A3D_FORCEINLINE float128 VectorAbs(const float128& v)
		{
			return vabsq_f32(v);
		}

		A3D_FORCEINLINE float128 VectorNegate(const float128& v)
		{
			return vnegq_f32(v);
		}

		A3D_FORCEINLINE float128 VectorReciprocalApproximate(const float128& v)
		{
			return vrecpeq_f32(v);
		}

		A3D_FORCEINLINE float128 VectorReciprocal(const float128& v)
		{
			float128 recp = vrecpeq_f32(v);
			recp = vmulq_f32(vrecpsq_f32(v, recp), recp);
			return vmulq_f32(vrecpsq_f32(v, recp), recp);
		}

		A3D_FORCEINLINE float128 VectorReciprocalSqrtApproximate(const float128& v)
		{
			return vrsqrteq_f32(v);
		}

		A3D_FORCEINLINE float128 VectorReciprocalSqrt(const float128& v)
		{
			// 2 iterations of Newton-Raphson refinement of reciprocal
			float128 S0 = vrsqrteq_f32(v);
			float128 P0 = vmulq_f32(v, S0);
			float32x4_t R0 = vrsqrtsq_f32(P0, S0);
			S0 = vmulq_f32(S0, R0);
			P0 = vmulq_f32(v, S0);
			R0 = vrsqrtsq_f32(P0, S0);
			return vmulq_f32(S0, R0);
		}

		A3D_FORCEINLINE float128 VectorSqrtApproximate(const float128& v)
		{
			const float128 S0 = vrsqrteq_f32(v);
			return vmulq_f32(v, S0);
		}

		A3D_FORCEINLINE float128 VectorSqrt(const float128& v)
		{
			// 2 iteration of Newton-Raphson refinment of sqrt
			float128 S0 = vrsqrteq_f32(v);
			float128 P0 = vmulq_f32(v, S0);
			float128 R0 = vrsqrtsq_f32(P0, S0);

			S0 = vmulq_f32(R0, S0);
			P0 = vmulq_f32(v, S0);
			return vrsqrtsq_f32(P0, S0);
		}

		//return (v1[0], v2[0], v1[1], v2[1])
		A3D_FORCEINLINE float128 VectorInterleaveXY(const float128& v1
			, const float128& v2)
		{
			return vzipq_f32(v1, v2).val[0];
		}

		A3D_FORCEINLINE float128 VectorInterleaveZW(const float128& v1
			, const float128& v2)
		{
			return vzipq_f32(v1, v2).val[1];
		}


		//return start + t*(end -start)
		A3D_FORCEINLINE float128 VectorLerp(const float128& start, const float128& end, float t)
		{
			return vmlaq_n_f32(start, vsubq_f32(end, start), t);
		}
	#endif //#if AURORA3D_SSE #elif AURORA3D_NEON

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////// Vector Operation Extension /////////////////////////////////////////////////////////////
#if 1   //Vector Function Extension

		//return (v[0],v[1],v[2],0.0f)
		A3D_FORCEINLINE void VectorSetW0( float128& v)
		{
			return VectorSet(v, 3, 0.0f);
		}

		//return (v[0],v[1],v[2],1.0f)
		A3D_FORCEINLINE void VectorSetW1( float128& v)
		{
			return VectorSet(v, 3, 1.0f);
		}

		// v1 = v1 + v2
		A3D_FORCEINLINE void VectorAddAssign(float128& v1, const float128& v2)
		{
			v1 = VectorAdd(v1, v2);
		}

		//return add1 + add2 + add3
		A3D_FORCEINLINE float128 VectorAddTwice(const float128& v1, const float128& v2, const float128& v3)
		{
			return VectorAdd(VectorAdd(v1, v2), v3);
		}

		//return add1 + add2 - Sub
		A3D_FORCEINLINE float128 VectorAddSub(const float128& add1, const float128& add2, const float128& Sub)
		{
			return VectorSub(VectorAdd(add1, add2), Sub);
		}

		//return (add1 + add2 )*mul
		A3D_FORCEINLINE float128 VectorAddMutiply(const float128& add1, const float128& add2, const float128& mul)
		{
			return VectorMul(VectorAdd(add1, add2), mul);
		}

		//return (add1+add2) / div
		A3D_FORCEINLINE float128 VectorAddDiv(const float128& add1, const float128& add2, const float128& div)
		{
			return VectorDiv(VectorAdd(add1, add2), div);
		}

		// v1 = v1+v2
		A3D_FORCEINLINE void VectorSubAssign(float128& v1, const float128 v2)
		{
			v1 = VectorSub(v1, v2);
		}

		//return Sub1 - Sub2 - Sub3
		A3D_FORCEINLINE float128 VectorSubTwice(const float128& Sub1, const float128& Sub2, const float128& Sub3)
		{
			return VectorSub(VectorSub(Sub1, Sub2), Sub3);
		}

		//return Sub1 - Sub2 + Sub3
		A3D_FORCEINLINE float128 VectorSubAdd(const float128& Sub1, const float128& Sub2, const float128& add3)
		{
			return VectorSub(VectorAdd(Sub1, Sub2), add3);
		}

		//return (Sub1-Sub2)*mul
		A3D_FORCEINLINE float128 VectorSubMutiply(const float128& Sub1, const float128& Sub2, const float128& mul)
		{
			return VectorMul(VectorSub(Sub1, Sub2), mul);
		}

		//return (Sub1-Sub2)/div
		A3D_FORCEINLINE float128 VectorSubDiv(const float128& Sub1, const float128& Sub2, const float128& div)
		{
			return VectorDiv(VectorSub(Sub1, Sub2), div);
		}

		//return ( mul1[0]*scale, mul1[1]*scale, mul1[2]*scale, mul1[3]*scale )
		A3D_FORCEINLINE float128 VectorMul(const float128& mul1, float scale)
		{
			return VectorMul(mul1, VectorLoad(scale));
		}

		// return
		A3D_FORCEINLINE void VectorMulAssign(float128& v1, const float128& v2)
		{
			v1 = VectorMul(v1, v2);
		}

		// return
		A3D_FORCEINLINE void VectorMulAssign(float128& v, float scale)
		{
			v = VectorMul(v, scale);
		}


		//return mul1 * mul2 * mul3
		A3D_FORCEINLINE float128 VectorMulTwice(const float128& mul1, const float128& mul2, const float128& mul3)
		{
			return VectorMul(mul1, VectorMul(mul2, mul3));
		}

		//return mul1 * mul2 + add
		A3D_FORCEINLINE float128 VectorMulAdd(const float128& mul1, const float128& mul2, const float128& add)
		{
			return VectorAdd(VectorMul(mul1, mul2), add);
		}

		//return mul1 * mul2 - Sub
		A3D_FORCEINLINE float128 VectorPreMulSub(const float128& mul1, const float128& mul2, const float128& Sub)
		{
			return VectorSub(VectorMul(mul1, mul2), Sub);
		}

		//return v1 - v2*v3
		A3D_FORCEINLINE float128 VectorPostMulSub(const float128& v1, const float128& v2, const float128& v3)
		{
			return VectorSub(v1, VectorMul(v2, v3));
		}


		//return mul1 * mul2 / div
		A3D_FORCEINLINE float128 VectorPreMulDiv(const float128& mul1, const float128& mul2, const float128& div)
		{
			return VectorDiv(VectorMul(mul1, mul2), div);
		}

		//return v1 / (v2*v3)
		A3D_FORCEINLINE float128 VectorPostMulDiv(const float128& v1, const float128& v2, const float128& v3)
		{
			return VectorDiv(v1, VectorMul(v2, v3));
		}

		// v1 = v1 / v2
		A3D_FORCEINLINE void VectorDivAssign(float128& v1, const float128& v2)
		{
			v1 = VectorDiv(v1, v2);
		}

		A3D_FORCEINLINE float128 VectorDiv(const float128& v1, float s)
		{
			return VectorDiv(v1, VectorLoad(s));
		}

		// v1 = v1 / v2
		A3D_FORCEINLINE void VectorDivAssign(float128& v1, float s)
		{
			v1 = VectorDiv(v1, s);
		}

		//return v1 / v2 + v3
		A3D_FORCEINLINE float128 VectorDivAdd(const float128& v1, const float128& v2, const float128& v3)
		{
			return VectorSub(VectorDiv(v1, v2), v3);
		}

		//return v1 / v2 - v3
		A3D_FORCEINLINE float128 VectorPreDivSub(const float128& v1, const float128& v2, const float128& v3)
		{
			return VectorSub(VectorDiv(v1, v2), v3);
		}

		//return v1 - v2/v3
		A3D_FORCEINLINE float128 VectorPostDivSub(const float128& v1, const float128& v2, const float128& v3)
		{
			return VectorSub(v1, VectorDiv(v2, v3));
		}

		//return mod1 - (int)(mod1/mod2)*mod2
		A3D_FORCEINLINE float128 VectorMod(const float128& mod1, const float128& mod2)
		{
			return VectorSub(mod1, VectorMul(mod2, VectorIntPart(VectorDiv(mod1, mod2))));
		}

		// IF abs(v1 - v2) < EPSIDE, return 0xffffffff otherwise is 0
		A3D_FORCEINLINE float128 VectorNearlyEquals(const float128& v1, const float128& v2, float epside)
		{
			return VectorLess(VectorAbs(VectorSub(v1, v2)), VectorLoad(epside));
		}

	


		// F = v1.x*v2.x + v1.y*v2.y
		// return (F,F, undef, undef)
		A3D_FORCEINLINE float128 VectorDot2(const float128& v1, const float128& v2)
		{
			float128 multi = VectorMul(v1, v2);
			return  VectorAdd(multi, VectorShuffle<1, 0, 2, 3>(multi));
		}

		// F = v1.x*v2.x + v1.y*v2.y + v1.z*v2.z
		// return (F,F,F, undef )
		A3D_FORCEINLINE float128 VectorDot3(const float128& v1, const float128& v2)
		{
			float128 multi = VectorMul(v1, v2);                       //x,y,z
			multi = VectorAdd(multi, VectorShuffle<1, 2, 0, 3>(multi)); //x+y, y+z, z+x
			return VectorAdd(multi, VectorShuffle<2, 0, 1, 3>(multi));
		}

		// v = point1 - point2;
		// distance = sqrt(v.x^2 + v.y^2), z,w is ingored
		// return (distance, distance, undef, undef )
		A3D_FORCEINLINE float128 VectorDistance2(const float128& point1, const float128& point2)
		{
			float128 vec = VectorSub(point1, point2);
			return VectorSqrt(VectorDot2(vec, vec));
		}


		// v = point1 - point2
		// distance = sqrt(v.x^2 + v.y^2 + v.z^2), w is ingored
		// return (distance, distance, distance, undef)
		A3D_FORCEINLINE float128 VectorDistance3(const float128& point1, const float128& point2)
		{
			float128 vec = VectorSub(point1, point2);
			return VectorSqrt( VectorDot3(vec, vec));
		}
		
		// distance = sqrt(v.x^2 + v.y^2), z,w is ingored
		// return (distance, distance, undef, undef )
		A3D_FORCEINLINE float128 VectorLength2(const float128& v)
		{
			return VectorSqrt(VectorDot2(v, v));
		}

		// distance = sqrt(v.x^2 + v.y^2 + v.z^2), w is ingored
		// return (distance, distance, distance, undef)
		A3D_FORCEINLINE float128 VectorLength3(const float128& v)
		{
			return VectorSqrt(VectorDot3(v, v));
		}

		// F = x + y
		// return (F,F,undef,undef)
		A3D_FORCEINLINE float128 VectorSumUp2(const float128& v)
		{
			return VectorAdd(v, VectorShuffle<1, 0, 2, 3>(v));
		}

		// F = x + y + z
		// return (F,F,F, undf )
		A3D_FORCEINLINE float128 VectorSumUp3(const float128& v)
		{
			float128 rotate = VectorShuffle<1, 2, 0, 3>(v); //y,z,x
			rotate = VectorAdd(v, rotate);                //x+y,y+z,z+x
			return VectorAdd(VectorShuffle< 2, 0, 1, 3>(v), rotate);
		}

		// v[i] < 0, return -1, otherwise 1
		A3D_FORCEINLINE float128 VectorSign(const float128& v)
		{
			//+ float , sign bit = 0,
			//- float , sign bit = 1
			return VectorSelect(math_impl::kVectorOne, math_impl::kVectorNegtiveOne,
				VectorEquals(math_impl::kVectorZero,VectorAnd(v, math_impl::kVectorSignMask)));
		}

		//return normalized(x,y, undef, undef)
		A3D_FORCEINLINE float128 VectorNormalize2(const float128& v)
		{
			return VectorMul(v, VectorReciprocalSqrt(VectorDot2(v, v)));
		}

		// return normalized(x,y,z, undef )
		A3D_FORCEINLINE float128 VectorNormalize3(const float128& v)
		{
			return VectorMul(v, VectorReciprocalSqrt(VectorDot3(v, v)));
		}

		// return normalize(x,y,z,w)
		A3D_FORCEINLINE float128 VectorNormalize4(const float128& v)
		{
			return VectorMul(v, VectorReciprocalSqrt(VectorDot4(v, v)));
		}

		//v[i] > 0 return 1, otherwise return 0
		A3D_FORCEINLINE float128 VectorStep(const float128& v)
		{
			return VectorSelect(math_impl::kVectorOne, math_impl::kVectorZero, VectorGreater(v, math_impl::kVectorZero));
		}

		//v[i] < 0 return 1, otherwise return 0
		A3D_FORCEINLINE float128 VectorReverseStep(const float128& v)
		{
			return VectorSelect(math_impl::kVectorOne, math_impl::kVectorZero, VectorLess(v, math_impl::kVectorZero));
		}

		//return v[i] - (int)v[i]
		A3D_FORCEINLINE float128 VectorFracPart(const float128& v)
		{
			return VectorSub(v, VectorIntPart(v));
		}

		//return ceil(v[i]), 
		A3D_FORCEINLINE float128 VectorCeil(const float128& v)
		{
			float128 integer = VectorIntPart(v);
			float128 add = VectorStep(v);
			return VectorAdd(integer, add);
		}

		//return floor(v[i])
		A3D_FORCEINLINE float128 VectorFloor(const float128& v)
		{
			float128 integer = VectorIntPart(v);
			float128 step = VectorReverseStep(v);
			return VectorSub(integer, step);
		}

		// abs(v[0,1])<bound[0,1] return true 
		A3D_FORCEINLINE bool VectorInBound2(const float128& v,
			const float128& bound)
		{
			const float128 abs = VectorAbs(v);
			return VectorTrue2(VectorLess(abs, bound));
		}

		// abs(v[0,1,2])<bound[0,1,2] return true 
		A3D_FORCEINLINE bool VectorInBound3(const float128&v, const float128& bound)
		{
			const float128 abs = VectorAbs(v);
			return VectorTrue3(VectorLess(abs, bound));
		}


		//abs(v[0,1,2,3])<bound[0,1,2,3] return true 
		A3D_FORCEINLINE bool VectorInBound4(const float128& v,
			const float128& bound)
		{
			const float128 abs = VectorAbs(v);
			return VectorTrue4(VectorLess(abs, bound));
		}

		// one of v[0,1] = NaN
		A3D_FORCEINLINE bool VectorIsNaN2(const float128& v)
		{
			//NaN != NaN is true
			return VectorAnyTrue4(VectorNotEquals(v, v));
		}

		// one of v[0,1,2] = NaN
		A3D_FORCEINLINE bool VectorIsNaN3(const float128& v)
		{
			//NaN != NaN is true
			return VectorAnyTrue4(VectorNotEquals(v, v));
		}

		// one of v[0,1,2,3] = NaN
		A3D_FORCEINLINE bool VectorIsNaN4(const float128& v)
		{
			//NaN != NaN is true
			return VectorAnyTrue4(VectorNotEquals(v, v));
		}

		// one of abs(v[0,1]) = INF
		A3D_FORCEINLINE bool VectorIsInfinite2(const float128& v)
		{
			return VectorAnyTrue4(VectorEquals(VectorAbs(v), math_impl::kVectorInfinte));
		}

		// one of abs(v[0,1,2]) = INF
		A3D_FORCEINLINE bool VectorIsInfinite3(const float128& v)
		{
			return VectorAnyTrue4(VectorEquals(VectorAbs(v), math_impl::kVectorInfinte));
		}

		// one of abs(v[0,1,2,3]) = INF
		A3D_FORCEINLINE bool VectorIsInfinite4(const float128& v)
		{
			return VectorAnyTrue4(VectorEquals(VectorAbs(v), math_impl::kVectorInfinte));
		}

		// v[i]>max[i], v[i] = max[i]
		// v[i]<min[i], v[i] = min[i]
		A3D_FORCEINLINE float128 VectorClamp(const float128& v,
			const float128& min, const float128& max)
		{
			return VectorMax(VectorMin(v, max), min);
		}

		// v[i]>1.0, v[i] = 1.0
		// v[i]<0.0, v[i] = 0.0
		A3D_FORCEINLINE float128 VectorSaturate(const float128& v)
		{
			return VectorMax(VectorMin(v, math_impl::kVectorOne), math_impl::kVectorZero);
		}

		//  2D ray reflect
		//  incident - 2 * dot(incident, normal)*normal 
		A3D_FORCEINLINE float128 VectorReflect2(const float128 incident, const float128& normal)
		{
			return VectorSub(incident, VectorMulTwice(math_impl::kVectorTwo, normal, VectorDot2(incident, normal)));
		}

		// 3D ray reflect
		//      incident   reflect
		//          \  |  /|
		//           \ | /
		//     _____ _\|/_________
		//     incident - 2 * dot(incident, normal)*normal 
		// return (x,y,z, undef) 
		A3D_FORCEINLINE float128 VectorReflect3(const float128 incident, const float128& normal)
		{
			return VectorSub(incident, VectorMulTwice(math_impl::kVectorTwo, normal, VectorDot3(incident, normal)));
		}

		// W is undef 
		A3D_FORCEINLINE float128 VectorCrossProduct3(const float128& v1, const float128& v2)
		{
			float128 v1_yzxw = VectorShuffle<1, 2, 0, 3>(v1);
			float128 v2_zxyw = VectorShuffle<2, 0, 1, 3>(v2);
			float128 v1_zxyw = VectorShuffle<2, 0, 1, 3>(v1);
			float128 v2_yzxw = VectorShuffle<1, 2, 0, 3>(v2);

			//y2 * z1 , x2*z1 ,  y2*x1 - 
			//y1 * z2 , z1*x1 ,  x2*y1 
			//y2 * z1 - y1 * z2 , x2 * z1 - x1 * z2, y2*x1 - x2-y1
			return VectorSub(VectorMul(v1_yzxw, v2_zxyw),
				VectorMul(v1_zxyw, v2_yzxw));
		}

		namespace SinCosConstant
		{
			constexpr float a = 7.58946638440411f;
			constexpr float b = 0.5256583509747431f;
			constexpr float s0 = -0.16666667f;
			constexpr float s1 = 0.0083333310f;
			constexpr float s2 = 0.00019840874f;
			constexpr float s3 = 2.7525562e-06f;
			constexpr float s4 = -2.3889859e-08f;
			constexpr float c0 = -0.5f;
			constexpr float c1 = 0.041666638f;
			constexpr float c2 = -0.0013888378f;
			constexpr float c3 = 2.4760495e-05f;
			constexpr float c4 = -2.6051615e-07f;
			static const float128 A = VectorLoad(a, a, a, a);
			static const float128 B = VectorLoad(b, b, b, b);
			static const float128 S0 = VectorLoad(s0, s0, s0, s0);
			static const float128 S1 = VectorLoad(s1, s1, s1, s1);
			static const float128 S2 = VectorLoad(s2, s2, s2, s2);
			static const float128 S3 = VectorLoad(s3, s3, s3, s3);
			static const float128 S4 = VectorLoad(s4, s4, s4, s4);
			static const float128 C0 = VectorLoad(c0, c0, c0, c0);
			static const float128 C1 = VectorLoad(c1, c1, c1, c1);
			static const float128 C2 = VectorLoad(c2, c2, c2, c2);
			static const float128 C3 = VectorLoad(c3, c3, c3, c3);
			static const float128 C4 = VectorLoad(c4, c4, c4, c4);
		}

		//Sine approximation using a squared parabola restrained to f(0) = 0, f(PI) = 0, f(PI/2) = 1.
		//After approx 2.5 million tests comparing to sin(): 
		//Average error of 0.000128
		//Max error of 0.001091
		// degree
		A3D_FORCEINLINE float128 VectorSinRadianFast(const float128& v)
		{
			//based on a good discussion here http://forum.devmaster.net/t/fast-and-accurate-sine-cosine/9648
			float128 result = VectorMul(v, math_impl::kVectorOneOver2PI);
			result = VectorMul(v, math_impl::kVectorOneOver2PI);
			result = VectorSub(result, VectorRound(result));
			result = VectorMul(SinCosConstant::A,VectorMul(result,
				VectorSub(math_impl::kVectorHalf, VectorAbs(result))));
			return VectorMul(result, VectorAdd(SinCosConstant::B,
				VectorAbs(result)));
		}

		A3D_FORCEINLINE float128 VectorRadianToDegree(const float128& v)
		{
			return VectorMul(v, math_impl::kVector180OverPI);
		}

		A3D_FORCEINLINE float128 VectorDegreeToRadian(const float128& v)
		{
			return VectorMul(v, math_impl::kVectorPIOver180);
		}

		A3D_FORCEINLINE float128 VectorSinDegreeFast(const float128& v)
		{
			return VectorSinRadianFast(VectorDegreeToRadian(v));
		}

		A3D_FORCEINLINE float128 VectorCosRadianFast(const float128& v)
		{
			return VectorSinRadianFast(VectorAdd(v, math_impl::kVectorHalfPI));
		}

		A3D_FORCEINLINE float128 VectorCosDegreeFast(const float128& v)
		{
			return VectorSinRadianFast(VectorAdd(
				VectorDegreeToRadian(v), math_impl::kVectorHalfPI));
		}

		A3D_FORCEINLINE void VectorSinAndCosFast(float128& sin, float128& cos, const float128& radians)
		{
			//A = A - 2PI * round(A/2PI);
			float128 A = VectorRound(VectorMul(radians, math_impl::kVectorOneOver2PI));
			A = VectorSub(radians, VectorMul(A, math_impl::kVectorOneOver2PI));
			const float128 abs_A = VectorAbs(A);

			//mapping to [-PI/2, PI/2]
			float128 sign = VectorSign(radians);
			float128 sign_pi = VectorOr(sign, math_impl::kVectorPI);
			float128 refl = VectorSub(sign_pi, abs_A);
			float128 cmp = VectorGreater(A, math_impl::kVectorHalfPI);
			A = VectorSelect(refl, A, cmp);
			sign = VectorSelect(math_impl::kVectorNegtiveOne, math_impl::kVectorOne, cmp);
			const float128 AA = VectorMul(A, A);

			//sin
			sin = SinCosConstant::S4;
			sin = VectorMulAdd(AA, sin, SinCosConstant::S3);
			sin = VectorMulAdd(AA, sin, SinCosConstant::S2);
			sin = VectorMulAdd(AA, sin, SinCosConstant::S1);
			sin = VectorMulAdd(AA, sin, SinCosConstant::S0);
			sin = VectorMulAdd(AA, sin, math_impl::kVectorOne);
			sin = VectorMul(A, sin);

			cos = SinCosConstant::C4;
			cos = VectorMulAdd(AA, cos, SinCosConstant::C3);
			cos = VectorMulAdd(AA, cos, SinCosConstant::C2);
			cos = VectorMulAdd(AA, cos, SinCosConstant::C1);
			cos = VectorMulAdd(AA, cos, SinCosConstant::C0);
			cos = VectorMulAdd(AA, cos, math_impl::kVectorOne);
			cos = VectorMul(sign, cos);
		}
#endif  

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////// Matrix Operation Implements ///////////////////////////////////////////////////////////
#if 1   
		// must match every elements
		A3D_FORCEINLINE bool MatrixIsIdentity(const float128x4& M)
		{
			float128 result = VectorEquals(M[0], math_impl::kVectorXOne);
			result = VectorAnd(VectorEquals(M[1], math_impl::kVectorYOne), result);
			result = VectorAnd(VectorEquals(M[2], math_impl::kVectorZOne), result);
			result = VectorAnd(VectorEquals(M[3], math_impl::kVectorWOne), result);
			return VectorTrue4(result);
		}

		// once one element is NaN, matrix is NaN,
		A3D_FORCEINLINE bool MatrixIsNaN(const float128x4& M)
		{
			// NaN != Nan is true , NaN == NaN is false
			float128 result = VectorNotEquals(M[0], M[0]);
			result = VectorOr(VectorNotEquals(M[1], M[1]), result);
			result = VectorOr(VectorNotEquals(M[2], M[2]), result);
			result = VectorOr(VectorNotEquals(M[3], M[3]), result);
			return VectorAnyTrue4(result);
		}

		//one element is infinite , matrix is infinite
		A3D_FORCEINLINE bool MatrixIsInInfinite(const float128x4& M)
		{
			float128 result = VectorEquals(VectorAbs(M[0]), math_impl::kVectorInfinte);
			result = VectorOr(VectorEquals(VectorAbs(M[1]), math_impl::kVectorInfinte), result);
			result = VectorOr(VectorEquals(VectorAbs(M[2]), math_impl::kVectorInfinte), result);
			result = VectorOr(VectorEquals(VectorAbs(M[3]), math_impl::kVectorInfinte), result);
			return VectorAnyTrue4(result);
		}


		//16 times Mul and 12 times add
		A3D_FORCEINLINE void MatrixMulNoCopy(const float128x4& A, const float128x4& B, float128x4& C)
		{
			/*
			*	A00 A01 A02 A03       B00 B01 B02 B03      C00 C01 C02 C03
			*	A10 A11 A12 A13   X   B10 B11 B12 B13   =  prefix,...
			*	A20 A21 A22 A23       B20 B21 B22 B23
			*	A30 A31 A32 A33       B30 B31 B32 B33
			*
			*  C00 = A00*B00 + A01*B10 + A02*B20 + A03*B30
			*  C01 = A00*B01 + A01*B11 + A02*B21 + A03*B31
			*  C02 = A00*B02 + A01*B12 + A02*B22 + A03*B32
			*  C03 = A00*B03 + A)1*B12 + A02*B23 + A03*B33
			*
			*  [C00 C01 C02 C03] = A00 * B[0] + A01 * B[1] + A02 * B[2] + A03 * B[3]
			*/
			float128 temp;

			///first row
			temp = VectorMul(VectorReplicate<0>(A[0]), B[0]);
			temp = VectorMulAdd(VectorReplicate<1>(A[0]), B[1], temp);
			temp = VectorMulAdd(VectorReplicate<2>(A[0]), B[2], temp);
			C[0] = VectorMulAdd(VectorReplicate<3>(A[0]), B[3], temp);

			// second row
			temp = VectorMul(VectorReplicate<0>(A[1]), B[0]);
			temp = VectorMulAdd(VectorReplicate<1>(A[1]), B[1], temp);
			temp = VectorMulAdd(VectorReplicate<2>(A[1]), B[2], temp);
			C[1] = VectorMulAdd(VectorReplicate<3>(A[1]), B[3], temp);

			//third row
			temp = VectorMul(VectorReplicate<0>(A[2]), B[0]);
			temp = VectorMulAdd(VectorReplicate<1>(A[2]), B[1], temp);
			temp = VectorMulAdd(VectorReplicate<2>(A[2]), B[2], temp);
			C[2] = VectorMulAdd(VectorReplicate<3>(A[2]), B[3], temp);

			//forth row
			temp = VectorMul(VectorReplicate<0>(A[4]), B[0]);
			temp = VectorMulAdd(VectorReplicate<1>(A[4]), B[1], temp);
			temp = VectorMulAdd(VectorReplicate<2>(A[4]), B[2], temp);
			C[4] = VectorMulAdd(VectorReplicate<3>(A[4]), B[3], temp);
		}


		A3D_FORCEINLINE float128x4 MatrixMul(const float128x4& A, const float128x4& B)
		{
			float128x4 C;
			MatrixMulNoCopy(A, B, C);
			return C;
		}

		//7 times Mul, 10 times shuffle, 2 times Sub, 2 times add
		A3D_FORCEINLINE float128 MatrixDeterminant(const float128x4& M)
		{
			float128 mat2_result[2], accumulate[3], det;
			mat2_result[0] = VectorShuffle<1, 2, 3, 0>(M[3]);
			mat2_result[1] = VectorShuffle<2, 3, 0, 1>(M[3]);
			//9*14, 10*15, 11*16, 12*13
			mat2_result[0] = VectorMul(mat2_result[0], M[2]);
			//9*15, 10*16, 11*13, 12*14
			mat2_result[1] = VectorMul(mat2_result[1], M[2]);
			det = VectorShuffle<1, 2, 3, 0>(M[2]);
			// 10*13, 11*14, 12*15, 9*16
			det = VectorMul(det, M[3]);
			//9*14-10*13, 10*15-11*14, 11*16-12*15, 12*13-9*16
			mat2_result[0] = VectorSub(mat2_result[0], det);
			det = VectorShuffle<2, 3, 0, 1>(M[2]);
			//11*13, 12*14, 9*15, 10*16 
			det = VectorMul(det, M[3]);
			//9*15-11*13, 10*16-12*14, 11*13-9*15, 12*14-10*16
			mat2_result[1] = VectorSub(mat2_result[1], det);

			//6,7,8,5
			accumulate[0] = VectorShuffle<1, 2, 3, 0>(M[1]);
			//7,8,5,6
			accumulate[1] = VectorShuffle<2, 3, 0, 1>(M[1]);
			//8,5,6,7
			accumulate[2] = VectorShuffle<3, 0, 1, 2>(M[1]);

			//6*(11*16-12*15) , 7*(12*13-9*16), 8*(9*14-10*13), 5*(10*15-11*14)
			det = VectorMul(accumulate[0], VectorShuffle<2, 3, 0, 1>(mat2_result[0]));
			//7*(12*14-10*16) , 8*(9*15-11*13), 5*(10*16-12*14), 6*(11*13-9*15)
			det = VectorMulAdd(accumulate[1], VectorShuffle<3, 0, 1, 2>(mat2_result[1]), det);
			//8*(10*15-11*14),  5*(11*16-12*15), 6*(12*13-9*16), 7*(9*14-10*13)
			det = VectorMulAdd(accumulate[2], VectorShuffle<2, 3, 0, 1>(mat2_result[0]), det);
			//+,-,+,-
			return VectorMul(det, math_impl::kVectorOddNegtive);
		}


		A3D_FORCEINLINE void MatrixTransposeNoCopy(const float128x4& original, float128x4& transpose)
		{
			/*
			*	1  2  3  4       1 5  9 13
			*	5  6  7  8   ->  2 6 10 14
			*	9 10 11 12       3 7 11 15
			*  13 14 15 16       4 8 12 16
			*/
			float128 block1 = VectorShuffle< 0, 1, 0, 1>(original[0], original[1]); //1,2,5,6
			float128 block2 = VectorShuffle< 2, 3, 2, 3>(original[0], original[1]); //3,4,7,8
			float128 block3 = VectorShuffle<0, 1, 0, 1>(original[2], original[3]); //9,10,13,14
			float128 block4 = VectorShuffle<2, 3, 2, 3>(original[2], original[3]); //11,12,15,16
			transpose[0] = VectorShuffle<0, 2, 0, 2>(block1, block3); //1,5,9,13
			transpose[1] = VectorShuffle<1, 3, 1, 3>(block1, block3); //2,6,10,14
			transpose[2] = VectorShuffle<0, 2, 0, 2>(block2, block4); //3,7,11,15
			transpose[3] = VectorShuffle<1, 3, 1, 3>(block2, block4); //4,8,12,16
		}

		A3D_FORCEINLINE float128x4 MatrixTranspose(const float128x4& A)
		{
			float128x4 transpose;
			MatrixTransposeNoCopy(A, transpose);
			return transpose;
		}

		//22 times Mul, 8 times Sub, 6 times add, 44 times shuffle
		//return determinant
		A3D_FORCEINLINE float128 MatrixInverse(const float128x4& original, float128x4& inverse)
		{
			/*
			*  1 5  9 13
			*  2 6 10 14
			*  3 7 11 15
			*  4 8 12 16
			*
			*  + 1, 9,3,11
			*  - 5,13,7,15
			*  - 2,10,4,12
			*  + 6,14,8,16
			*
			*  mat2  have two part.   like 1*6 - 2*5
			*  mat3  have three part, like 1*(6*11-7*19) + 5*(10*3-2*11) + 9*(2*7-3*6)
			*/

			//transpose and get determinant
			float128x4 transpose;
			MatrixTransposeNoCopy(original, transpose);

			//[1-6,5-10, 9-14, 13 -2] and [3-8, 7-12, 11-16, 15-4 ] 
			float128 counter[2];
			//[1-10, 5-14, 3-12, 7-16]
			float128 jump;
			float128 mat2_result[4];
			counter[0] = VectorShuffle<1, 2, 3, 0>(transpose[1]);         //[6,10,14,2]
			counter[1] = VectorShuffle<1, 2, 3, 0>(transpose[3]);         //[8,12,16,4]
			jump = VectorShuffle<0, 1, 0, 1>(transpose[0], transpose[2]); //[1,5,3,7]
			mat2_result[0] = VectorMul(counter[0], transpose[0]);    //[1*6, 5*10, 9*14, 13*2]
			mat2_result[1] = VectorMul(counter[1], transpose[2]);    //[3*8, 7*12, 11*16, 15*4]
			mat2_result[2] = VectorMul(jump, 
				VectorShuffle<2, 3, 2, 3>(transpose[1], transpose[3]));//[1*10,9*14, 3*12, 11*16]						
			counter[0] = VectorShuffle<1, 2, 3, 0>(transpose[0]);         //[5,9,13,1]
			counter[1] = VectorShuffle<1, 2, 3, 0>(transpose[2]);         //[7,11,15,3]
			jump = VectorShuffle<0, 1, 0, 1>(transpose[1], transpose[3]); //[2,6,4,8]
			counter[0] = VectorMul(counter[0], transpose[1]);        //[2*5, 6*9, 10*13, 14*1]
			counter[1] = VectorMul(counter[1], transpose[3]);        //[4*7, 8*11,12*15, 16*3]
			jump = VectorMul(jump, 
				VectorShuffle<2, 3, 2, 3>(transpose[0],transpose[2]));    //[2*9, 6*13, 4*11, 8*15]

			//[1*6-2*5, 5*10-6*9, 9*14-10*13, 13*2-14*1]
			mat2_result[0] = VectorSub(mat2_result[0], counter[0]);
			//[3*8-4*7, 7*12-8*11, 11*16-12*15, 15*4- 16*3]
			mat2_result[1] = VectorSub(mat2_result[1], counter[1]);
			//[1*10-2*9, 5*14-6*13, 3*12-4*11, 7*16-8*15]
			mat2_result[2] = VectorSub(mat2_result[2], jump);
			//[2*9-1*10, 6*13-5*14, 4*11-3*12, 8*15-7*16]
			mat2_result[3] = VectorSub(math_impl::kVectorZero, mat2_result[2]);

			//caculate 1,9,3,11's one and two part
			float128 accumulate[2];
			//[6*11*16, 6*15*4, 16*5*10, 16*1*6] = [6,6,16,16] * [11*16, 15*4, 5*10, 1*6]
			accumulate[0] = VectorShuffle<1, 1, 3, 3>(transpose[1], transpose[3]);
			//[14*7*12, 14*3*8, 8*9*14, 8*13*2] = [14,14, 8, 8] * [7*12, 3*8, 9*14, 13*2]
			accumulate[1] = VectorShuffle<3, 3, 1, 1>(transpose[1], transpose[3]);
			inverse[0] = VectorMul(accumulate[0],
				VectorShuffle<2, 3, 1, 0>(mat2_result[1], mat2_result[0]));
			inverse[0] = VectorMulAdd(accumulate[1],
				VectorShuffle<1, 0, 2, 3>(mat2_result[1], mat2_result[0]), inverse[0]);

			//caculate 5,13,7,15's one and two part
			//-[10*3*16, 10*7*4, 4*13*10, 4*9*6]= [10, 10, 4, 4] * -[3*16, 7*4, 13*10, 9*6]
			accumulate[0] = VectorShuffle<2, 2, 0, 0>(transpose[1], transpose[3]);
			//-[2*15*12, 2*11*8, 12*1*14, 12*5*2]= [2, 2, 12, 12] * -[15*12, 11*8, 1*14, 5*2]
			accumulate[1] = VectorShuffle<0, 0, 2, 2>(transpose[1], transpose[3]);
			inverse[1] = VectorMul(accumulate[0],
				VectorShuffle<3, 0, 2, 1>(mat2_result[1], mat2_result[0]));
			inverse[1] = VectorMulAdd(accumulate[1],
				VectorShuffle<2, 1, 3, 0>(mat2_result[1], mat2_result[0]), inverse[0]);

			//caculate 2, 10, 4, 12's one and two part
			//-[13*11*8, 13*4*7, 7*13*10, 7*1*14]= [13, 13, 7, 7] * -[11*8, 4*7, 13*10, 1*14]
			accumulate[0] = VectorShuffle<3, 3, 1, 1>(transpose[0], transpose[2]);
			//-[5*15*12, 5*3*16, 15*9*6, 15*5*2]= [5, 5, 15, 15] * -[15*12, 3*16, 9*6, 5*2]
			accumulate[1] = VectorShuffle<1, 1, 3, 3>(transpose[0], transpose[2]);
			inverse[2] = VectorMul(accumulate[0],
				VectorShuffle<1, 0, 2, 3>(mat2_result[1], mat2_result[0]));
			inverse[2] = VectorMulAdd(accumulate[1],
				VectorShuffle<2, 3, 1, 0>(mat2_result[1], mat2_result[0]), inverse[0]);

			//caculate 6,14,8,16's one and two part
			//[1*11*16, 1*7*12, 11*2*13, 11*1*6]= [1,1,11, 11] * [11*16, 7*12, 2*13, 1*6]
			accumulate[0] = VectorShuffle<0, 0, 2, 2>(transpose[0], transpose[2]);
			//[9*15*4, 9*3*8, 3*9*14, 3*5*10]= [9,9, 3, 3]* [15*4, 3*8, 9*14, 5*10]
			accumulate[1] = VectorShuffle<2, 2, 0, 0>(transpose[0], transpose[2]);
			inverse[2] = VectorMul(accumulate[0],
				VectorShuffle<2, 1, 3, 0>(mat2_result[1], mat2_result[0]));
			inverse[2] = VectorMulAdd(accumulate[1],
				VectorShuffle<3, 0, 2, 1>(mat2_result[1], mat2_result[0]), inverse[0]);

			//[1*10-2*9, 5*14-6*13, 3*12-4*11, 7*16-8*15]
			mat2_result[2] = VectorSub(mat2_result[2], jump);
			//[2*9-1*10, 6*13-5*14, 4*11-3*12, 8*15-7*16]
			mat2_result[3] = VectorSub(math_impl::kVectorZero, mat2_result[2]);

			//1,5,9,13's third accumulate
			//[10*15*8, 14*11*4, 2*7*16, 6*3*12] = [10, 14, 2, 6]*[15*8, 11*4, 7*16, 3*12]
			float128 row0 = VectorShuffle<2, 3, 0, 1>(transpose[1]);
			row0 = VectorMul(row0, VectorShuffle<3, 2, 3, 2>(mat2_result[3], mat2_result[2]));

			//2,6,10,14's third part
			//[9*7*16, 13*3*12, 1*15*8, 5*11*4] = [9, 13, 1, 5]*[7*16, 3*12, 15*8, 11*4]
			float128 row1 = VectorShuffle<2, 3, 0, 1>(transpose[0]);
			row1 = VectorMul(row1, VectorShuffle<3, 2, 3, 2>(mat2_result[2], mat2_result[3]));

			//3,7,11,15's third part
			//[12*6*13, 16*2*9, 4*5*14, 8*1*10] = [12, 16, 4,8]*[6*13, 2*9, 5*14, 1*10]
			float128 row2 = VectorShuffle<2, 3, 0, 1>(transpose[3]);
			row2 = VectorMul(row2, VectorShuffle<1, 0, 1, 0>(mat2_result[2], mat2_result[3]));

			//4,8,12,16's third part
			//[11*5*14, 15*1*10, 3*13*6, 7*9*2] = [11, 15, 3, 7]*[5*14, 1*10, 13*6, 9*2]
			float128 row3 = VectorShuffle<2, 3, 0, 1>(transpose[2]);
			row3 = VectorMul(row3, VectorShuffle<1, 0, 1, 0>(mat2_result[3], mat2_result[2]));

			//1,9,3,11 final result
			inverse[0] = VectorAdd(VectorShuffle<0, 2, 0, 2>(row0, row2), inverse[0]);
			//5,13,7,15 final result
			inverse[1] = VectorSub(VectorShuffle<1, 3, 1, 3>(row0, row2), inverse[1]);
			//2,10,4,12 final result
			inverse[2] = VectorSub(VectorShuffle<0, 2, 0, 2>(row1, row3), inverse[2]);
			//6,14,8,16 final result
			inverse[3] = VectorAdd(VectorShuffle<1, 3, 1, 3>(row1, row3), inverse[3]);

			//1,5,9,13
			row0 = VectorShuffle<0, 1, 0, 1>(inverse[0], inverse[1]);
			row0 = VectorShuffle<0, 2, 1, 3>(row0);
			//2,6,10,14
			row1 = VectorShuffle<0, 1, 0, 1>(inverse[2], inverse[3]);
			row1 = VectorShuffle<0, 2, 1, 3>(row1);
			//3,7,11,15
			row2 = VectorShuffle<2, 3, 2, 3>(inverse[0], inverse[1]);
			row2 = VectorShuffle<0, 2, 1, 3>(row2);
			//4,8,12,16
			row3 = VectorShuffle<2, 3, 2, 3>(inverse[2], inverse[3]);
			row3 = VectorShuffle<0, 2, 1, 3>(row3);

			float128 determinant = VectorMul(transpose[0], row0);
			float128 inv_determinant = VectorReciprocal(determinant);

			inverse[0] = VectorMul(inv_determinant, row0);
			inverse[1] = VectorMul(inv_determinant, row1);
			inverse[2] = VectorMul(inv_determinant, row2);
			inverse[3] = VectorMul(inv_determinant, row3);
			return determinant;
		}

		//homogeneous transform
		A3D_FORCEINLINE float128 MatrixTransformVector(
			const float128x4& M, const float128& V)
		{
			/*
			*			1  2  3  4
			* a,b,c,d * 5  6  7  8
			*			9  10 11 12
			*			13 14 15 16
			*
			* = (1+2+3+4)a, (5+6+7+8)b, (9+10+11+12)c, (13+14+15+16)d
			*/
			float128 component[4], result;
			component[0] = VectorReplicate<0>(V);
			component[1] = VectorReplicate<1>(V);
			component[2] = VectorReplicate<2>(V);
			component[3] = VectorReplicate<3>(V);

			result = VectorMul(M[0], component[0]);
			result = VectorMulAdd(M[1], component[1], result);
			result = VectorMulAdd(M[2], component[2], result);
			result = VectorMulAdd(M[3], component[3], result);
			return result;
		}
#endif  //Matrix Operation Implements

#endif  //Vector Matrix Operation Implements

	} // namespace math
}//namespace Aurora3D

 /*	static const float128 kVectorAbsMask = VectorLoad(kHighestZero);
 static const float128 kVectorSignMask = VectorLoad(kHighestOne);
 static const float128 kVectorInfinte = VectorLoad(kInfinite);
 static const float128 kVectorZero = VectorLoad(0.0f);
 static const float128 kVectorOne = VectorLoad(1.0f);
 static const float128 kVectorNegtiveOne = VectorLoad(-1.0f);
 static const float128 kVectorHalf = VectorLoad(0.5f);
 static const float128 kVectorTwo = VectorLoad(2.0f);
 static const float128 kVectorAllOneMask = VectorLoad(kAllOneMask);
 static const float128 kVectorOneOver2PI = VectorLoad(kOneOver2PI);
 static const float128 kVector2PI = VectorLoad(k2PI);
 static const float128 kVectorPI = VectorLoad(kPI);
 static const float128 kVectorHalfPI = VectorLoad(kHalfPI);
 static const float128 kVector180OverPI = VectorLoad(k180OverPI);
 static const float128 kVectorPIOver180 = VectorLoad(kPIOver180);
 static const float128 kVectorXYZMask = VectorLoad(kAllOneMask, kAllOneMask, kAllOneMask, kAllZeroMask);
 static const float128 kVectorXOne = VectorLoad(1.0f, 0.0f, 0.0f, 0.0f);
 static const float128 kVectorYOne = VectorLoad(0.0f, 1.0f, 0.0f, 0.0f);
 static const float128 kVectorZOne = VectorLoad(0.0f, 0.0f, 1.0f, 0.0f);
 static const float128 kVectorWOne = VectorLoad(0.0f, 0.0f, 0.0f, 1.0f);
 static const float128 kVectorOddNegtive = VectorLoad(1.0f, -1.0f, 1.0f, -1.0f);
 static const float128 kVectorEvenNegtive = VectorLoad(-1.0f, 1.0f, -1.0f, 1.0f);
 static const float128 kVectorEpside = VectorLoad(kMiddleEpside);*/