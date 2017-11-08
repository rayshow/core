#pragma once

#include<core/mpl/int_.h>
namespace Aurora3D
{
	namespace mpl
	{

		template<typename S> struct Length: public Int_<S::length>{};
		template<typename S> struct Empty : public Bool_<S::length == 0> {};
		struct ZeroLength { static constexpr int length = 0; };
	}
}