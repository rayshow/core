#pragma once

#include<exception>

#include<ccdk/type.h>
#include<ccdk/mpl/base/arg_pack_first.h>
#include<ccdk/mpl/type_traits/is_function.h>
#include<ccdk/mpl/type_traits/is_function_obj.h>
#include<ccdk/mpl/type_traits/is_mfn_ptr.h>
#include<ccdk/mpl/type_traits/is_pointer.h>
#include<ccdk/mpl/type_traits/is_const.h>
#include<ccdk/mpl/type_traits/forward.h>
#include<ccdk/mpl/type_traits/move.h>
#include<ccdk/mpl/container/tuple_storage.h>
#include<ccdk/mpl/container/tuple.h>


namespace ccdk
{
	namespace mpl
	{
		namespace function_detail
		{
			struct bad_invoke_exception :public std::exception
			{
				const char* msg;

				bad_invoke_exception(const char* inMsg)
					:msg{ inMsg } {}

				virtual char const* what() const noexcept  override
				{
					return msg;
				}
			};

			template<typename Ret, typename... Args>
			struct invoker
			{
				//should be override
				virtual void set_private_data(void* data)
				{
					throw bad_invoke_exception{ "invoker::set_private_data should been override." };
				};

				virtual Ret invoke(Args...) = 0;
			};

			//T is function object
			template<typename T, typename Ret, typename... Args>
			struct function_object_invoker
				:public invoker<Ret, Args...>
			{
				T t;
				function_object_invoker(T&& inT) :t{ util::move(inT) } {}

				virtual Ret invoke(Args... args) override
				{
					return t(util::forward<Args>(args)...);
				}
			};

			//T is normal function ptr
			template<typename T, typename Ret, typename... Args>
			struct normal_function_invoker
				:public invoker<Ret, Args...>
			{
				T t;

				//template<typename = check< is_callable
				normal_function_invoker(T inT) : t(inT) {}

				virtual Ret invoke(Args... args) override
				{
					return t(util::forward<Args>(args)...);
				}
			};

			//T is member function ptr, P is class type 
			template<typename T, typename P, typename Ret, typename... Args>
			struct member_function_invoker :public invoker<Ret, Args...>
			{
				P  p;
				T* t;
				member_function_invoker(P inP) : p(inP), t(nullptr) {}

				virtual void set_private_data(void* inT) override
				{
					t = (T*)inT;
				}

				Ret __invoke_impl(Args... arg)
				{
					if (!t) throw bad_invoke_exception{ "member_function_invoker::__invoke_impl t is nullptr." };
					return (t->*p)(util::forward<Args>(arg)...);
				}

				//is pointer
				virtual Ret invoke(Args... args) override
				{
					return __invoke_impl(util::forward<Args>(args)...);
				}
			};
		}

		//this implements of function have some defects
		//1. member function's owner object lost const info, so const obj can also call non-const member function
		//2. can't valid member function 's owner type is coincide with input object
		//3. parameter need careful valid
		//4. return need check
		// also some merits
		//1. universal interface
		//2. lighter and effecient then std::function, only 2-3 pointer, a virtual call consume
		//3. clear then std::function
		template<typename T>
		struct function;

		template<typename Ret, typename... Args>
		struct function<Ret(Args...)>
		{
			typedef function<Ret(Args...)> type;
			static constexpr uint32 L = sizeof...(Args);
			function_detail::invoker<Ret, Args...> *fn;

			function() noexcept : fn{ nullptr } {}

			function(ptr::nullptr_t) noexcept : fn{ nullptr } {}

			~function() { CCDK_SAFE_DELETE(fn); }

			template<typename Fn>
			function(Fn&& fn, true_, false_, false_) noexcept
				: fn(new(ptr::nothrow) function_detail::normal_function_invoker<
					Fn, Ret, Args...>{ fn })
			{
				DebugValue("function: normal ");
			}

			//is function obj
			template<typename Fn>
			function(Fn&& fn, false_, true_, false_) noexcept
				:fn{ new(ptr::nothrow) function_detail::function_object_invoker<
					Fn, Ret, Args...>{ util::move(fn) } }
			{
				DebugValue("function: object ");
			}

			//is mfn function ptr
			template<typename Fn>
			function(Fn&& fn, false_, false_, true_) noexcept
				:fn{ new(ptr::nothrow) function_detail::member_function_invoker<
					mfn_class_t<Fn>,Fn,Ret,Args...>{ fn } }
			{
				DebugValue("function: member ");
			}

			//just copy and then move it, call proxy constructor
			template<typename Fn>
			function(Fn fn) noexcept
				: function(util::move(fn),
					typename is_function_ptr<Fn>::type{},
					typename is_function_obj<Fn>::type{},
					typename is_mfn_ptr<Fn>::type{})
			{}

			operator bool() noexcept
			{
				return !!fn;
			}

			//t is const pointer
			template<typename T>
			Ret __invoke_impl_arg_type(true_, T&& t, Args... args)
			{
				fn->set_private_data((void*)t);
				return fn->invoke(util::forward<Args>(args)...);
			}

			//t is object 
			template<typename T>
			Ret __invoke_impl_arg_type(false_, T&& t, Args... args)
			{
				fn->set_private_data((void*)(&(t)));
				return fn->invoke(util::forward<Args>(args)...);
			}

			//member function call
			template<typename... Args1>
			Ret __invoke_impl_arg_len(uint_<L + 1>, Args1&&... args1)
			{
				typedef remove_ref_t< arg_pack_first_t<Args1...>> first_type;
				return __invoke_impl_arg_type(
					typename is_pointer<first_type>::type{},
					util::forward<Args1>(args1)...);
			}

			//normal function or  function object
			template<typename... Args1>
			Ret __invoke_impl_arg_len(uint_<L>, Args1&&... args1)
			{
				return fn->invoke(util::forward<Args1>(args1)...);
			}

			template<typename... Args1>
			Ret operator()(Args1... args1)
			{
				//dispatch by parameter length
				return __invoke_impl_arg_len(uint_<sizeof...(Args1)>{}, util::forward<Args1>(args1)...);
			}
		};

	}
}