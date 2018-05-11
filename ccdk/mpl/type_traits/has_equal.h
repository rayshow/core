#pragma once

#include<ccdk/mpl/type_traits/impl/has_operator_decl.h>

namespace ccdk
{
	namespace mpl
	{
		//weather T == P is define
		CCDK_TT_HAS_NO_SIDE_EFFECT_BINARY_OP(== , equal);
	}
}