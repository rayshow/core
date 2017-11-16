#pragma once

#include<ccdk/mpl/type_traits/impl/has_operator_decl.h>

namespace ccdk
{
	namespace mpl
	{
		CCDK_TT_HAS_RET_UNARY_OP(~, logic_not);
	}
}