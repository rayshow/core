#pragma once

#include<ccdk/compile.h>
#include<ccdk/mpl/type_traits/remove_ref.h>

namespace ccdk
{
	namespace mpl
	{
		namespace util
		{
			template<
				typename T,
				typename P = remove_ref_t<T> 
			> 
			CCDK_FORCEINLINE constexpr 
			P&& move(T&& inT) noexcept 
			{ 
				return static_cast<P&&>(inT); 
			}
		}
	}
}