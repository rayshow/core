#pragma once

#include<exception>

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/util/forward.h>
#include<ccdk/mpl/util/move.h>

ccdk_namespace_mpl_fn_start

	struct bad_invoke_exception
		:public std::exception
	{
		const char* msg;

		bad_invoke_exception(const char* inMsg)
			:msg{ inMsg } {}

		virtual char const*
			what() const noexcept  override
		{
			return msg;
		}
	};

	template<
		typename Ret,
		typename... Args
	>
	struct invoker_base
	{
		//should be override
		virtual void set_private_data(void* data)
		{
			throw bad_invoke_exception{ "invoker::set_private_data should been override." };
		};

		virtual Ret invoke(Args...) = 0;
	};

	//T is function object
	template<
		typename T,
		typename Ret,
		typename... Args
	>
	struct function_object_invoker
		:public invoker_base<Ret, Args...>
	{
		T t;

		function_object_invoker(T&& inT)
			:t{ util::move(inT) }
		{}

		virtual Ret
			invoke(Args... args) override
		{
			return t(util::forward<Args>(args)...);
		}
	};

	//T is normal function ptr
	template<
		typename T,
		typename Ret,
		typename... Args
	>
	struct normal_function_invoker
		:public invoker_base<Ret, Args...>
	{
		T t;

		//template<typename = check< is_callable
		normal_function_invoker(T inT)
			: t(inT) {}

		virtual Ret
			invoke(Args... args) override
		{
			return t(util::forward<Args>(args)...);
		}
	};

	//T is member function ptr, P is class type 
	template<
		typename T,
		typename P,
		typename Ret,
		typename... Args
	>
	struct member_function_invoker
		:public invoker_base<Ret, Args...>
	{
		P  p;
		T* t;
		member_function_invoker(P inP)
			: p(inP),
			t(nullptr)
		{}

		virtual void
			set_private_data(void* inT) override
		{
			t = (T*)inT;
		}

		Ret __invoke_impl(Args... arg)
		{
			if (!t) throw bad_invoke_exception{
				"member_function_invoker::__invoke_impl t is nullptr." };
			return (t->*p)(util::forward<Args>(arg)...);
		}

		//is pointer
		virtual Ret
			invoke(Args... args) override
		{
			return __invoke_impl(util::forward<Args>(args)...);
		}
	};

ccdk_namespace_mpl_fn_end