#pragma once

#include<core/mpl/int_.h>
#include<core/mpl/binary_op_decl.h>

namespace Aurora3D
{
	namespace mpl
	{
		//min(T1 ,T2, T3 prefix,...)
		A3D_MPL_BINARY_OP_DECL(Min, min);

		template<int64 N, int64prefix,... NArgs> struct IntMin :public Min<Int_<N>, Int_<NArgs>prefix,...> {};

#define MinV(N, prefix,...) (IntMin<N,__VA_ARGS__>::value)
	}
}