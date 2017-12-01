#pragma once


#include<ccdk/type.h>
#include<ccdk/mpl/type_traits/is_function.h>
#include<ccdk/mpl/type_traits/is_mfn_ptr.h>
#include<ccdk/mpl/type_traits/is_function_obj.h>
#include<ccdk/mpl/type_traits/forward.h>
#include<ccdk/mpl/type_traits/move.h>
#include<ccdk/mpl/type_traits/decay.h>
#include<ccdk/mpl/container/tuple_storage.h>
#include<ccdk/mpl/function/bind_mfn.h>

namespace ccdk
{
	namespace mpl
	{
		namespace function_detail
		{
			template<uint32 L, typename Fn, typename Ret, typename... Args>
			struct partial_t
			{
				constexpr static uint32 arg_length = sizeof...(Args);
				constexpr static uint32 left_length = L - arg_length;
				typedef partial_t<L, Fn, Ret, Args...> type;
				typedef tuple_storage< arg_length, make_indice<arg_length>, decay_t<Args>...> value_type;
				value_type storage;
				Fn fn;

				partial_t(Fn inFn, Args&&... args)
					:fn{ util::move(inFn) }, storage{ util::move(args)... }
				{}

				template<typename... Args1, uint32... indice>
				Ret __invoke_impl(indice_pack<indice...>, Args1&&... args1)
				{
					DebugFunctionName();
					return fn(ebo_at<indice>(storage)...,
						args1...
					);
				}

				template<typename... Args1>
				Ret operator()(Args1&&... args1)
				{
					return __invoke_impl(
						make_indice<arg_length>{},
						util::forward<Args1>(args1)...
					);
				}
			};

			struct partial_create_t
			{
				template<typename Ret, typename Fn, typename... Args1, typename... Args2>
				auto partial_normal_function(fn_args<Args2...>, Fn&& fn, Args1&&... args1) const noexcept
				{
					return partial_t<sizeof...(Args2), decay_t<Fn>, Ret, Args1...>{
						fn,
							util::move(args1)...
					};
				}

				template<typename Ret, typename Fn, typename... Args1, typename... Args2>
				auto partial_member_function(mfn_args<Args2...>, Fn&& fn, Args1&&... args1) const noexcept
				{
					return partial_t<sizeof...(Args2), decay_t<Fn>, Ret, Args1...>{
						util::move(fn),
							util::move(args1)...
					};
				}


				//is function
				template<typename Fn, typename... Args>
				auto partial_create_impl(true_, false_, Fn&& fn, Args&&... args) const noexcept
				{
					typedef function_body<Fn> FB;
					return partial_normal_function<typename FB::ret>(
						typename FB::args{},
						fn,
						util::move(args)...);
				}

				//is member function
				template<typename Fn, typename C, typename... Args>
				auto partial_create_impl(false_, true_, Fn&& fn, C&& c, Args&&... args) const noexcept
				{ 
					typedef mfn_body<Fn> MB;
					return partial_member_function<typename MB::ret>(
						typename MB::args{},
						bind_mfn(fn, c),
						util::move(args)...);
				}

				//is function object, need check
				template<typename Fn, typename... Args, typename = check< is_function_obj_v<Fn>> >
				auto partial_create_impl(false_, false_, Fn&& fn, Args&&... args) const noexcept
				{
					typedef mfn_body<decltype(&Fn::operator())> MB;
					return partial_member_function<typename MB::ret>(
						typename MB::args{},
						util::move(fn),
						util::move(args)...);
				}

				template<typename Fn, typename... Args>
				auto operator()(Fn&& fn, Args&&... args) const noexcept
				{
					//static dispatch
					//1. first make sure Fn is function ptr or member function ptr
					//2. default check is function obj
					//3. or will rise no-match function error
					return partial_create_impl(
						typename is_function_ptr<Fn>::type{},
						typename is_mfn_ptr<Fn>::type{},
						util::move(fn),
						util::move(args)...);
				}
			};
		}

		constexpr function_detail::partial_create_t partial{};
	}
}