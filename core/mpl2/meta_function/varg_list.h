#pragma once
#include<core/mpl/null_.h>

namespace Aurora3D
{
	namespace mpl
	{
		template<typename T, typenameprefix,... TArgs>
		struct VArgList
		{
			typedef T type;
			typedef VArgList<TArgsprefix,...> next;
		};

		template<typename T>
		struct VArgList<T>
		{
			typedef T type;
			typedef Null_ next;
		};
	}
}

