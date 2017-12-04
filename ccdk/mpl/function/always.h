#pragma once

#include<ccdk/type.h>
#include<ccdk/mpl/type_traits/move.h>
#include<ccdk/mpl/function/create.h>

namespace ccdk
{
	namespace mpl
	{
		namespace fn_detail
		{
			template<typename T>
			struct always_t
			{
				T t;
				template<typename... Args>
				constexpr T const& operator()(Args const&...) const&
				{
					return t;
				}

				template<typename... Args>
				constexpr T& operator()(Args const& ...) &
				{
					return t;
				}

				template<typename... Args>
				constexpr T operator()(Args const& ...) &&
				{
					return util::move(t);
				}
			};
		}

		constexpr fn_detail::create<fn_detail::always_t> always{};
	}
}