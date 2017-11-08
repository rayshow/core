#pragma once

#include<Core/type.h>

namespace Aurora3D
{
	namespace math
	{


		struct float128 {
			union alignas(16)
			{
				float f32[4];
				float fx, fy, fz, fw;
				uint32 u32[4];
				uint32 ux, uy, uz, uw;
			};

			float128() = default;

			float128(float f0, float f1, float f2, float f3) :
				fx(f0), fy(f1), fz(f2), fw(f3)
			{}

			float128(uint32 u0, uint32 u1, uint32 u2, uint32 u3) :
				ux(u0), uy(u1), uz(u2), uw(u3)
			{}

			float& operator[](int i) {
				return f32[i];
			}
			const float& operator[](int i) const {
				return f32[i];
			}

		};

		struct float128x4 {
			float128 V[4];
			float128& operator[](int i) {
				return V[i];
			}
			const float128& operator[](int i) const {
				return V[i];
			}
			float128x4() = default;
		};

		A3D_FORCEINLINE float FloatReciprocalSqrt(float F)
		{
			return 1.0f / sqrtf(F);
		}

		A3D_FORCEINLINE float FloatReciprocalSqrtApproximate(float F)
		{
			return 1.0f / sqrtf(F);
		}

		A3D_FORCEINLINE float FloatSqrt(float F)
		{
			return sqrtf(F);
		}

		A3D_FORCEINLINE float FloatSqrtApproximate(float F)
		{
			return sqrtf(F);
		}

		A3D_FORCEINLINE float128 VectorZero()
		{
			return{ 0.0f, 0.0f, 0.0f, 0.0f };
		}

		A3D_FORCEINLINE float128 VectorLoad(float x, float y,
			float z, float w)
		{
			return{ x, y, z, w };
		}

		A3D_FORCEINLINE float128 VectorLoad(uint32 x, uint32 y, uint32 z, uint32 w)
		{
			return{ x, y, z, w };
		}

		A3D_FORCEINLINE float128 VectorLoad(uint32 U)
		{
			return{ U,U,U,U };
		}

		A3D_FORCEINLINE float128 VectorLoad(float F)
		{
			return{ F, F, F, F };
		}

		A3D_FORCEINLINE float128 VectorLoad4(const float *v)
		{
			return{ v[0], v[1], v[2], v[3] };
		}

		A3D_FORCEINLINE float128 VectorLoad4Aligned(const float *v)
		{
			return{ v[0], v[1], v[2], v[3] };
		}

		A3D_FORCEINLINE float128 VectorLoad3W0(const float *v)
		{
			return{ v[0], v[1], v[2], 0.0 };
		}

		A3D_FORCEINLINE float128 VectorLoad3W1(const float *v)
		{
			return{ v[0], v[1], v[2], 1.0f };
		}

		A3D_FORCEINLINE void VectorSet(float128& v, int i, float F)
		{
			float128 cp = v;
			cp.f32[i] = F;
			return cp;
		}

		A3D_FORCEINLINE float VectorGetFloat(const float128& v, int i)
		{
			return v.f32[i];
		}

		A3D_FORCEINLINE float VectorGetFirst(const float128& v)
		{
			return v.fx;
		}

		A3D_FORCEINLINE uint32 VectorGetUint(const float128& v, int i)
		{
			return v.u32[i];
		}

		A3D_FORCEINLINE void VectorSetW0(float128& v)
		{
			return VectorSet(v, 3, 0.0f);
		}

		A3D_FORCEINLINE void VectorSetW1(float128& v)
		{
			return VectorSet(v, 3, 1.0f);
		}

		A3D_FORCEINLINE void VectorStore4(const float128& v, float *m)
		{
			memcpy(m, v.f32, sizeof(float) * 4);
		}

		A3D_FORCEINLINE void VectorStore4Aligned(const float128& v, float *m)
		{
			memcpy(m, v.f32, sizeof(float) * 4);
		}

		A3D_FORCEINLINE void VectorStore4(const float128& v, uint32 *m)
		{
			memcpy(m, v.u32, sizeof(uint32) * 4);
		}

		A3D_FORCEINLINE void VectorStore3(const float128& v, float* m)
		{
			memcpy(m, v.f32, sizeof(float) * 3);
		}

		A3D_FORCEINLINE void VectorStore1(const float128& v, float *m)
		{
			*m = v.f32[0];
		}

		A3D_FORCEINLINE void VectorStore1(const float128& v, int i, float *m)
		{
			*m = v.f32[i];
		}

