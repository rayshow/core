#pragma once

#include<ccdk/type.h>
#include<ccdk/mpl/base/enable_if.h>
#include<ccdk/mpl/base/or_.h>
#include<ccdk/mpl/type_traits/decay.h>
#include<ccdk/mpl/type_traits/is_function.h>
#include<ccdk/mpl/type_traits/is_function_obj.h>
#include<ccdk/mpl/container/tuple_storage.h>
#include<ccdk/mpl/function/partial.h>
#include<ccdk/mpl/function/create.h>

namespace ccdk
{
	namespace mpl
	{
		namespace fn_detail
		{
			template<typename... Args>
			struct capture_t
			{
				static constexpr uint32 L = sizeof...(Args);
				typedef tuple_storage<L, make_indice<L>, decay_t<Args>...> value_type;
				typedef capture_t<Args...> type;
				value_type storage;

				constexpr capture_t(Args... args)
					:storage{ util::forward<Args>(args)... }
				{}

				template<typename Fn, uint32... indice>
				constexpr auto __invoke_impl(Fn&& fn, indice_pack<indice...>) const noexcept
				{
					return partial(util::forward<Fn>(fn), ebo_at<indice>(util::move(storage))...);
				}

				//function ptr
				template<typename Fn, typename = check_t< is_function<Fn> >>
				constexpr auto operator()(Fn* fn) const noexcept
				{
					return __invoke_impl(util::forward<Fn*>(fn), make_indice<L>{});
				}

				template<typename Fn, typename = check_t< or_< is_function<Fn>, is_function_obj<Fn>>>  >
				constexpr auto operator()(Fn& fn) const noexcept
				{
					return __invoke_impl(util::forward<Fn&>(fn), make_indice<L>{});
				}
			};
		}

		constexpr fn_detail::create< fn_detail::capture_t > capture{};
	}
}