#pragma once

#include<ccdk/compile.h>
#include<ccdk/type.h>
#include<ccdk/mpl/function/expr.h>


namespace ccdk
{
	namespace mpl
	{
		namespace fn_impl
		{
			//mark value
			template<typename T>
			struct value_t {};

			template<typename T>
			struct expr< value_t<T> >
			{
				typedef expr type;
				typedef decay_t<T> value_type;

				static constexpr uint32 L = 0;	//args length
				static constexpr uint32 W = 0;  //wild placeholder count of sub-expr(e.g. _ )
				static constexpr uint32 I = 0;  //max index-placeholder count of sub-expr(e.g. 2_ )

				value_type t;

				CCDK_FORCEINLINE constexpr explicit
					expr(const T& inT) 
					:t{ inT }
				{}

				CCDK_FORCEINLINE constexpr explicit
					expr(T&& inT) 
					:t{ util::move(inT) }
				{}

				template<uint32 Start, typename... Args>
				const T& eval(Args const& ...) const noexcept
				{
					return t;
				}

				template<typename... Args>
				CCDK_FORCEINLINE constexpr
				const T& operator()(Args const& ...) const noexcept
				{
					return t;
				}
			};
		}
	}
}