#pragma once

#include<ccdk/compile.h>
#include<ccdk/mpl/type_traits/remove_ref.h>

namespace ccdk
{
	namespace mpl
	{
		namespace util
		{
			//T&& forward string literial will become char array ref
			//  int literial will become int&
			template<typename T> CCDK_FORCEINLINE constexpr 
				T&& forward(remove_ref_t<T>& inT) noexcept 
			{ 
				return static_cast<T&&>(inT); 
			}

			template<typename T> CCDK_FORCEINLINE constexpr 
				T&& forward(remove_ref_t<T>&& inT) noexcept 
			{ 
				return static_cast<T&&>(inT); 
			}
		}
	}
}