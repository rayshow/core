#pragma once

#include<core/mpl/type_traits/impl/has_operator_decl.h>

namespace core
{
	namespace mpl
	{
		CCDK_TT_HAS_RET_UNARY_OP(~, bit_reverse);
	}
}