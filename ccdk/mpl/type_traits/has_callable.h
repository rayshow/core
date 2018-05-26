#pragma once

#include<ccdk/mpl/type_traits/declval.h>
#include<ccdk/mpl/type_traits/is_same.h>
#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/mpl_module.h>

ccdk_namespace_mpl_start

namespace tt_impl {

	// test weather Ret FN(Args...) is exists
	template<typename FN, typename Ret, typename ... Args>
	struct has_callable_impl {

		template<
			typename FN2,
			typename Ret2 = decltype(declval<FN2>()(declval<Args>()...)),
			typename = check_t< is_same<Ret, Ret2>> >
			constexpr static bool sfinae(int) { return true; }

		template<typename FN2>
		constexpr static bool sfinae(...) { return false; }

		static constexpr bool value = sfinae<FN>(0);
	};

	// test weather anyRet FN(args...) is exists
	template<typename FN, typename ... Args>
	struct has_callable_impl<FN,null_,Args...> {

		template<
			typename FN2,
			typename = decltype(declval<FN2>()(declval<Args>()...))>
			constexpr static bool sfinae(int) { return true; }

		template<typename FN2>
		constexpr static bool sfinae(...) { return false; }

		static constexpr bool value = sfinae<FN>(0);
	};
}

template<typename FN, typename... Args>
struct has_callable
	:bool_<tt_impl::has_callable_impl<FN, null_, Args...>::value> {};

template<typename FN, typename Ret, typename... Args>
struct has_callable_ret
	:bool_<tt_impl::has_callable_impl<FN, Ret, Args...>::value> {};

template<typename FN, typename... Args>
static constexpr bool has_callable_v =
tt_impl::has_callable_impl<FN, null_, Args...>::value;

template<typename FN, typename Ret, typename... Args>
static constexpr bool has_callable_ret_v =
tt_impl::has_callable_impl<FN, Ret, Args...>::value;

ccdk_namespace_mpl_end