#pragma once

#include<ccdk/compile.h>
#include<ccdk/type.h>
#include<ccdk/mpl/util/move.h>
#include<ccdk/mpl/type_traits/has_constructor.h>
#include<ccdk/mpl/function/create.h>

namespace ccdk
{
	namespace mpl
	{
		namespace fn_detail
		{
			template<typename T>
			struct reference_wrapper
			{
				typedef reference_wrapper type;
				typedef T& value_type;

				value_type t;

				CCDK_FORCEINLINE constexpr explicit
					reference_wrapper(value_type inT) noexcept( has_nothrow_copy_constructor_v<T> )
					:t(inT)
				{}

				CCDK_FORCEINLINE constexpr explicit
					reference_wrapper(T&& inT) noexcept(has_nothrow_move_constructor_v<T>)
					:t(util::move(inT))
				{}

				CCDK_FORCEINLINE constexpr
					operator value_type() const noexcept
				{
					return t;
				}

				template<typename... Args>
				const T& eval(Args&&...) const noexcept
				{
					return t;
				}

				template<typename... Args>
				CCDK_FORCEINLINE constexpr
					value_type& operator()(Args&&... args) noexcept
				{
					return t;
				}
			};
		}

		namespace fn
		{
			constexpr fn_impl::create<fn_detail::reference_wrapper> ref{};
			constexpr fn_impl::create_const_raw<fn_detail::reference_wrapper> cref{};
		}

	}
}