		A3D_FORCEINLINE void VectorStore4Aligned(const float128& v, float *m)
		{
			memcpy(m, v.f32, sizeof(float) * 4);
		}

		A3D_FORCEINLINE void VectorStore4AlignedNoCache(
			const float128& v, float *m)
		{
			memcpy(m, v.f32, sizeof(float) * 4);
		}

		A3D_FORCEINLINE float128 VectorReplicate(const float128& v, int i)
		{
			float F = v.f32[i];
			return{ F, F, F, F };
		}

		A3D_FORCEINLINE float128 VectorShuffle(const float128& v, int i0, int i1, int i2, int i3)
		{
			return{ v.f32[i0], v.f32[i1], v.f32[i2], v.f32[i3] };
		}

		A3D_FORCEINLINE float128 VectorShuffle(const float128& v1,
			const float128& v2, int i0, int i1, int i2, int i3)
		{
			return{ v1.f32[i0], v1.f32[i1], v2.f32[i2], v2.f32[i3] };
		}

		//latency 3
		A3D_FORCEINLINE float128 VectorAdd(const float128& v1, const float128& v2)
		{
			return{ v1.fx + v2.fx, v1.fy + v2.fy, v1.fz + v2.fz, v1.fw + v2.fw };
		}

		A3D_FORCEINLINE float128 VectorMinus(const float128& v1, const float128& v2)
		{
			return{ v1.fx - v2.fx, v1.fy - v2.fy, v1.fz - v2.fz, v1.fw - v2.fw };
		}

		A3D_FORCEINLINE float128 VectorMultiply(const float128& v1, const float128& v2)
		{
			return{ v1.fx * v2.fx, v1.fy * v2.fy, v1.fz * v2.fz, v1.fw * v2.fw };
		}

		A3D_FORCEINLINE float128 VectorMultiply(const float128& v1, float scale)
		{
			return{ v1.fx * scale, v1.fy * scale, v1.fz * scale, v1.fw * scale };
		}

		A3D_FORCEINLINE float128 VectorMultiplyAdd(const float128& m1,
			const float128& m2, const float128& add)
		{
			return{ m1.fx * m2.fx + add.fx, m1.fy * m2.fy + add.fy,
				m1.fz * m2.fz + add.fz, m1.fw * m2.fw + add.fw };
		}

		A3D_FORCEINLINE float128 VectorMultiplyMinus(const float128& m1,
			const float128& m2, const float128& minus)
		{
			return{ m1.fx * m2.fx - minus.fx, m1.fy * m2.fy - minus.fy,
				m1.fz * m2.fz - minus.fz, m1.fw * m2.fw - minus.fw };
		}

		A3D_FORCEINLINE float128 VectorDivide(const float128& v1, const float128& v2)
		{
			return{ v1.fx / v2.fx, v1.fy / v2.fy, v1.fz / v2.fz, v1.fw / v2.fw };
		}

		A3D_FORCEINLINE float128 VectorEquals(const float128& v1, const float128& v2)
		{
			return{ (uint32)(v1.fx == v2.fx), (uint32)(v1.fy == v2.fy),
				(uint32)(v1.fz == v2.fz), (uint32)(v1.fw == v2.fw) };
		}


		A3D_FORCEINLINE float128 VectorNotEquals(const float128& v1, const float128& v2)
		{
			return{ (uint32)(v1.fx != v2.fx), (uint32)(v1.fy != v2.fy),
				(uint32)(v1.fz != v2.fz), (uint32)(v1.fw != v2.fw) };
		}

		A3D_FORCEINLINE float128 VectorGreater(const float128& v1, const float128& v2)
		{
			return{ (uint32)(v1.fx > v2.fx), (uint32)(v1.fy > v2.fy),
				(uint32)(v1.fz > v2.fz), (uint32)(v1.fw > v2.fw) };
		}

		A3D_FORCEINLINE float128 VectorLess(const float128& v1, const float128& v2)
		{
			return{ (uint32)(v1.fx < v2.fx), (uint32)(v1.fy < v2.fy),
				(uint32)(v1.fz < v2.fz), (uint32)(v1.fw < v2.fw) };
		}

		A3D_FORCEINLINE float128 VectorGreaterOrEqual(const float128& v1, const float128& v2)
		{
			return{ (uint32)(v1.fx >= v2.fx), (uint32)(v1.fy >= v2.fy),
				(uint32)(v1.fz >= v2.fz), (uint32)(v1.fw >= v2.fw) };
		}

