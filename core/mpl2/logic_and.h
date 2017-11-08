#pragma once

#include<core/mpl/bool_.h>
#include<core/mpl/if.h>

namespace Aurora3D
{
	namespace mpl
	{
		//lazy 
		template<typenameprefix,... Args> struct And : public True_ {};

		template<typename First, typenameprefix,... Other> struct And<First, Otherprefix,...> 
			: public DeriveIf<First, And<Otherprefix,...>, False_>{};
	}
}