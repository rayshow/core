#pragma once

#include<ccdk/mpl/base/logic_.h>
#include<ccdk/mpl/type_traits/is_function.h>
#include<ccdk/mpl/type_traits/is_function_obj.h>
#include<ccdk/mpl/type_traits/is_mfn_ptr.h>
#include<ccdk/mpl/type_traits/declval.h>
#include<ccdk/mpl/type_traits/is_same.h>
#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/mpl_module.h>

namespace ccdk
{
	namespace mpl
	{
		namespace tt_impl {

			// test weather Ret FN(Args...) is exists
			template<typename FN, typename Ret, typename ... Args>
			struct is_invocable_impl {

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
			struct is_invocable_impl<FN, null_, Args...> {

				template<
					typename FN2,
					typename = decltype(declval<FN2>()(declval<Args>()...))>
					constexpr static bool sfinae(int) { return true; }

				template<typename FN2>
				constexpr static bool sfinae(...) { return false; }

				static constexpr bool value = sfinae<FN>(0);
			};
		}

		// testing callable type with parameter
		template<typename FN, typename... Args>
		struct is_invocable
			:bool_<tt_impl::is_invocable_impl<FN, null_, Args...>::value> {};

		// testing callable type ingore parameter and ret
		template<typename Fn>
		struct is_invocable<Fn>
			:or_< is_function<Fn>, is_mfn_ptr<Fn>, is_function_obj<Fn> > {};

		// testing callable type with parameter and ret
		template<typename FN, typename Ret, typename... Args>
		struct is_invocable_ret
			:bool_<tt_impl::is_invocable_impl<FN, Ret, Args...>::value> {};

		template<typename FN, typename... Args>
		static constexpr bool is_invocable_v =
			tt_impl::is_invocable_impl<FN, null_, Args...>::value;

		template<typename FN, typename Ret, typename... Args>
		static constexpr bool is_invocable_ret_v =
			tt_impl::is_invocable_impl<FN, Ret, Args...>::value;

	
	}
}