		A3D_FORCEINLINE float128 VectorLessOrEqual(const float128& v1, const float128& v2)
		{
			return{ (uint32)(v1.fx <= v2.fx), (uint32)(v1.fy <= v2.fy),
				(uint32)(v1.fz <= v2.fz), (uint32)(v1.fw <= v2.fw) };
		}

		A3D_FORCEINLINE float128 VectorOr(const float128& v1, const float128& v2)
		{
			return{ v1.ux | v2.ux, v1.uy | v2.uy, v1.uz | v2.uz, v1.uw | v2.uw };
		}

		A3D_FORCEINLINE float128 VectorAnd(const float128& v1, const float128& v2)
		{
			return{ v1.ux & v2.ux, v1.uy & v2.uy, v1.uz & v2.uz, v1.uw & v2.uw };
		}

		A3D_FORCEINLINE float128 VectorAndNot(const float128& v1, const float128& v2)
		{
			return{ ~(v1.ux & v2.ux), ~(v1.uy & v2.uy), ~(v1.uz & v2.uz), ~(v1.uw & v2.uw) };
		}

		A3D_FORCEINLINE float128 VectorXor(const float128& v1, const float128& v2)
		{
			return{ v1.ux ^ v2.ux, v1.uy ^ v2.uy, v1.uz ^ v2.uz, v1.uw ^ v2.uw };
		}

		A3D_FORCEINLINE float128 VectorMax(const float128& v1, const float128& v2)
		{
			return{ FloatMax(v1.fx, v2.fx), FloatMax(v1.fy, v2.fy),
				FloatMax(v1.fz, v2.fz), FloatMax(v1.fw, v2.fw) };
		}

		A3D_FORCEINLINE float128 VectorMin(const float128& v1, const float128& v2)
		{
			return{ FloatMin(v1.fx, v2.fx), FloatMin(v1.fy, v2.fy),
				FloatMin(v1.fz, v2.fz), FloatMin(v1.fw, v2.fw) };
		}

		//all is bigger than 0
		A3D_FORCEINLINE bool VectorTrue(const float128& v)
		{
			return 1 == v.ux & v.uy & v.uz & v.uw;
		}

		//all is zero
		A3D_FORCEINLINE bool VectorFalse(const float128& v)
		{
			return 0 == v.ux | v.uy | v.uz | v.uw;
		}

		//some is bigger than 0, and some is 0
		A3D_FORCEINLINE bool VectorAnyTrue(const float128& v)
		{
			return 0 != v.ux | v.uy | v.uz | v.uw;
		}

		A3D_FORCEINLINE float128 VectorDot4(const float128& v1, const float128& v2)
		{
			float F = v1.fx*v2.fx + v1.fy*v2.fy + v1.fz*v2.fz + v1.fw*v2.fw;
			return{ F,F,F,F };
		}

		A3D_FORCEINLINE float128 VectorSumUp(const float128& v)
		{
			float F = v.fx + v.fy + v.fz + v.fw;
			return{ F,F,F,F };
		}

		A3D_FORCEINLINE float128 VectorSelect(const float128& v1,
			const float128& v2, const float128& mask)
		{
			return{
				v2.ux ^ (mask.ux&(v1.ux^v2.ux)),
				v2.uy ^ (mask.uy&(v1.uy^v2.uy)),
				v2.uz ^ (mask.uz&(v1.uz^v2.uz)),
				v2.uw ^ (mask.uw&(v1.uw^v2.uw)),
			};
		}

		A3D_FORCEINLINE float128 VectorIntPart(const float128& v)
		{
			return{ FloatIntPart(v.fx), FloatIntPart(v.fy), FloatIntPart(v.fz), FloatIntPart(v.fw) };
		}

		A3D_FORCEINLINE float128 VectorRound(const float128& v)
		{
			return{ FloatRound(v.fx),FloatRound(v.fy),FloatRound(v.fz),FloatRound(v.fw) };
		}


		A3D_FORCEINLINE float128 VectorAbs(const float128& v)
		{
			return{ FloatAbs(v.fx),FloatAbs(v.fy),FloatAbs(v.fz),FloatAbs(v.fw) };
		}

		A3D_FORCEINLINE float128 VectorReciprocalApproximate(const float128& v)
		{
			return{ 1.0 / v.fx, 1.0f / v.fy, 1.0f / v.fz, 1.0f / v.fw };
		}

