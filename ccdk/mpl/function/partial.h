#pragma once


#include<ccdk/type.h>
#include<ccdk/mpl/type_traits/is_function.h>
#include<ccdk/mpl/type_traits/is_mfn_ptr.h>
#include<ccdk/mpl/type_traits/is_function_obj.h>
#include<ccdk/mpl/type_traits/is_invokable.h>
#include<ccdk/mpl/type_traits/decay.h>
#include<ccdk/mpl/type_traits/select_case.h>
#include<ccdk/mpl/type_traits/args_of.h>
#include<ccdk/mpl/type_traits/result_of.h>
#include<ccdk/mpl/util/forward.h>
#include<ccdk/mpl/util/move.h>
#include<ccdk/mpl/container/tuple_storage.h>
#include<ccdk/mpl/function/bind_mfn.h>

namespace ccdk
{
	namespace mpl
	{
		//implments, internal use
		namespace fn_impl
		{
			template<uint32 L, typename Fn, typename Ret, typename... Args>
			struct partial_t
			{
				constexpr static uint32 arg_length = sizeof...(Args);
				constexpr static uint32 left_length = L - arg_length;
				typedef partial_t type;
				typedef tuple_storage< arg_length, make_indice<arg_length>, decay_t<Args>...> value_type;

				value_type storage;
				decay_t<Fn> fn;

				CCDK_FORCEINLINE
				partial_t(Fn&& inFn, Args&&... args)
					:fn{ util::forward<Fn>(inFn) },
					 storage{ util::forward<Args>(args)... }
				{}

				//use for re-partial
				CCDK_FORCEINLINE
				partial_t( int, const Fn& inFn, const Args&... args)
					:fn{ util::move(inFn) },
					storage{ util::move(args)... }
				{}

				//args is fulfiled / execute
				template<
					typename... Args1,
					uint32... indice
				>
				CCDK_FORCEINLINE
				auto
				__invoke_impl( true_,  indice_pack<indice...>, Args1&&... args1)
				{
					return fn(ebo_at<indice>(util::move(storage))...,
						args1...
					);
				}

				//args is not fulfiled / return new partial_t and move parameter
				template<
					typename... Args1,
					uint32... indice
				>
				CCDK_FORCEINLINE
				auto
				__invoke_impl( false_,  indice_pack<indice...>, Args1&&... args1)
				{
					static_assert(sizeof...(Args1) < left_length,
						"parameter length out of limit");

					//always move all parameters
					return partial_t< L, Fn, Ret, Args..., Args1...>{
						0,
						util::move(fn),
						ebo_at<indice>(util::move(storage))...,
						args1...
					};
				}


				template<
					typename... Args1,
					typename = check< (sizeof...(Args1)>0) >
				>
				CCDK_FORCEINLINE
				auto
				operator()(Args1&&... args1)
				{
					return __invoke_impl(
						bool_<  sizeof...(Args1) == left_length >{},
						make_indice<arg_length>{},
						util::forward<Args1>(args1)...
					);
				}
			};


			//for non-overload function create partial
			struct partial_create_t
			{
				template<
					typename Ret,
					typename Fn,
					typename... Args1,
					typename... Args2
				>
				CCDK_FORCEINLINE
				auto
				partial_normal_function( arg_pack<Args2...>, Fn&& fn,  Args1&&... args1 ) const noexcept
				{
					return partial_t<sizeof...(Args2), Fn, Ret, Args1...>{
							fn,
							util::forward<Args1>(args1)...
					};
				}

				template<
					typename Ret,
					typename Fn,
					typename... Args1,
					typename... Args2
				>
				CCDK_FORCEINLINE
				auto  
				partial_function_obj( arg_pack<Args2...>, Fn&& fn,  Args1&&... args1 ) const noexcept
				{
					return partial_t<sizeof...(Args2), Fn, Ret, Args1...>{
							util::forward<Fn>(fn),
							util::forward<Args1>(args1)...
					};
				}


				
				template<
					typename Fn,
					typename... Args
				>
				CCDK_FORCEINLINE
				auto
				partial_create_impl(case_t< is_function >, Fn&& fn, Args&&... args ) const noexcept
				{
					typedef args_of<remove_ref_t<Fn>> FnArgs;
					return partial_normal_function<result_of_t<remove_ref_t<Fn>> >(
						typename FnArgs::type{},
						fn,
						util::forward<Args>(args)...
					);
				}

				//case is member function pointer, need wrap with object
				template<
					typename Fn,
					typename Fn1 = remove_ref_t<Fn>,
					typename C, typename... Args
				>
				CCDK_FORCEINLINE
				auto 
				partial_create_impl( case_t< is_mfn_ptr> , Fn&& fn, C&& c,  Args&&... args ) const noexcept
				{ 
					typedef args_of<Fn1> FnArgs;
					return partial_function_obj<result_of_t<Fn1>>(
						typename FnArgs::args{},
						fn::bind_mfn(fn, c),
						util::forward<Args>(args)...
					);
				}

				//case is function object
				template<
					typename Fn,
					typename Fn1 = remove_ref_t<Fn>,
					typename... Args
				>
				CCDK_FORCEINLINE
				auto
				partial_create_impl( case_t< is_function_obj>, Fn&& fn, Args&&... args) const noexcept
				{
					typedef args_of<decltype(&Fn1::operator())> FnArgs;
					return partial_function_obj<result_of_t<Fn1>>(
						typename FnArgs::args{},
						util::forward<Fn>(fn),
						util::forward<Args>(args)...
					);
				}


				template<
					typename Fn,
					typename Fn1 = decay_t<Fn>,
					typename = check_t< is_no_overload_invocable<Fn1> >,  //Fn can't be overload
					typename... Args
				>
				CCDK_FORCEINLINE
				auto
				operator()( Fn&& fn,  Args&&... args ) const noexcept
				{

					//static dispatch
					//Fn can be function/function pointer/member pointer/function object
					return partial_create_impl(
							select_case<
								is_function<Fn1>,
								is_mfn_ptr<Fn1>,
								is_function_obj<Fn1>>,       //can't check function object with override/template operator()
							util::forward<Fn>(fn),
							util::forward<Args>(args)...
					);
				}
			};
		}

		namespace fn
		{
			//
			constexpr fn_impl::partial_create_t partial{};
		}
	}
}