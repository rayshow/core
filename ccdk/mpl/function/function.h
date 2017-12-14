#pragma once

#include<exception>

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/arg_pack_first.h>
#include<ccdk/mpl/type_traits/is_function.h>
#include<ccdk/mpl/type_traits/is_function_obj.h>
#include<ccdk/mpl/type_traits/is_mfn_ptr.h>
#include<ccdk/mpl/type_traits/is_pointer.h>
#include<ccdk/mpl/type_traits/is_const.h>
#include<ccdk/mpl/util/forward.h>
#include<ccdk/mpl/util/move.h>
#include<ccdk/mpl/fusion/tuple_storage.h>
#include<ccdk/mpl/fusion/tuple.h>
#include<ccdk/mpl/function/invoke_interface.h>

ccdk_namespace_mpl_fn_start

	//this implements of function have some defects
	//1. member function's owner object lost const info, so const obj can also call non-const member function
	//2. can't valid member function 's owner type is coincide with input object
	//3. parameter need careful valid
	//4. return need check
	// also some merits
	//1. universal interface
	//2. lighter and effecient then std::function, only 2-3 pointer, a virtual call consume
	//3. clear then std::function
	template<typename T,typename... Args>
	struct function
			:public function<T>, public function<Args...>
	{
		using function<T>::operator();
		using function<Args...>::operator();

	};


	template<typename Ret, typename... Args>
	struct function<Ret(Args...)>
	{
		typedef function type;
		static constexpr uint32 L = sizeof...(Args);

		invoker_base<Ret, Args...> *fn;

		CCDK_FORCEINLINE
		function() noexcept 
			: fn{ nullptr }
		{}

		CCDK_FORCEINLINE
		function(ptr::nullptr_t) noexcept 
			: fn{ nullptr } 
		{}

		CCDK_FORCEINLINE
		~function() { ptr::safe_delete(fn); }

		//is normal function / function pointer
		template<typename Fn>
		CCDK_FORCEINLINE
		function(Fn&& fn, true_, false_, false_) noexcept
			: fn( new(ptr::nothrow) normal_function_invoker<Fn, Ret, Args...>{ fn } )
		{
			DebugValue("function: normal initalized");
		}

		//is function obj
		template<typename Fn>
		function(Fn&& fn, false_, true_, false_) noexcept
			:fn{ new(ptr::nothrow) function_object_invoker<Fn, Ret, Args...>{ util::move(fn) } }
		{
			DebugValue("function: function object  initalized");
		}

		//is mfn function ptr
		template<typename Fn>
		CCDK_FORCEINLINE
		function( Fn&& fn, false_, false_, true_ ) noexcept
			:fn{ new(ptr::nothrow) member_function_invoker< mfn_class_t<Fn>,Fn,Ret,Args...>{ fn } }
		{
			DebugValue("function: member function initalized ");
		}

		//call proxy constructor and static dispatch
		template<typename Fn>
		CCDK_FORCEINLINE
		function(Fn fn) noexcept
			: function(
				util::move(fn),
				typename is_function_ptr<Fn>::type{},
				typename is_function_obj<Fn>::type{},
				typename is_mfn_ptr<Fn>::type{})
		{}


		CCDK_FORCEINLINE
		operator bool() noexcept
		{
			return !!fn;
		}

		//t is const pointer
		template<typename T>
		CCDK_FORCEINLINE
		Ret __invoke_impl_arg_type(true_, T&& t, Args... args)
		{
			fn->set_private_data((void*)t);
			return fn->invoke( util::forward<Args>(args)... );
		}

		//t is object 
		template<typename T>
		CCDK_FORCEINLINE
		Ret __invoke_impl_arg_type(false_, T&& t, Args... args)
		{
			fn->set_private_data((void*)(&(t)));
			return fn->invoke( util::forward<Args>(args)... );
		}

		//member function call
		template<typename... Args1>
		CCDK_FORCEINLINE
		Ret __invoke_impl_arg_len(uint_<L + 1>, Args1&&... args1)
		{
			typedef remove_ref_t< arg_pack_first_t<Args1...>> first_type;
			return __invoke_impl_arg_type(
				typename is_pointer<first_type>::type{},
				util::forward<Args1>(args1)...
			);
		}

		//normal function or function object
		template<typename... Args1>
		CCDK_FORCEINLINE
		Ret __invoke_impl_arg_len(uint_<L>, Args1&&... args1)
		{
			return fn->invoke( util::forward<Args1>(args1)... );
		}

		template<typename... Args1>
		CCDK_FORCEINLINE
		Ret operator()(Args1... args1)
		{
			//dispatch by parameter length
			return __invoke_impl_arg_len(
				uint_<sizeof...(Args1)>{},
				util::forward<Args1>(args1)...
			);
		}
	};  //function 

ccdk_namespace_mpl_fn_end