		A3D_FORCEINLINE float128 VectorReciprocal(const float128& v)
		{
			return{ 1.0 / v.fx, 1.0f / v.fy, 1.0f / v.fz, 1.0f / v.fw };
		}

		A3D_FORCEINLINE float128 VectorSqrt(const float128& v)
		{
			return{ FloatSqrt(v.fx),FloatSqrt(v.fy),FloatSqrt(v.fz),FloatSqrt(v.fw) };
		}

		A3D_FORCEINLINE float128 VectorSqrtApprixmate(const float128& v)
		{
			return{ FloatSqrt(v.fx),FloatSqrt(v.fy),FloatSqrt(v.fz),FloatSqrt(v.fw) };
		}

		A3D_FORCEINLINE float128 VectorReciprocalSqrtApproximate(const float128& v)
		{
			return{ 1.0f / FloatSqrt(v.fx),1.0f / FloatSqrt(v.fy),1.0f / FloatSqrt(v.fz),1.0f / FloatSqrt(v.fw) };
		}

		A3D_FORCEINLINE float128 VectorReciprocalSqrt(const float128& v)
		{
			return{ 1.0f / FloatSqrt(v.fx),1.0f / FloatSqrt(v.fy),1.0f / FloatSqrt(v.fz),1.0f / FloatSqrt(v.fw) };
		}

		A3D_FORCEINLINE float128 VectorSetW0(const float128& v)
		{
			return{ v.fx, v.fy,v.fz,0.0f };
		}

		A3D_FORCEINLINE float128 VectorSetW1(const float128& v)
		{
			return{ v.fx, v.fy,v.fz,1.0f };
		}

		A3D_FORCEINLINE float128 VectorNegate(const float128& v)
		{
			return{ -v.fx, -v.fy,-v.fz,-v.fw };
		}

		A3D_FORCEINLINE bool VectorNearlyEquals(const float128& v1, const float128& v2,
			float epside = MathConstant::kMiddleEpside)
		{
			return FloatNearlyEquals(v1.fx, v2.fx, epside) &&
				FloatNearlyEquals(v1.fy, v2.fy, epside) &&
				FloatNearlyEquals(v1.fz, v2.fz, epside) &&
				FloatNearlyEquals(v1.fw, v2.fw, epside);
		}

		//x1*x2 + y1*y2 + z1*z2
		A3D_FORCEINLINE float128 VectorDot3(const float128& v1, const float128& v2)
		{
			float F = v1.fx*v2.fx + v1.fy*v2.fy + v1.fz*v2.fz;
			return{ F,F,F,F };
		}

		A3D_FORCEINLINE float128 VectorSign(const float128& v)
		{
			return{ v.ux&MathConstant::kHighestOne,
				v.uy&MathConstant::kHighestOne,
				v.uz&MathConstant::kHighestOne,
				v.uw&MathConstant::kHighestOne
			};
		}

		A3D_FORCEINLINE float128 VectorNormalize(const float128& v)
		{
			const float128 len_sq = VectorDot4(v, v);
			return VectorReciprocalSqrt(len_sq);
		}

		A3D_FORCEINLINE float128 VectorStep(const float128& v)
		{
			return{ FloatStep(v.fx),FloatStep(v.fy),FloatStep(v.fz),FloatStep(v.fw) };
		}

		A3D_FORCEINLINE float128 VectorFracPart(const float128& v)
		{
			return{ FloatFracPart(v.fx),FloatFracPart(v.fy),
				FloatFracPart(v.fz),FloatFracPart(v.fw) };
		}

		A3D_FORCEINLINE float128 VectorCeil(const float128& v)
		{
			return{ FloatCeil(v.fx),FloatCeil(v.fy),
				FloatCeil(v.fz),FloatCeil(v.fw) };
		}

		A3D_FORCEINLINE float128 VectorFloor(const float128& v)
		{
			return{ FloatFloor(v.fx),FloatFloor(v.fy),
				FloatFloor(v.fz),FloatFloor(v.fw) };
		}

		A3D_FORCEINLINE bool VectorInBound(const float128& v,
			const float128& bound)
		{
			return FloatInBound(v.fx, bound.fx) &&
				FloatInBound(v.fy, bound.fy) &&
				FloatInBound(v.fz, bound.fz) &&
				FloatInBound(v.fw, bound.fw);
		}

		A3D_FORCEINLINE bool VectorIsNaN(const float128& v)
		{
			return FloatIsNaN(v.fx) ||
				FloatIsNaN(v.fy) ||
				FloatIsNaN(v.fz) ||
				FloatIsNaN(v.fw);
		}

