#pragma once

#include<ccdk/mpl/base/arg_pack_first.h>
#include<ccdk/mpl/type_traits/is_function.h>
#include<ccdk/mpl/type_traits/is_function_obj.h>
#include<ccdk/mpl/type_traits/is_mfn_ptr.h>
#include<ccdk/mpl/type_traits/is_pointer.h>
#include<ccdk/mpl/type_traits/is_invocable.h>
#include<ccdk/mpl/type_traits/args_of.h>
#include<ccdk/mpl/type_traits/remove_ref.h>
#include<ccdk/mpl/util/forward.h>
#include<ccdk/mpl/util/move.h>
#include<ccdk/mpl/util/swap.h>
#include<ccdk/mpl/util/addressof.h>
#include<ccdk/mpl/function/invoke_base.h>

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/function/function_fwd.h>

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
	template<typename Ret, typename... Args>
	class function<Ret(Args...)>
	{
	public:
		typedef function this_type;
		typedef invoker_base<Ret, Args...> invoke_type;
		static constexpr uint32 L = sizeof...(Args);
		template<typename> friend class function;
	private:
		invoker_base<Ret, Args...> *base_ptr;    /* derive constructor is noexcept */

		/* proxy constructor for normal function / function pointer */
		template<typename Fn>
		CCDK_FORCEINLINE function(Fn&& fn, true_, false_, false_) noexcept : base_ptr{ new(ptr::nothrow) normal_function_invoker<Fn, Ret, Args...>{ util::forward<Fn>(fn) } } { ccdk_assert(base_ptr != nullptr);  DebugValue("function: normal initalized");}

		/* proxy constructor for  function object, new may throw */
		template<typename Fn>
		CCDK_FORCEINLINE function(Fn&& fn, false_, true_, false_) noexcept : base_ptr{ new(ptr::nothrow) function_object_invoker<Fn, Ret, Args...>{ util::forward<Fn>(fn) } } { ccdk_assert(base_ptr != nullptr);  DebugValue("function: function object initalized"); }

		/* proxy constructor for member function pointer, new may throw */
		template<typename Fn> 
		CCDK_FORCEINLINE function(Fn&& fn, false_, false_, true_) noexcept : base_ptr{ new(ptr::nothrow) member_function_invoker<Fn, mfn_class_t<Fn>, Ret, Args...>{ util::forward<Fn>(fn) } } { ccdk_assert(base_ptr != nullptr);  DebugValue("function: member function initalized ");}

		/* First is class pointer, called function may throw */
		template<typename T>
		CCDK_FORCEINLINE Ret _invoke_impl_arg_type(true_, T&& t, Args... args) { base_ptr->set_private_data((void*)t); return base_ptr->invoke(util::forward<Args>(args)...); }

		/* First is class object, transform to class pointer , called function may throw*/
		template<typename T>
		CCDK_FORCEINLINE Ret _invoke_impl_arg_type(false_, T&& t, Args... args) { base_ptr->set_private_data((void*)(&(t))); return base_ptr->invoke(util::forward<Args>(args)...); }

		/* length(args1) == L, means normal function or function object, directly call , called function may throw*/
		template<typename... Args1>
		CCDK_FORCEINLINE Ret _invoke_impl_arg_len(uint_<L>, Args1&&... args1) { return base_ptr->invoke(util::forward<Args1>(args1)...); }

		/* length(args1) == L + 1, assert is member function call, then static dispatch by First arg type */
		template<typename... Args1, typename First = remove_ref_t< arg_pack_first_t<Args1...>> >
		CCDK_FORCEINLINE Ret _invoke_impl_arg_len(uint_<L + 1>, Args1&&... args1) { return _invoke_impl_arg_type(typename is_pointer<First>::type{}, util::forward<Args1>(args1)...); }

		/* clone content */
		CCDK_FORCEINLINE invoke_type* clone_pointer() noexcept { base_ptr ? base_ptr->clone() : nullptr; }
	public:
		/* default and nullptr constructor */
		CCDK_FORCEINLINE function() noexcept : base_ptr{ nullptr } {}
		CCDK_FORCEINLINE function(ptr::nullptr_t) noexcept  : base_ptr{ nullptr }  {}

		/* value constructor, call proxy constructor and static dispatch */
		template<typename Fn, typename NoRefFn = remove_ref_t<Fn>, typename = check_t< or_< is_invocable<NoRefFn>, is_mfunction<NoRefFn>> > >
		CCDK_FORCEINLINE function(Fn&& fn) noexcept : function(util::forward<Fn>(fn), typename is_function_ptr<NoRefFn>::type{}, typename or_< is_function_obj<NoRefFn>, is_mfunction<NoRefFn>>::type{}, typename is_mfn_ptr<NoRefFn>::type{}) {}

		/* copy and move  */
		CCDK_FORCEINLINE function(const function& other) noexcept : base_ptr{ other.clone_pointer() } {}
		CCDK_FORCEINLINE function(function& other) noexcept : base_ptr{ other.base_ptr } { other.base_ptr = nullptr;  }


		/*template copy and move */
		template<typename T, typename = check_t< is_convertible< function<T>, this_type>>> 
		CCDK_FORCEINLINE function(const function<T>& other) noexcept : base_ptr{ other.clone_pointer() }{}
		template<typename T, typename = check_t< is_convertible< function<T>, this_type>>>
		CCDK_FORCEINLINE function(function<T>&& other) noexcept : base_ptr{ other.base_ptr } { other.base_ptr = nullptr; }
		 
		/* swap compatible function */
		template<typename Fn, typename = check_t< is_compatible< function<Fn>, this_type>> > 
		CCDK_FORCEINLINE void swap(function<Fn>& other) noexcept { util::swap(base_ptr, other.base_ptr); }

		/* nullptr assign */
		CCDK_FORCEINLINE function& operator=(ptr::nullptr_t) noexcept { function{}.swap(*this); return *this; }

		/* value assign */
		template<typename Fn, typename = check_t< or_< is_invocable<Fn>, is_mfunction<Fn>> > >
		CCDK_FORCEINLINE function& operator=(Fn&& fn) noexcept { function{ util::forward<Fn>(fn) }.swap(*this); return *this; }

		/* copy/move assign, avoid self assign*/
		CCDK_FORCEINLINE function& operator=(const function& other) { ccdk_if_not_this(other) { function{ other }.swap(*this); } return *this; }
		CCDK_FORCEINLINE function& operator=(function&& other) { ccdk_if_not_this(other) { function{ util::move(other) }.swap(*this); } return *this; }

		/* template copy/move assign, for comvertible function */
		template<typename T, typename = check_t< is_convertible< function<T>, this_type>>>
		CCDK_FORCEINLINE function& operator=(const function<T>& other) { function{ other }.swap(*this);  return *this; }
		template<typename T, typename = check_t< is_convertible< function<T>, this_type>>>
		CCDK_FORCEINLINE function& operator=(function<T>&& other) { function{ util::move(other) }.swap(*this);  return *this; }

		/* destructor */
		CCDK_FORCEINLINE ~function() noexcept { ptr::safe_delete(base_ptr); }

		/* test invoker exists */
		CCDK_FORCEINLINE operator bool() noexcept { return !!base_ptr; }

		/* static dispatch by parameter length, assert L is normal/object function call, L + 1 is member function call  */
		template<typename... Args1>
		CCDK_FORCEINLINE Ret operator()(Args1&&... args1) { return _invoke_impl_arg_len( uint_<sizeof...(Args1)>{}, util::forward<Args1>(args1)... );  }
	}; 

ccdk_namespace_mpl_fn_end