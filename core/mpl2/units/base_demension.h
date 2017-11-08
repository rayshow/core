#pragma once


#include<core/mpl/ingore_t.h>
#include<core/mpl/int_.h>

namespace Aurora3D
{
	namespace mpl
	{
		// L length  (m)
		// M mass    (kg)
		// T time    (s)
		// I eletricity (A)
		// O temperature (k)
		// N substance quantity (mol)
		// J light intensity (cd)
		template<int64 L, int64 M, int64 T, int64 I, int64 O, int64 N, int64 J>
		struct BaseDemension
		{

		};

		//base 
		typedef BaseDemension<0, 0, 0, 0, 0, 0, 0> Scaler;
		typedef BaseDemension<1, 0, 0, 0, 0, 0, 0> Length;
		typedef BaseDemension<0, 1, 0, 0, 0, 0, 0> Mass;
		typedef BaseDemension<0, 0, 1, 0, 0, 0, 0> Time;
		typedef BaseDemension<0, 0, 0, 1, 0, 0, 0> EletricityQuantity;
		typedef BaseDemension<0, 0, 0, 0, 1, 0, 0> Temperature;
		typedef BaseDemension<0, 0, 0, 0, 0, 1, 0> SubstanceQuantity;
		typedef BaseDemension<0, 0, 0, 0, 0, 0, 1> LightAmount;

		//compose
		typedef BaseDemension<2, 0, 0, 0, 0, 0, 0>  Acreage;
		typedef BaseDemension<3, 0, 0, 0, 0, 0, 0>  Volume;
		typedef BaseDemension<1, 0, -1, 0, 0, 0, 0> Velocity;
		typedef BaseDemension<1, 0, -2, 0, 0, 0, 0> Acceleration;   
		typedef BaseDemension<1, 1, -2, 0, 0, 0, 0> Force;          //M*L/T^2
		typedef BaseDemension<1, 1, -1, 0, 0, 0, 0> Momentum;       //M*L/T  
		
	}
}