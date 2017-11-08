#pragma once

#include<core/type.h>
#include<core/mpl/integral_decl.h>

namespace Aurora3D
{
	namespace mpl
	{
#define GreaterV(a, b)    ((a)>(b))
#define GreaterEqV(a, b)  ((a)>=(b))
#define LessV(a, b)       ((a)<(b))
#define LessEqV(a,b)      ((a)<=(b))
#define EqualV(a,b)       ((a)==(b))
#define NotEqV(a,b)       ((a)!=(b))
#define ShlV(a,b)         ((a)<<(b))
#define ShrV(a,b)         ((a)>>(b))
#define Max2V(a,b)        ((a)>(b)?(a):(b))
#define Min2V(a,b)        ((a)<(b)?(a):(b))

		template<int64 N> struct Int_:public Integral_<int64, N>
		{ 
			typedef Int_<N>       type;
			typedef Int_<-N>      not;
			typedef Bool_<N != 0> to_bool;
			
			//arithmatic
			typedef Int_<N + 1> next;
			typedef Int_<N - 1> prior;
			template<typename T> struct add :public Int_<N + ValueV(T)> {};
			template<typename T> struct sub :public Int_<N - ValueV(T)> {};
			template<typename T> struct mul :public Int_<N * ValueV(T)> {};
			template<typename T> struct div :public Int_<N / ValueV(T)> {};
			template<typename T> struct mod :public Int_<N % ValueV(T)> {};
			template<typename T> struct max :public Int_< Max2V(N, ValueV(T))> {};
			template<typename T> struct min :public Int_< Min2V(N, ValueV(T))> {};

			//cmp
			template<typename T> struct less :public Bool_<LessV(N, ValueV(T))> {};
			template<typename T> struct less_eq :public Bool_<LessEqV(N, ValueV(T))> {};
			template<typename T> struct equal :public Bool_<N == ValueV(T)> {};

			//bit op
			typedef Int_<~N> bitnot;
			template<typename T> struct bitand :Int_<N & ValueV(T)> {};
			template<typename T> struct bitor  :Int_<N | ValueV(T)> {};
			template<typename T> struct bitxor :Int_<N ^ ValueV(T)> {};
			template<typename T> struct bitshl :Int_<ShlV(N, ValueV(T))> {};
			template<typename T> struct bitshr :Int_<ShrV(N, ValueV(T))> {};
		};

		template<typename T> using IntT = typename T::to_int;

		typedef Int_<-1> intn1_;
		typedef Int_<0>  int0_;
		typedef Int_<1>  int1_;
		typedef Int_<2>  int2_;
		typedef Int_<3>  int3_;
		typedef Int_<4>  int4_;
		typedef Int_<5>  int5_;
		typedef Int_<6>  int6_;
		typedef Int_<7>  int7_;
		typedef Int_<8>  int8_;
		typedef Int_<9>  int9_;
	}
}