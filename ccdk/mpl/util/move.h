#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/type_traits/remove_ref.h>

ccdk_namespace_mpl_util_start

			template<
				typename T,
				typename P = remove_ref_t<T> 
			> 
			CCDK_FORCEINLINE constexpr 
			P&& move(T&& inT) noexcept 
			{ 
				return static_cast<P&&>(inT); 
			}

ccdk_namespace_mpl_util_end