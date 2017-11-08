#pragma once
namespace Aurora3D
{
	namespace mpl
	{
#define BoolIfV(c,a,b)   ((c)?(a)::(b))

		// for non-inner-contained-type type
		// if(B) type = T1
		// else type = T2
		template<bool B, typename T1, typename T2>
		struct BoolIf  { typedef T1 type; };
		template<typename T1, typename T2>
		struct BoolIf<false, T1, T2>  { typedef T2 type; };
		template<bool B, typename T1, typename T2>
		using BoolIfT = typename BoolIf<B, T1, T2>::type;

		// if(C::value) type = T1
		// else type = T2
		template<typename C, typename T1, typename T2>
		struct If :public BoolIf<C::value, T1, T2> {};
		template<typename C, typename T1, typename T2>
		using IfT = typename If<C, T1, T2>::type;

		// for inner-contained-type type
		// if(B) derive T1
		// else  derive T2
		template<bool B, typename T1, typename T2> 
		struct BoolDeriveIf :public T1 {};
		template<typename T1, typename T2> 
		struct BoolDeriveIf<false, T1, T2> :public T2 {};

		// if(C::value) derive T1
		// else derive T2
		template<typename C, typename T1, typename T2>
		struct DeriveIf :public BoolDeriveIf<C::value, T1, T2> {};
	}
}