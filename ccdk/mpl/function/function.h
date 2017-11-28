#pragma once

#include<ccdk/type.h>
#include<ccdk/mpl/type_traits/is_function.h>
#include<ccdk/mpl/type_traits/is_function_obj.h>
#include<ccdk/mpl/type_traits/is_mfn_ptr.h>
#include<ccdk/mpl/type_traits/forward.h>
#include<ccdk/mpl/type_traits/move.h>
#include<ccdk/mpl/container/tuple_storage.h>
#include<ccdk/mpl/container/tuple.h>
#include<exception>

namespace ccdk
{
	namespace mpl
	{
		template<typename Ret, typename... Args>
		struct invoker
		{
			virtual Ret invoke(Args...)=0;
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
			P p;
			member_function_invoker(P inP) : p(inP) {}

			template<typename... Args1>
			Ret __invoke_impl(T* t, Args1&&... args1)
			{
				return (t->*p)(util::forward<Args1>(args1)...);
			}

			template<typename... Args1>
			Ret __invoke_impl(T& t, Args1&&... args1)
			{
				DebugFunctionName();
				return (t.*p)(util::forward<Args1>(args1)...);
			}

			//is pointer
			virtual Ret invoke(T&& t, Args... args) override
			{
				return __invoke_impl(t, util::forward<Args>(args)...);
			}
		};

		template<typename T>
		struct function;


		template<typename Ret, typename... Args>
		struct function<Ret(Args...)>
		{
			typedef function<Ret(Args...)> type;
			static constexpr uint32 L = 2+sizeof...(Args);
			invoker<Ret, Args...> *fn;

			function() noexcept : fn{nullptr} {}

			function(ptr::nullptr_t) noexcept : fn{ nullptr } {}

			~function() { CCDK_SAFE_DELETE(fn); }
			
			template<typename Fn>
			function(Fn&& fn, true_, false_, false_) noexcept
				:fn(new(ptr::nothrow) normal_function_invoker<Fn, Ret, Args...>{ fn })
			{
				DebugValue("function: normal ");
			}

			//is function obj
			template<typename Fn>
			function(Fn&& fn, false_, true_, false_) noexcept
				:fn{ new(ptr::nothrow) function_object_invoker<Fn, Ret, Args...>{ util::move(fn) } }
			{
				DebugValue("function: object ");
			}

			//is mfn function ptr
			template<typename Fn>
			function(Fn&& fn, false_, false_, true_) noexcept
				:fn{ new(ptr::nothrow) member_function_invoker<mfn_class_t<Fn>,Fn,Ret,Args...>{ fn } }
			{
				DebugValue("function: member ");
			}

			//just copy and then move it, call proxy constructor
			template<typename Fn>
			function(Fn fn) noexcept
				: function( util::move(fn),
					typename is_function_ptr<Fn>::type{},
					typename is_function_obj<Fn>::type{},
					typename is_mfn_ptr<Fn>::type{})
			{}

			operator bool()
			{
				return !!fn;
			}

			Ret operator()(Args... args)
			{
				return fn->invoke(util::forward<Args>(args)...);
			}

			template<typename T>
			Ret operator()(T&& t, Args... args)
			{

			}
		};
	}
}