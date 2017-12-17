#pragma once

#include<ccdk/compile.h>

namespace ccdk
{
	namespace mpl
	{
		template<typename... Args> struct arg_pack {};

		template<typename... Args>
		CCDK_FORCEINLINE constexpr void 
		arg_dummy_fn(Args&&... args) {}
	}
}