#pragma once

#include<ccdk/mpl/type_traits/impl/has_operator_decl.h>
#include<ccdk/mpl/type_traits/traits_case.h>

namespace ccdk
{
	namespace mpl
	{
		
		//class define Ret T::operator()( Args... )
		HAS_MEMBER_FN_DECL( operator (), invoker);

		CCDK_TT_SINGLE_RANK(has_invoker);
	}
}