		A3D_FORCEINLINE bool VectorIsInfinite(const float128& v)
		{
			return FloatIsInfinite(v.fx) ||
				FloatIsInfinite(v.fy) ||
				FloatIsInfinite(v.fz) ||
				FloatIsInfinite(v.fw);
		}

		A3D_FORCEINLINE float128 VectorClamp(const float128& v,
			const float128& min, const float128& max)
		{
			return{ FloatClamp(v.fx, min.fx, max.fx),
				FloatClamp(v.fy, min.fy, max.fy),
				FloatClamp(v.fz, min.fz, max.fz),
				FloatClamp(v.fw, min.fw, max.fw)
			};
		}

		//clamp to [0,1]
		A3D_FORCEINLINE float128 VectorSaturate(const float128& v)
		{
			return{ FloatSaturate(v.fx),FloatSaturate(v.fy),FloatSaturate(v.fz),FloatSaturate(v.fw) };
		}

		A3D_FORCEINLINE float128 VectorCrossProduct(const float128& v1, const float128& v2)
		{
			return{
				v1.fy*v2.fz - v2.fz*v1.fy,
				v1.fz*v2.fx - v2.fz*v1.fx,
				v1.fx*v2.fy - v2.fx*v1.fy,
				0.0f
			};
		}

		A3D_FORCEINLINE float128 VectorSinRadianFast(const float128& v)
		{
			return{ FloatSin(v.fx),FloatSin(v.fy), FloatSin(v.fz), FloatSin(v.fw) };
		}

		A3D_FORCEINLINE float128 VectorRadianToDegree(const float128& v)
		{
			return{
				FloatRadianToDegree(v.fx),
				FloatRadianToDegree(v.fy),
				FloatRadianToDegree(v.fz),
				FloatRadianToDegree(v.fw)
			};
		}

		A3D_FORCEINLINE float128 VectorDegreeToRadian(const float128& v)
		{
			return{
				FloatDegreeToRadian(v.fx),
				FloatDegreeToRadian(v.fy),
				FloatDegreeToRadian(v.fz),
				FloatDegreeToRadian(v.fw)
			};
		}

		A3D_FORCEINLINE float128 VectorSinDegreeFast(const float128& v)
		{
			return{
				FloatSin(FloatDegreeToRadian(v.fx)),
				FloatSin(FloatDegreeToRadian(v.fy)),
				FloatSin(FloatDegreeToRadian(v.fz)),
				FloatSin(FloatDegreeToRadian(v.fw))
			};
		}

		A3D_FORCEINLINE float128 VectorCosRadianFast(const float128& v)
		{
			return{ FloatCos(v.fx),FloatCos(v.fy), FloatCos(v.fz), FloatCos(v.fw) };
		}

		A3D_FORCEINLINE float128 VectorCosDegreeFast(const float128& v)
		{
			return{
				FloatCos(FloatDegreeToRadian(v.fx)),
				FloatCos(FloatDegreeToRadian(v.fy)),
				FloatCos(FloatDegreeToRadian(v.fz)),
				FloatCos(FloatDegreeToRadian(v.fw))
			};
		}

		A3D_FORCEINLINE void VectorSinAndCosFast(float128& sin, float128& cos, const float128& radians)
		{
			sin = VectorSinRadianFast(radians);
			cos = VectorCosRadianFast(radians);
		}

		//TODO opt vectoralize
		A3D_FORCEINLINE float128 VectorPow(const float128& base,
			const float128& exp)
		{
			return{
				FloatPow(base.fx, exp.fx),
				FloatPow(base.fy, exp.fx),
				FloatPow(base.fz, exp.fz),
				FloatPow(base.fw, exp.fw)
			};
		}

		A3D_FORCEINLINE float128 VectorExp(const float128& v)
		{
			return{
				FloatExp(v.fx),
				FloatExp(v.fy),
				FloatExp(v.fz),
				FloatExp(v.fw)
			};
		}

		A3D_FORCEINLINE float128 VectorExp2(const float128& v)
		{
			return{
				FloatExp2(v.fx),
				FloatExp2(v.fy),
				FloatExp2(v.fz),
				FloatExp2(v.fw)
			};
		}

		A3D_FORCEINLINE float128 VectorLogE(const float128& v)
		{
			return{
				FloatLogE(v.fx),
				FloatLogE(v.fy),
				FloatLogE(v.fz),
				FloatLogE(v.fw)
			};
		}

