#pragma once

#include<Core/mpl/container/set_decl.h>

namespace Aurora3D
{
	namespace mpl
	{
		template<typename S, typename Key, typename CmpFn>
		struct SetHasKey :Bool_< FoundV( SetSearch<S, Key, CmpFn>) > {};
	}
}