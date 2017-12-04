#pragma once

#include<ccdk/type.h>
#include<ccdk/mpl/type_traits/forward.h>
#include<ccdk/mpl/base/enable_if.h>

namespace ccdk
{
	namespace mpl
	{
		namespace fn_detail
		{
			template<uint32 n>
			struct arg_t
			{
				template<typename T0, typename T1, typename T2,
					typename T3, typename T4, typename T5,
					typename... Args,
					typename = check<(sizeof...(Args) >= n - 5)> >

					constexpr decltype(auto) operator()(T0&& t0, T1&& t1, T2&& t2,
						T3&& t3, T4&& t4, T5&& t5, Args&&... args) const noexcept
				{
					return arg_t<n - 6>{}(util::forward<Args>(args)...);
				}
			};

			template<>
			struct arg_t<0>
			{
				template<typename T, typename... Args>
				constexpr decltype(auto) operator()(T&& t, Args&&... args) const noexcept
				{
					return util::forward<T>(t);
				}
			};

			template<>
			struct arg_t<1>
			{
				template<typename T0, typename T1, typename... Args>
				constexpr decltype(auto) operator()(T0&& t0, T1&& t1,
					Args&&... args) const noexcept
				{
					return util::forward<T1>(t1);
				}
			};

			template<>
			struct arg_t<2>
			{
				template<typename T0, typename T1, typename T2, typename... Args>
				constexpr decltype(auto) operator()(T0&& t0, T1&& t1,
					T2&& t2, Args&&... args) const noexcept
				{
					return util::forward<T2>(t2);
				}
			};

			template<>
			struct arg_t<3>
			{
				template<typename T0, typename T1, typename T2, typename T3, typename... Args>
				constexpr decltype(auto) operator()(T0&& t0, T1&& t1,
					T2&& t2, T3&& t3, Args&&... args) const noexcept
				{
					return util::forward<T3>(t3);
				}
			};

			template<>
			struct arg_t<4>
			{
				template<typename T0, typename T1, typename T2, typename T3,
					typename T4, typename... Args>
					constexpr decltype(auto) operator()(T0&& t0, T1&& t1, T2&& t2,
						T3&& t3, T4&& t4, Args&&... args) const noexcept
				{
					return util::forward<T4>(t4);
				}
			};

			template<>
			struct arg_t<5>
			{
				template<typename T0, typename T1, typename T2, typename T3,
					typename T4, typename T5, typename... Args>
					constexpr decltype(auto) operator()(T0&& t0, T1&& t1, T2&& t2,
						T3&& t3, T4&& t4, T5&& t5, Args&&... args) const noexcept
				{
					return util::forward<T5>(t5);
				}
			};
		}
		
		template<uint32 n>
		constexpr fn_detail::arg_t<n> arg{};
	}
}
