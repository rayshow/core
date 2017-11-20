#pragma once

#include<ccdk/mpl/type_traits/impl/has_operator_decl.h>

namespace ccdk
{
	namespace mpl
	{
		//test if typename T::type is well-formed
		CCDK_TT_HAS_INNER_DECL(type, type);

		//test if typename T::value_type is well-formed
		CCDK_TT_HAS_INNER_DECL(value_type, value_type);

		//test if typename T::tag is well-formed
		CCDK_TT_HAS_INNER_DECL(tag, tag);
	}
}