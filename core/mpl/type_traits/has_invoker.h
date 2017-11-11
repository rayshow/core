#pragma once

#include<core/mpl/type_traits/impl/has_operator_type.h>
#include<core/mpl/type_traits/impl/has_member_fn_decl.h>

namespace core
{
	namespace mpl
	{
		//class define Ret T::operator()( Args... )
		HAS_MEMBER_FN_DECL( operator (), invoker);
	}
}