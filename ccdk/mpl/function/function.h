#pragma once

#include<ccdk/type.h>
#include<ccdk/mpl/container/tuple_storage.h>
#include<ccdk/mpl/container/tuple.h>
#include<ccdk/mpl/type_traits/is_function.h>
#include<ccdk/mpl/type_traits/is_function_obj.h>
#include<ccdk/mpl/type_traits/is_mfn_ptr.h>
#include<ccdk/mpl/type_traits/forward.h>
#include<ccdk/mpl/type_traits/move.h>


namespace ccdk
{
	namespace mpl
	{
		template<typename Ret, typename... Args>
		struct callable
		{
			virtual Ret invoke(Args...)=0;
		};

		//T is function object
		template<typename T, typename Ret, typename... Args>
		struct function_object_callable :public callable<Ret, Args...>
		{
			T t;

			function_object_callable(T&& inT) :t{ util::move(inT) } {}

			virtual Ret invoke(Args&&... args) override
			{
				return t(util::forward<Args>(args...));
			}
		};

		//T is normal function prototype
		template<typename T, typename Ret, typename... Args>
		struct normal_function_callable :public callable<Ret, Args...>
		{
			T* t;

			normal_function_callable(const T* inT) : t(inT) {}

			virtual Ret invoke(Args&&... args) override
			{
				return t(util::forward<Args>(args...));
			}
		};

		//T is member function, P is object 
		template<typename T, typename P, typename Ret, typename... Args>
		struct member_function_callable :public callable<Ret, Args...>
		{
			T* t;
			member_function_callable(const T* inT) : t(inT) {}

			template<typename... Args1>
			Ret __invoke_impl(const P* p, Args1&&... args1)
			{
				return (p->*t)(util::forward<Args1>(args1...));
			}

			template<typename... Args1>
			Ret __invoke_impl(const P& p, Args1&&... args1)
			{
				return (p.*t)(util::forward<Args1>(args1...));
			}

			//is pointer
			virtual Ret invoke(Args&&... args) override
			{
				return __invoke_impl(util::forward<Args...>(args)...);
			}
		};

		template<typename T>
		struct function;


		template<typename Ret, typename... Args>
		struct function<Ret(Args...)>
		{
			typedef function<Ret(Args...)> type;
			static constexpr uint32 L = 2+sizeof...(Args);
			callable<Ret, Args...> *fn;

			function() noexcept : fn{nullptr} {}

			function(ptr::nullptr_t) noexcept : fn{ nullptr } {}
			
			template<typename Fn>
			function(Fn fn, true_, false_, false_) noexcept
				:fn(new(ptr::nothrow) normal_function_callable<Fn, Ret, Args...>{ fn })
			{}

			//is function obj
			template<typename Fn>
			function(Fn&& fn, false_, true_, false_) noexcept
				:fn{ new(ptr::nothrow) function_object_callable<Fn, Ret, Args...>{ util::move(fn) } }
			{}

			//is mfn function ptr
			template<typename Fn>
			function(Fn fn, false_, false_, true_) noexcept
				:fn{ new(ptr::nothrow) member_function_callable<Fn,Fn,Ret,Args...>{ fn } }
			{}

			//just copy and then move it, call proxy constructor
			template<typename Fn>
			function(Fn fn) noexcept
				: function( util::move(fn),
					typename is_function<Fn>::type{},
					typename is_function_obj<Fn>::type{},
					typename is_mfn_ptr<Fn>::type{})
			{}

		};
	}
}