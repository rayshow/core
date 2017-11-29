#pragma once

#include<ccdk/mpl/base/enable_if.h>
#include<ccdk/mpl/type_traits/remove_const.h>
#include<ccdk/mpl/type_traits/is_mfn_ptr.h>

namespace ccdk
{
	namespace mpl
	{
		namespace detail
		{
			template<typename F, typename C, typename Ret, typename... Args>
			struct member_function
			{
				//keep member function pointer
				F  fn;
				//keep object pointer
				C *obj; 

				member_function(const member_function&) = default;
				member_function(F inFn, C* inObj) noexcept
					: fn(inFn), obj(inObj) {}

				Ret operator()(Args... args)
				{
					return (obj->*fn)(args...);
				}
			};

			template<typename F, typename C, typename Ret, typename... Args>
			auto bind_mfn_impl(F f, C* c, mfn_args<Args...>)
			{
				return member_function< F, C, Ret, Args...>(f, c);
			}
		}

		//object style, bind object with member function
		template<typename F, typename B = mfn_body<F>, typename C = typename B::clazz, typename = check< is_mfn_ptr_v<F>> >
		auto bind_mfn(F f, C& c)
		{
			typedef remove_const_t<C> NC;
			return detail::bind_mfn_impl<F, NC, typename B::ret>(f, (NC*)(&c), typename B::args{});
		}

		//object pointer style, bind object pointer with member function
		template<typename F, typename B = mfn_body<F>, typename C = typename B::clazz, typename = check< is_mfn_ptr_v<F>> >
		auto bind_mfn(F f, C* c)
		{
			typedef remove_const_t<C> NC;
			return detail::bind_mfn_impl<F, NC, typename B::ret>(f, (NC*)(c), typename B::args{});
		}

	}
}