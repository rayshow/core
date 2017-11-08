#pragma once

#include<cassert>
#include<core/math/vector4.h>


using namespace Aurora3D::math;


inline void TestVector()
{

	//================ basic operation =====================
	constexpr Vector4 t1(2.4f, 2.5f, 2.6f, 2.7f), t2(2.0f, 2.0f, 2.0f, 2.0f);
	cout << " test union1:" << t1.ToString() << " " << t1.ToStringAsInt() << " " << t1.vec.m128_f32[0] << " " << t1.vec.m128_f32[1] << " " << t1.vec.m128_f32[2] << " " << t1.vec.m128_f32[3] << endl;
	Vector4 zero{};
	Vector4 allOneMask{ kfNegativeInf,0,0,0 };
	cout << " test union2:" << zero.ToString() << " " << zero.ToStringAsInt() << endl;
	cout << " test union3:" << allOneMask.ToString() << " " << allOneMask.ToStringAsInt() << endl;

	constexpr Vector4 const_cp(t1);
	Vector4 simd(t1.vec);
	cout << " simd128 construct:" << const_cp.ToString() << " " << simd.ToString() << " " << simd.ToStringAsInt() << endl;
	cout << endl;

	cout << "#########" << "test data t1" << t1.ToString() << " t2 " << t2.ToString() << "#########" << endl;
	Vector4 plus = t1 + t2;
	Vector4 assign_plus = t1.Copy().AssignAdd(t2);
	cout << " t1+t2   :" << plus.ToString() << "  " << assign_plus.ToString() << endl;

	Vector4 minus = t1 - t2;
	Vector4 assign_minus = t1.Copy().AssignSub(t2);
	cout << " t1-t2   :" << minus.ToString() << " " << assign_minus.ToString() << endl;

	Vector4 mul = t1 * t2;
	Vector4 assign_mul = t1.Copy().AssignMul(t2);
	cout << " t1*t2   :" << mul.ToString() << " " << assign_mul.ToString() << endl;

	Vector4 div = t1 / t2;
	Vector4 assign_div = t1.Copy().AssignDiv(t2);
	cout << " t1/t2   :" << div.ToString() << " " << assign_div.ToString() << endl;

	Vector4 mod = t1 % t2;
	Vector4 assign_mod = t1.Copy().AssignMod(t2);
	cout << " t1%t2   :" << mod.ToString() << " " << assign_mod.ToString() << endl;

	Vector4 intPart = t1.IntPart();
	Vector4 assign_int = t1.Copy().AssignIntPart();
	cout << " t1 int  :" << intPart.ToString() << " " << assign_int.ToString() << endl;

	Vector4 fracPart = t1.FracPart();
	Vector4 assign_fract = t1.Copy().AssignFracPart();
	cout << " t1 frac :" << fracPart.ToString() << " " << assign_fract.ToString() << endl;
	cout << endl;

	Vector4 t3{ 1, 2, 4, 8 }, t4{ 2, 2, 2, 2 };
	cout << "#########" << "test data t3" << t3.ToStringAsInt() << " t4 " << t4.ToStringAsInt() << " " << "#########" << endl;
	Vector4 bitand = t3 & t4;
	Vector4 assign_bitand = t3.Copy().AssignBitAnd(t4);
	cout << " t3 & t4 :" << bitand.ToStringAsInt() << " " << assign_bitand.ToStringAsInt() << endl;

	Vector4 bitor = t3 | t4;
	Vector4 assign_bitor = t3.Copy().AssignBitOr(t4);
	cout << " t3 | t4 :" << bitor.ToStringAsInt() << " " << assign_bitor.ToStringAsInt() << endl;

	Vector4 bitxor = t3 ^ t4;
	Vector4 assign_bitxor = t3.Copy().AssignBitXor(t4);
	cout << " t3 ^ t4 :" << bitxor.ToStringAsInt() << " " << assign_bitxor.ToStringAsInt() << endl;

	Vector4 bitnot = ~t3;
	Vector4 assign_bitnot = t3.Copy().AssignBitNot();
	cout << " ~ t3    :" << bitnot.ToStringAsInt() << " " << assign_bitnot.ToStringAsInt() << endl;

	Vector4 min = t3.Min(t4);
	Vector4 max = t4.Max(t3);
	cout << " min t3, t4:" << min.ToString() << endl;
	cout << " max t3, t4:" << max.ToString() << endl;

	Vector4 less = t3 < t4;
	cout << " t3<t4   :" << less.ToStringAsInt() << endl;
	cout << "    any4 false: " << less.AnyFalse4() << endl;
	cout << "    any4 true:  " << less.AnyTrue4() << endl;
	cout << "    1,2,3 false:" << less.False3<1, 2, 3>() << endl;
	cout << "    0,2,3 false:" << less.False3<0, 2, 3>() << endl;
	cout << "    0 true:" << less.True1<0>() << endl;
	cout << "    1 true:" << less.True1<1>() << endl;
	cout << "    3 true:" << less.True1<3>() << endl;
	Vector4 lessEqual = t3 <= t4;
	cout << " t3<=t4   :" << lessEqual.ToStringAsInt() << endl;
	cout << "    0,1 true:   " << lessEqual.True2<0, 1>() << endl;
	cout << "    2,3 false:  " << lessEqual.False2<2, 3>() << endl;
	Vector4 greater = t3 > t4;
	cout << " t3>t4   :   " << greater.ToStringAsInt() << endl;
	Vector4 greaterEq = t3 >= t4;
	cout << " t3>=t4  :" << greaterEq.ToStringAsInt() << endl;

	cout << " true4 test:" << kVector4::AllOneMask.True4() << " " << kVector4::XAllOneMask.True4() << " " << " " << kVector4::XAllOneMask.AnyTrue4() << " " << kVector4::Zero.AnyTrue4() << endl;
	cout << " true4 test:" << kVector4::AllOneMask.AnyFalse4() << " " << kVector4::XAllOneMask.AnyFalse4() << " " << " " << kVector4::XAllOneMask.False4() << " " << kVector4::Zero.False4() << endl;
	cout << " true3 test:" << kVector4::XYZAllOneMask.True3() << " " << kVector4::XYAllOneMask.True3() << endl;
	cout << " true2 test:" << kVector4::XYAllOneMask.True2() << " " << kVector4::XAllOneMask.True2() << endl;
	cout << " true1 test:" << kVector4::XAllOneMask.True1() << " " << kVector4::Zero.True1() << endl;

	Vector4 inf4(kfPositiveInf), inf3(kfPositiveInf, kfPositiveInf, kfPositiveInf, 0u), inf2(kfPositiveInf, kfPositiveInf, 0u, 0u);
	Vector4 inf_l(0u, 0u, 0u, kfPositiveInf);
	Vector4 inf_l2(0u, 0u, kfPositiveInf, kfPositiveInf);
	Vector4 ninf4(kfNegativeInf), ninf3(kfNegativeInf, kfNegativeInf, kfNegativeInf, 0u), ninf2(kfNegativeInf, kfNegativeInf, 0u, 0u);
	cout << " IsInf4 test:" << inf4.IsInfinite4() << " " << ninf4.IsInfinite4() << " " << inf_l.IsInfinite4() << " " << kVector4::TwoPi.IsInfinite4() << endl;
	cout << " IsInf3 test:" << inf3.IsInfinite3() << " " << ninf3.IsInfinite3() << " " << inf_l.IsInfinite3() << " " << kVector4::Pi.IsInfinite3() << endl;
	cout << " IsInf2 test:" << inf2.IsInfinite2() << " " << ninf2.IsInfinite2() << " " << inf_l2.IsInfinite2() << " " << inf_l2.IsInfinite3() << " " << kVector4::Pi.IsInfinite2() << endl;

	Vector4 nan4(kfNaN), nan3(kfNaN, kfNaN, kfNaN, 0u), nan2(kfNaN, kfNaN, 0u, 0u);
	Vector4 nan_l(0u, 0u, 0u, kfNaN), nan_l2(0u, 0u, kfNaN, 0u);
	cout << " IsNan4 test:" << nan4.IsNaN4() << endl;
	cout << " IsNan3 test:" << nan3.IsNaN3() << " " << nan_l.IsNaN3() << " " << nan_l2.IsNaN3() << endl;
	cout << " IsNan2 test:" << nan2.IsNaN3() << " " << nan_l.IsNaN2() << " " << nan_l2.IsNaN2() << endl;


	constexpr Vector4 nearlyone(1.00002f, 0.0f, 0.0f, 0.0000001f);
	Vector4 testNormal(1.0f, 2.0f, 3.0f, 4.0f);
	Vector4 testNormal2(-1.0f, -2.0f, -3.0f, 0.0f);
	Vector4 testNormal3(1.0f, 1.0f, 0.0f, 0.0f);
	testNormal.AssignNormalize4();
	testNormal2.AssignNormalize3();
	testNormal3.AssignNormalize2();
	cout << " IsUnit4 Test:" << nearlyone.IsUnit4() << " " << testNormal.IsUnit4() << endl;
	cout << " IsUnit3 Test:" << testNormal.IsUnit3() << " " << testNormal2.IsUnit3() << endl;
	cout << " IsUnit2 Test:" << testNormal2.IsUnit2() << " " << testNormal3.IsUnit2() << endl;
	cout << endl;

	constexpr Vector4 order1(1.0f, 2.f, 3.f, 4.f), order2(5.f, 6.f, 7.f, 8.f);
	cout << "#########" << "test data order1" << order1.ToString() << " order2 " << order2.ToString() << "#########" << endl;
	cout << " pick compose 1,2,0,1:" << order1.PickCompose<1, 2, 0, 1>(order2).ToString() << endl;
	cout << " order1 dot2 order2  :" << order1.Dot2(order2).ToString() << endl;
	cout << " order1 dot3 order2  :" << order1.Dot3(order2).ToString() << endl;
	cout << " order1 dot4 order2  :" << order1.Dot4(order2).ToString() << endl;

	Vector4 select0(kfAllOneMask, 0u, 0u, 0u), select1(0u, kfAllOneMask, 0u, 0u), select2(0u, 0u, kfAllOneMask, 0u), select3(0u, 0u, 0u, kfAllOneMask);
	cout << " (1,0,0,0) select order1 order2£º" << select0.Select(order1, order2).ToString() << endl;
	cout << " (0,1,0,0) select order1 order2£º" << select1.Select(order1, order2).ToString() << endl;
	cout << " (0,0,1,0) select order1 order2£º" << select2.Select(order1, order2).ToString() << endl;
	cout << " (0,0,0,1) select order1 order2£º" << select3.Select(order1, order2).ToString() << endl;
	cout << endl;

	Vector4 t5{ -1.1f, -1.6f, 1.1f, 1.6f }, t6{ 0.25f, 0.5f, 2.0f, 4.f }, t7{1.0f, 4.f, 9.f, 16.f}, t8{ .03f, .3f, 3.f, 30.f };
	cout << "#########" << "test data t5:" << t5.ToString() << "test data t6:" << t6.ToString() << "test data t7:" << t7.ToString() << "#########" << endl;
	cout << " round:"<<t5.Copy().AssignRound().ToString() << endl;
	cout << " floor:" << t5.Copy().AssignFloor().ToString() << endl;
	cout << " ceil :" << t5.Copy().AssignCeil().ToString() << endl;
	cout << " Fast Rcp:" << t6.Copy().AssignFastRcp().ToString()<<" "<<t8.Copy().AssignFastRcp().ToString()  << endl;
	cout << " Accu Rcp:" << t6.AccurateRcp().ToString() << " " << t8.Copy().AssignAccurateRcp().ToString() << endl;
	cout << " Fast Sqrt:" << t7.Copy().AssignSqrt().ToString() << endl;
	cout << " Fast Sqrt Rcp:" << t7.Copy().AssignFastRcpSqrt().ToString() << endl;
	cout << " Accu Sqrt Rcp:" << t7.AccurateRcpSqrt().ToString() << endl;

	Vector4 t9{ -1.0f, 0.0f, -0.0f, 1.0f }, t10{-1.0, 0.0, 1.0f, 2.0f};
	cout << "test data t9:" << t9.ToString() << "test data t10:" << t10.ToString() << endl;
	cout << " t9 abs  :" << t9.Abs().ToString() << endl;
	cout << " t9 step :" << t10.Step(kVector4::Zero).ToString() << endl;
	cout << " t9 rstep:" << t10.RStep(kVector4::Zero).ToString() << endl;
	cout << " t9 satur:" << t10.Saturate().ToString() << endl;
	cout << " t9 clamp:" << t10.Saturate().ToString() << endl;
	cout << " t9 sum2 :" << t10.Sumup2().ToString() << endl;
	cout << " t9 sum3 :" << t10.Sumup3().ToString() << endl;
	cout << " t9 sum4 :" << t10.Sumup4().ToString() << endl;
	cout << " t9 t10 dist2 :" << t9.Distance2(t10).ToString() << endl;
	cout << " t9 t10 dist3 :" << t9.Distance3(t10).ToString() << endl;
	cout << " t9 t10 dist4 :" << t9.Distance4(t10).ToString() << endl;
	cout << " t9 len2 :" << t10.Length2().ToString() << endl;
	cout << " t9 len3 :" << t10.Length3().ToString() << endl;
	cout << " t9 len4 :" << t10.Length4().ToString() << endl;

	Vector4 rad{ kfPi, kf2Pi, kfHalfPi, kfQuarterPi }, deg{ 90.f, 180.f, 270.f, 360.f };
	cout << "test data rad:" << t9.ToString() << "test data deg:" << t10.ToString() << endl;
	cout << " rad2deg  :" << rad.ToDegree().ToString() << endl;
	cout << " deg2rad  :" << deg.ToRadian().ToString() << endl;

	Vector4 t11{ 3.f, 4.f, 0.f,0.f }, t12{ 0.f, 4.f, 3.f,0.f }, t13{ 0.f, 4.f, 0.f,3.f };
	cout << "test data t11:" << t11.ToString() << "test data t12:" << t12.ToString() << "test data t13:" << t13.ToString() << endl;
	cout << " t11 normalize2:" << t11.Normalize2().ToString() << endl;
	cout << " t11 normalize3:" << t12.Normalize3().ToString() << endl;
	cout << " t11 normalize4:" << t13.Normalize4().ToString() << endl;
}