		//TODO opt vectoralize
		A3D_FORCEINLINE float128 VectorLog2(const float128& v)
		{
			return{
				FloatLog2(v.fx),
				FloatLog2(v.fy),
				FloatLog2(v.fz),
				FloatLog2(v.fw)
			};
		}

		// must match every elements
		A3D_FORCEINLINE bool MatrixIsIdentity(const float128x4& M)
		{
			static const float128 kVectorXOne = VectorLoad(1.0f, 0.0f, 0.0f, 0.0f);
			static const float128 kVectorYOne = VectorLoad(0.0f, 1.0f, 0.0f, 0.0f);
			static const float128 kVectorZOne = VectorLoad(0.0f, 0.0f, 1.0f, 0.0f);
			static const float128 kVectorWOne = VectorLoad(0.0f, 0.0f, 0.0f, 1.0f);
			return VectorTrue(VectorEquals(M[0], kVectorXOne))
				&& VectorTrue(VectorEquals(M[1], kVectorXOne))
				&& VectorTrue(VectorEquals(M[2], kVectorXOne))
				&& VectorTrue(VectorEquals(M[3], kVectorXOne));
		}

		// once one element is NaN, matrix is NaN,
		A3D_FORCEINLINE bool MatrixIsNaN(const float128x4& M)
		{
			return VectorIsNaN(M[0]) ||
				VectorIsNaN(M[1]) ||
				VectorIsNaN(M[2]) ||
				VectorIsNaN(M[3]);
		}

		A3D_FORCEINLINE bool MatrixIsInInfinite(const float128x4& M)
		{
			return VectorIsInfinite(M[0]) ||
				VectorIsInfinite(M[1]) ||
				VectorIsInfinite(M[2]) ||
				VectorIsInfinite(M[3]);
		}

		//R != A, R!=B ca't over-write
		A3D_FORCEINLINE void MatrixMultiplyNoCopy(const float128x4& A,
			const float128x4& B, float128x4& R)
		{
			R[0][0] = A[0][0] * B[0][0] + A[0][1] * B[1][0] + A[0][2] * B[2][0] + A[0][3] * B[3][0];
			R[0][1] = A[0][0] * B[0][1] + A[0][1] * B[1][1] + A[0][2] * B[2][1] + A[0][3] * B[3][1];
			R[0][2] = A[0][0] * B[0][2] + A[0][1] * B[1][2] + A[0][2] * B[2][2] + A[0][3] * B[3][2];
			R[0][3] = A[0][0] * B[0][3] + A[0][1] * B[1][3] + A[0][2] * B[2][3] + A[0][3] * B[3][3];

			R[1][0] = A[1][0] * B[0][0] + A[1][1] * B[1][0] + A[1][2] * B[2][0] + A[1][3] * B[3][0];
			R[1][1] = A[1][0] * B[0][1] + A[1][1] * B[1][1] + A[1][2] * B[2][1] + A[1][3] * B[3][1];
			R[1][2] = A[1][0] * B[0][2] + A[1][1] * B[1][2] + A[1][2] * B[2][2] + A[1][3] * B[3][2];
			R[1][3] = A[1][0] * B[0][3] + A[1][1] * B[1][3] + A[1][2] * B[2][3] + A[1][3] * B[3][3];

			R[2][0] = A[2][0] * B[0][0] + A[2][1] * B[1][0] + A[2][2] * B[2][0] + A[2][3] * B[3][0];
			R[2][1] = A[2][0] * B[0][1] + A[2][1] * B[1][1] + A[2][2] * B[2][1] + A[2][3] * B[3][1];
			R[2][2] = A[2][0] * B[0][2] + A[2][1] * B[1][2] + A[2][2] * B[2][2] + A[2][3] * B[3][2];
			R[2][3] = A[2][0] * B[0][3] + A[2][1] * B[1][3] + A[2][2] * B[2][3] + A[2][3] * B[3][3];

			R[3][0] = A[3][0] * B[0][0] + A[3][1] * B[1][0] + A[3][2] * B[2][0] + A[3][3] * B[3][0];
			R[3][1] = A[3][0] * B[0][1] + A[3][1] * B[1][1] + A[3][2] * B[2][1] + A[3][3] * B[3][1];
			R[3][2] = A[3][0] * B[0][2] + A[3][1] * B[1][2] + A[3][2] * B[2][2] + A[3][3] * B[3][2];
			R[3][3] = A[3][0] * B[0][3] + A[3][1] * B[1][3] + A[3][2] * B[2][3] + A[3][3] * B[3][3];
		}

