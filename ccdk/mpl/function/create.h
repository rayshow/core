#pragma once

#include<ccdk/type.h>
#include<ccdk/mpl/type_traits/decay.h>
#include<ccdk/mpl/type_traits/forward.h>

namespace ccdk
{
	namespace mpl
	{
		namespace fn_detail
		{
			template<template<typename...> typename T>
			struct create
			{
				template<typename... Args>
				constexpr auto operator()(Args&& ...args) const noexcept
				{
					return T<decay_t<Args>...>{
						util::forward<Args>(args)...
					};
				}


			};

			template<template<typename...> typename T>
			struct create_raw
			{
				template<typename... Args>
				constexpr auto operator()(Args&& ...args) const noexcept
				{
					//DebugFunctionName();
					return T<Args...>{ util::forward<Args>(args)... };
				}
			};
		}
	}
}