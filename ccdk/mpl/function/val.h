#pragma once

#include<ccdk/compile.h>
#include<ccdk/type.h>
#include<ccdk/mpl/type_traits/has_constructor.h>
#include<ccdk/mpl/util/move.h>
#include<ccdk/mpl/function/create.h>

namespace ccdk
{
	namespace mpl
	{
		namespace fn_detail
		{
			template<typename T>
			struct value_t
			{
				typedef value_t type;
				typedef T value_type;

				T t;

				CCDK_FORCEINLINE constexpr explicit
					value_t(const T& inT) noexcept(has_nothrow_copy_constructor_v<T>)
					:t{ inT }
				{}

				CCDK_FORCEINLINE constexpr explicit
					value_t(T&& inT) noexcept(has_nothrow_move_constructor_v<T>)
					:t{ util::move(inT) }
				{}

				template<typename... Args>
				CCDK_FORCEINLINE constexpr 
					T const& operator()(Args const&...) const& noexcept
				{
					return t;
				}

				template<typename... Args>
				CCDK_FORCEINLINE constexpr
				T& operator()(Args const& ...) & noexcept
				{
					return t;
				}

				template<typename... Args>
				CCDK_FORCEINLINE constexpr
				T operator()(Args const& ...) && noexcept
				{
					return util::move(t);
				}
			};
		}

		namespace fn
		{
			constexpr fn_detail::create<fn_detail::val_t> val{};
		}
		
	}
}