		//can assign to self 
		A3D_FORCEINLINE float128x4 MatrixMultiply(const float128x4& A, const float128x4& B)
		{
			float128x4 R;
			MatrixMultiplyNoCopy(A, B, R);
			return R;
		}

		A3D_FORCEINLINE float128 MatrixDeterminant(const float128x4& M)
		{
			float cross[4]; //means like 1*6-2*5 style
			cross[0] = M[2][0] * M[3][1] - M[2][1] * M[3][0];
			cross[1] = M[2][1] * M[3][2] - M[2][2] * M[3][1];
			cross[2] = M[2][2] * M[3][3] - M[2][3] * M[3][2];
			cross[3] = M[2][3] * M[3][0] - M[2][0] * M[3][3];

			float jump_cross[2]; //means 9*15-13*11 style
			jump_cross[0] = M[2][0] * M[3][2] - M[2][2] * M[3][0];
			jump_cross[1] = M[2][1] * M[3][3] - M[2][3] * M[3][1];

			float part[4];
			part[0] = M[0][0] * (M[1][1] * cross[2] - M[1][2] * jump_cross[1] + M[1][3] * cross[1]);
			part[1] = -M[0][1] * (M[1][0] * cross[2] + M[1][2] * cross[3] + M[1][3] * jump_cross[0]);
			part[2] = M[0][2] * (M[1][0] * jump_cross[0] + M[1][1] * cross[3] + M[1][3] * cross[0]);
			part[3] = -M[0][3] * (M[1][0] * cross[1] - M[1][1] * jump_cross[0] + cross[0]);
			const float D = part[0] + part[1] + part[2] + part[3];

			return{ D,D,D,D };
		}

		A3D_FORCEINLINE void MatrixTransposeNoCopy(const float128x4& O, float128x4& T)
		{
			T[0][0] = O[0][0], T[0][1] = O[1][0], T[0][2] = O[2][0], T[0][3] = O[3][0];
			T[1][0] = O[0][1], T[1][1] = O[1][1], T[1][2] = O[2][1], T[1][3] = O[3][1];
			T[2][0] = O[0][2], T[2][1] = O[1][2], T[2][2] = O[2][2], T[2][3] = O[3][2];
			T[3][0] = O[0][3], T[3][1] = O[1][3], T[3][2] = O[2][3], T[3][3] = O[3][3];
		}

		A3D_FORCEINLINE float128x4 MatrixTranspose(const float128x4& O)
		{
			float128x4 T;
			MatrixTransposeNoCopy(O, T);
			return T;
		}

