#pragma once
#include<core/mpl/bool_.h>
#include<core/mpl/if.h>

namespace Aurora3D
{
	namespace mpl
	{
		//T1 && T2 && T3 prefix,...
		template<typename prefix,... Args> struct Or:public False_ {};
		template<typename T, typenameprefix,... TArgs> struct Or<T, TArgsprefix,...> 
			: public DeriveIf<T, True_, Or<TArgsprefix,...>>{};
	}
}