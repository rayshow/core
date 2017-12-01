#pragma once


#include<ccdk/type.h>
#include<ccdk/mpl/type_traits/is_function.h>
#include<ccdk/mpl/type_traits/is_mfn_ptr.h>
#include<ccdk/mpl/type_traits/is_function_obj.h>
#include<ccdk/mpl/type_traits/is_invokable.h>
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

				partial_t(Fn inFn, Args... args)
					:fn{ util::forward<Fn>(inFn) },
					 storage{ util::forward<Args>(args)... }
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
				auto partial_normal_function(arg_pack<Args2...>, Fn&& fn, Args1&&... args1) const noexcept
				{
					return partial_t<sizeof...(Args2), decay_t<Fn>, Ret, Args1...>{
							fn,
							util::forward<Args1>(args1)...
					};
				}

				template<typename Ret, typename Fn, typename... Args1, typename... Args2>
				auto partial_function_obj(arg_pack<Args2...>, Fn&& fn, Args1&&... args1) const noexcept
				{
					return partial_t<sizeof...(Args2), decay_t<Fn>, Ret, Args1...>{
							util::forward<Fn>(fn),
							util::forward<Args1>(args1)...
					};
				}


				//is function
				template<typename Fn, typename... Args>
				auto partial_create_impl(true_, false_, Fn&& fn, Args&&... args) const noexcept
				{
					typedef function_traits<Fn> FT;
					return partial_normal_function<typename FT::ret>(
						typename FT::args{},
						fn,
						util::forward<Args>(args)...
					);
				}

				//is member function pointer
				template<typename Fn, typename C, typename... Args>
				auto partial_create_impl(false_, true_, Fn&& fn, C&& c, Args&&... args) const noexcept
				{ 
					typedef mfn_traits<Fn> MT;
					return partial_function_obj<typename MT::ret>(
						typename MT::args{}, 
						bind_mfn(fn, c),
						util::forward<Args>(args)...
					);
				}

				//is function object, need check
				template<typename Fn, typename... Args, typename = check_t< is_function_obj<Fn>> >
				auto partial_create_impl(false_, false_, Fn&& fn, Args&&... args) const noexcept
				{
					typedef mfn_traits<decltype(&Fn::operator())> MT;
					return partial_function_obj<typename MT::ret>(
						typename MT::args{},
						util::forward<Fn>(fn),
						util::forward<Args>(args)...
					);
				}

				template<typename Fn, typename = check_t< is_invokable<remove_ref_t<Fn>>>, typename... Args>
				auto operator()(Fn&& fn, Args&&... args) const noexcept
				{
					

					//static dispatch
					//1. first make sure Fn is function ptr or member function ptr
					//2. default check is function obj
					//3. or will rise no-match function error
					/*return partial_create_impl(
						typename is_function_ptr<Fn>::type{},
						typename is_mfn_ptr<Fn>::type{},
						util::forward<Fn>(fn),
						util::forward<Args>(args)...
					);*/
					return 0;
				}
			};
		}


		//partialize for non-override function/function pointer/ member function pointer/ function object with operator()
		constexpr function_detail::partial_create_t partial{};
	}
}