		A3D_FORCEINLINE float128 MatrixInverse(const float128x4& M, float128x4& I)
		{
			float mat2[3][3];
			mat2[0][0] = M[2][2] * M[3][3] - M[2][3] * M[3][2];
			mat2[0][1] = M[1][2] * M[3][3] - M[1][3] * M[3][2];
			mat2[0][2] = M[1][2] * M[2][3] - M[1][3] * M[2][2];

			mat2[1][0] = M[2][2] * M[3][3] - M[2][3] * M[3][2];
			mat2[1][1] = M[0][2] * M[3][3] - M[0][3] * M[3][2];
			mat2[1][2] = M[0][2] * M[2][3] - M[0][3] * M[2][2];

			mat2[2][0] = M[1][2] * M[3][3] - M[1][3] * M[3][2];
			mat2[2][1] = M[0][2] * M[3][3] - M[0][3] * M[3][2];
			mat2[2][2] = M[0][2] * M[1][3] - M[0][3] * M[1][2];

			mat2[3][0] = M[1][2] * M[2][3] - M[1][3] * M[2][2];
			mat2[3][1] = M[0][2] * M[2][3] - M[0][3] * M[2][2];
			mat2[3][2] = M[0][2] * M[1][3] - M[0][3] * M[1][2];

			float part[4];
			part[0] = M[1][1] * mat2[0][0] - M[2][1] * mat2[0][1] + M[3][1] * mat2[0][2];
			part[1] = M[0][1] * mat2[1][0] - M[2][1] * mat2[1][1] + M[3][1] * mat2[1][2];
			part[2] = M[0][1] * mat2[2][0] - M[1][1] * mat2[2][1] + M[3][1] * mat2[2][2];
			part[3] = M[0][1] * mat2[3][0] - M[1][1] * mat2[3][1] + M[2][1] * mat2[3][2];

			const float D = M[0][0] * part[0] - M[1][0] * part[1] + M[2][0] * part[2] - M[3][0] * part[3];
			const float	recip_det = 1.0f / D;

			I[0][0] = recip_det * part[0];
			I[0][1] = -recip_det * part[1];
			I[0][2] = recip_det * part[2];
			I[0][3] = -recip_det * part[3];
			I[1][0] = -recip_det * (M[1][0] * mat2[0][0] - M[2][0] * mat2[0][1] + M[3][0] * mat2[0][2]);
			I[1][1] = recip_det * (M[0][0] * mat2[1][0] - M[2][0] * mat2[1][1] + M[3][0] * mat2[1][2]);
			I[1][2] = -recip_det * (M[0][0] * mat2[2][0] - M[1][0] * mat2[2][1] + M[3][0] * mat2[2][2]);
			I[1][3] = recip_det * (M[0][0] * mat2[3][0] - M[1][0] * mat2[3][1] + M[2][0] * mat2[3][2]);
			I[2][0] = recip_det * (
				M[1][0] * (M[2][1] * M[3][3] - M[2][3] * M[3][1]) -
				M[2][0] * (M[1][1] * M[3][3] - M[1][3] * M[3][1]) +
				M[3][0] * (M[1][1] * M[2][3] - M[1][3] * M[2][1])
				);
			I[2][1] = -recip_det * (
				M[0][0] * (M[2][1] * M[3][3] - M[2][3] * M[3][1]) -
				M[2][0] * (M[0][1] * M[3][3] - M[0][3] * M[3][1]) +
				M[3][0] * (M[0][1] * M[2][3] - M[0][3] * M[2][1])
				);
			I[2][2] = recip_det * (
				M[0][0] * (M[1][1] * M[3][3] - M[1][3] * M[3][1]) -
				M[1][0] * (M[0][1] * M[3][3] - M[0][3] * M[3][1]) +
				M[3][0] * (M[0][1] * M[1][3] - M[0][3] * M[1][1])
				);
			I[2][3] = -recip_det * (
				M[0][0] * (M[1][1] * M[2][3] - M[1][3] * M[2][1]) -
				M[1][0] * (M[0][1] * M[2][3] - M[0][3] * M[2][1]) +
				M[2][0] * (M[0][1] * M[1][3] - M[0][3] * M[1][1])
				);
			I[3][0] = -recip_det * (
				M[1][0] * (M[2][1] * M[3][2] - M[2][2] * M[3][1]) -
				M[2][0] * (M[1][1] * M[3][2] - M[1][2] * M[3][1]) +
				M[3][0] * (M[1][1] * M[2][2] - M[1][2] * M[2][1])
				);
			I[3][1] = recip_det * (
				M[0][0] * (M[2][1] * M[3][2] - M[2][2] * M[3][1]) -
				M[2][0] * (M[0][1] * M[3][2] - M[0][2] * M[3][1]) +
				M[3][0] * (M[0][1] * M[2][2] - M[0][2] * M[2][1])
				);
			I[3][2] = -recip_det * (
				M[0][0] * (M[1][1] * M[3][2] - M[1][2] * M[3][1]) -
				M[1][0] * (M[0][1] * M[3][2] - M[0][2] * M[3][1]) +
				M[3][0] * (M[0][1] * M[1][2] - M[0][2] * M[1][1])
				);
			I[3][3] = recip_det * (
				M[0][0] * (M[1][1] * M[2][2] - M[1][2] * M[2][1]) -
				M[1][0] * (M[0][1] * M[2][2] - M[0][2] * M[2][1]) +
				M[2][0] * (M[0][1] * M[1][2] - M[0][2] * M[1][1])
				);
			return{ D, D, D, D };
		}

		A3D_FORCEINLINE float128 MatrixTransformVector(const float128x4& M, const float128& V)
		{
			float x = M[0][0] * V[0] + M[1][0] * V[1] + M[2][0] * V[2] + M[3][0] * V[3];
			float y = M[0][1] * V[0] + M[1][1] * V[1] + M[2][1] * V[2] + M[3][1] * V[3];
			float z = M[0][2] * V[0] + M[1][2] * V[1] + M[2][2] * V[2] + M[3][2] * V[3];
			float w = M[0][3] * V[0] + M[1][3] * V[1] + M[2][3] * V[2] + M[3][3] * V[3];
			return{ x,y,z,w };
		}

	}
}