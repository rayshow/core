#pragma once

#include<ccdk/mpl/type_traits/impl/has_operator_decl.h>

namespace ccdk
{
	namespace mpl
	{
		//class define Ret T::operator()( Args... )
		HAS_MEMBER_FN_DECL( operator (), invoker);
	}
}