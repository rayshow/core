#pragma once

#include<ccdk/mpl/base/enable_if.h>
#include<ccdk/mpl/type_traits/remove_const.h>
#include<ccdk/mpl/type_traits/is_mfn_ptr.h>
#include<ccdk/mpl/type_traits/result_of.h>
#include<ccdk/mpl/type_traits/args_of.h>
#include<ccdk/mpl/util/addressof.h>

namespace ccdk
{
	namespace mpl
	{
		namespace fn_impl
		{
			template<
				typename Fn,
				typename Class,
				typename Ret,
				typename... Args
			>
			struct member_function_t
			{
				//keep member function pointer
				Fn  fn;
				//keep object pointer
				Class *clz; 

				constexpr member_function_t(const member_function_t&) = default;

				CCDK_FORCEINLINE constexpr
				member_function_t( Fn inFn, Class* inClz) noexcept
					:fn( inFn ),
					clz( inClz )
				{}

				CCDK_FORCEINLINE constexpr
				Ret operator()( Args... args ) const noexcept
				{
					return (clz->*fn)(args...);
				}
			};

			struct bind_mfn_t
			{
				template<
					typename Fn,
					typename Class,
					typename Ret,
					typename... Args
				>
				CCDK_FORCEINLINE constexpr
				auto 
				__bind_mfn_impl( Fn fn, Class* clz, arg_pack<Args...> ) const noexcept
				{
					return member_function_t< Fn, Class, Ret, Args...>(fn, clz);
				}

				template<
					typename Fn,
					typename = check_t< is_mfn_ptr<Fn>>,
					typename FnArgs = args_of<Fn>,
					typename Class = typename FnArgs::clazz
				>
				CCDK_FORCEINLINE constexpr
				auto 
				operator()( Fn fn, Class& clz )const noexcept
				{
					typedef remove_const_t<Class> NonConstClass;
					return __bind_mfn_impl<Fn, NonConstClass, result_of_t<Fn> >(
						fn, 
						(NonConstClass*)(util::addressof(clz)),
						typename FnArgs::args{}
					);
				}

				template<
					typename Fn,
					typename = check_t< is_mfn_ptr<Fn>>,
					typename FnArgs = args_of<Fn>,
					typename Class = typename FnArgs::clazz
				>
				CCDK_FORCEINLINE constexpr
				auto 
				operator()( Fn fn, Class* clz ) const noexcept
				{
					typedef remove_const_t<Class> NoConstClass;
					return __bind_mfn_impl<Fn, NoConstClass, result_of_t<Fn> >(
						fn, (NoConstClass*)clz,
						typename FnArgs::args{}
					);
				}
			};
		}

		namespace fn
		{
			//bind_mfn is function object
			constexpr fn_impl::bind_mfn_t bind_mfn{};
		}
		
	}
}