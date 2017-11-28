#pragma once

#include<ccdk/type.h>
#include<ccdk/mpl/type_traits/is_function.h>
#include<ccdk/mpl/type_traits/is_function_obj.h>
#include<ccdk/mpl/type_traits/is_mfn_ptr.h>
#include<ccdk/mpl/type_traits/is_pointer.h>
#include<ccdk/mpl/type_traits/is_const.h>
#include<ccdk/mpl/type_traits/forward.h>
#include<ccdk/mpl/type_traits/move.h>
#include<ccdk/mpl/container/tuple_storage.h>
#include<ccdk/mpl/container/tuple.h>
#include<exception>

namespace ccdk
{
	namespace mpl
	{
		

		struct bad_invoke_exception:public std::exception
		{
			bad_invoke_exception(const char* msg)
				:std::exception(msg) {}
		};

		template<typename Ret, typename... Args>
		struct invoker
		{
			//should be override
			virtual void set_private_data(void* data)
			{ 
				throw bad_invoke_exception{ "invoker::set_private_data should  been override." }; 
			};
			virtual void set_private_data(const void* data)
			{
				throw bad_invoke_exception{ "invoker::set_private_data should been override." };
			}
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
			P  p;
			T* t;
			bool is_const;
			member_function_invoker(P inP) : p(inP), t(nullptr), is_const(false){}

			virtual void set_private_data(void* inT) override
			{
				t = (T*)inT;
			}

			virtual void set_private_data(const void* inT) override
			{
				is_const = true;
				t = (T*)inT;
			}
			
			Ret __invoke_impl(Args... arg)
			{
				DebugFunctionName();
				if(!t) throw bad_invoke_exception{ "member_function_invoker::__invoke_impl t is nullptr." };
				return (t->*p)(util::forward<Args>(arg)...);
			}

			//is pointer
			virtual Ret invoke(Args... args) override
			{
				return __invoke_impl(util::forward<Args>(args)...);
			}
		};

		template<typename T>
		struct function;

		//this implements of function have 3 defect
		//1. member function's owner object lost const info, so const obj can also call non-const member function
		//2. can't valid member function 's owner type is coincide with input object
		//3. parameter need careful valid
		template<typename Ret, typename... Args>
		struct function<Ret(Args...)>
		{
			typedef function<Ret(Args...)> type;
			static constexpr uint32 L = sizeof...(Args);
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
				DebugFunctionName();
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

			operator bool() noexcept
			{
				return !!fn;
			}

			//t is const pointer
			template<typename T>
			Ret __invoke_impl_arg_type(true_, true_, T&& t, Args... args)
			{
				fn->set_private_data((const void*)t);
				fn->invoke(util::forward<Args>(args)...);
			}

			//t is pointer
			template<typename T>
			Ret __invoke_impl_arg_type(true_, false_, T&& t, Args... args)
			{
				fn->set_private_data((void*)t);
				fn->invoke(util::forward<Args>(args)...);
			}

			//t is object and non-const
			template<typename T>
			Ret __invoke_impl_arg_type(false_, false_, T&& t, Args... args)
			{
				fn->set_private_data((void*)(&(t)));
				fn->invoke(util::forward<Args>(args)...);
			}

			//member function call
			template<typename... Args1>
			Ret __invoke_impl_arg_len(uint_<L + 1>, Args1&&... args1)
			{
				typedef typename remove_ref_t< arg_pack_first_t<Args1...>> first_type;
				DebugValue(typename is_pointer<first_type>::type{});
				DebugValue(typename is_const<first_type>::type{});
				DebugValue(typename is_const<remove_pointer_t<first_type>>::type{});

				return __invoke_impl_arg_type(
					typename is_pointer<first_type>::type{},
					typename is_const<remove_pointer_t<first_type>>::type{},
					util::forward<Args1>(args1)...);
			}

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

		
		namespace detail
		{
			template<typename F, typename C, typename Ret, typename... Args>
			struct member_function
			{
				F  fn;
				C *obj;
				member_function(F inFn, C* inObj) noexcept
					: fn(inFn), obj(inObj) {}

				Ret operator()(Args... args)
				{
					return (obj->*fn)(args...);
				}
			};

			template<typename F, typename C, typename Ret, typename... Args>
			auto bind_mfn_impl(F f, C* c,  mfn_args<Args...>, false_)
			{
				return member_function< F, C, Ret, Args...>(f, c);
			}

			template<typename F, typename C, typename Ret, typename... Args>
			const auto bind_mfn_impl(F f, C* c, mfn_args<Args...>, true_)
			{
				return member_function< F, C, Ret, Args...>(f,  c);
			}
		}
		


		template<typename F,  typename B = mfn_body<F>, typename C = typename B::clazz, typename = check< is_mfn_ptr_v<F>> >
		decltype(auto) bind_mfn(F f, C& c)
		{
			typedef remove_const_t<C> NC;
			return detail::bind_mfn_impl<F, NC, typename B::ret>(f,(NC*)(&c), typename B::args{}, typename is_const<C>::type{});
		}

		template<typename F, typename B = mfn_body<F>, typename C = typename B::clazz, typename = check< is_mfn_ptr_v<F>> >
		decltype(auto) bind_mfn(F f, C* c)
		{
			typedef remove_const_t<C> NC;
			return detail::bind_mfn_impl<F, NC, typename B::ret>(f, (NC*)(c), typename B::args{}, typename is_const<C>::type{});
		}

	}
}