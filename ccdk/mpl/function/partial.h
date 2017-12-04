#pragma once


#include<ccdk/type.h>
#include<ccdk/mpl/type_traits/is_function.h>
#include<ccdk/mpl/type_traits/is_mfn_ptr.h>
#include<ccdk/mpl/type_traits/is_function_obj.h>
#include<ccdk/mpl/type_traits/is_invokable.h>
#include<ccdk/mpl/type_traits/forward.h>
#include<ccdk/mpl/type_traits/move.h>
#include<ccdk/mpl/type_traits/decay.h>
#include<ccdk/mpl/type_traits/select_case.h>
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

				partial_t(const Fn& inFn, const Args&... args)
					:fn{ inFn },
					 storage{ args... }
				{}

				partial_t(Fn&& inFn, const Args&... args)
					:fn{ util::move(inFn) },
					storage{ args... }
				{}

				//use for re-partial
				partial_t( int, const Fn& inFn, const Args&... args)
					:fn{ util::move(inFn) },
					storage{ util::move(args)... }
				{}

				//args is fulfiled / execute
				template<typename... Args1, uint32... indice>
				Ret __invoke_impl( true_, indice_pack<indice...>, Args1&&... args1)
				{
					return fn(ebo_at<indice>(util::move(storage))...,
						args1...
					);
				}

				//args is not fulfiled / return new partial_t and move parameter
				template<typename... Args1,  uint32... indice>
				auto __invoke_impl( false_, indice_pack<indice...>, Args1&&... args1)
				{
					static_assert(sizeof...(Args1) < left_length, "parameter length out of limit");

					//always move all parameters
					return partial_t< L, Fn, Ret, Args..., Args1...>{
						0,
						util::move(fn),
						ebo_at<indice>(util::move(storage))...,
						args1...
					};
				}


				template<typename... Args1, typename = check< (sizeof...(Args1)>0) > >
				auto operator()(Args1&&... args1)
				{
					return __invoke_impl(
						bool_<  sizeof...(Args1) == left_length >{},
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


				//case is function / function pointer
				template<typename Fn, typename... Args>
				auto partial_create_impl(case_or< is_function>, Fn&& fn, Args&&... args) const noexcept
				{
					typedef function_traits<remove_ref_t<Fn>> FT;
					return partial_normal_function<typename FT::ret>(
						typename FT::args{},
						fn,
						util::forward<Args>(args)...
					);
				}

				//case is member function pointer, need wrap with object
				template<typename Fn,
					typename Fn1 = remove_ref_t<Fn>,
					typename C, typename... Args>
				auto partial_create_impl(case_t< is_mfn_ptr> , Fn&& fn, C&& c, Args&&... args) const noexcept
				{ 
					typedef mfn_traits<Fn1> MT;
					return partial_function_obj<typename MT::ret>(
						typename MT::args{}, 
						bind_mfn(fn, c),
						util::forward<Args>(args)...
					);
				}

				//case is function object
				template<typename Fn,
					typename Fn1 = remove_ref_t<Fn>,
					typename... Args>
				auto partial_create_impl(case_t< is_function_obj>, Fn&& fn, Args&&... args) const noexcept
				{
					typedef mfn_traits<decltype(&Fn1::operator())> MT;
					return partial_function_obj<typename MT::ret>(
						typename MT::args{},
						util::forward<Fn>(fn),
						util::forward<Args>(args)...
					);
				}

				template<typename Fn,
					typename Fn1 = remove_ref_t<Fn>,         
					typename = check_t< is_invokable<Fn1>>,  //check Fn is  
					typename... Args>
				auto operator()(Fn&& fn, Args&&... args) const noexcept
				{
					//static dispatch
					//Fn can be function/function pointer/member pointer/function object
					return partial_create_impl(
						select_case<
							is_function<Fn1>,
							is_mfn_ptr<Fn1>,
							is_function_obj<Fn1>>,
						util::forward<Fn>(fn),
						util::forward<Args>(args)...
					);
				}
			};
		}


		//partialize for non-override function/function pointer/ member function pointer/ function object with operator()
		constexpr function_detail::partial_create_t partial{};
	}
}