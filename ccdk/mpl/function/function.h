#pragma once

#include<ccdk/type.h>
#include<ccdk/mpl/container/tuple_storage.h>
#include<ccdk/mpl/container/tuple.h>
#include<ccdk/mpl/type_traits/is_function.h>
#include<ccdk/mpl/type_traits/is_function_obj.h>
#include<ccdk/mpl/type_traits/is_mfn_ptr.h>

namespace ccdk
{
	namespace mpl
	{
		template<typename T, typename P= void>
		struct function;




		template<typename T, typename Fn>
		inline auto function_(Fn fn)
		{
			
		}

		template<typename Ret, typename... Args>
		struct function<Ret(Args...)>
		{
			typedef function<Ret(Args...)> type;
			static constexpr uint32 L = 2+sizeof...(Args);
			tuple<void> *storage;

			function() noexcept
				: storage{}
			{
				
			}

			function(ptr::nullptr_t) noexcept
				: storage{} {}

			//is function 
			template<typename Fn>
			function(Fn fn, true_, false_, false_)
				:storage { false, &fn }
			{

			}

			//is function obj
			template<typename Fn>
			function(Fn fn, false_, true_, false_)
				:storage{ false, &fn }
			{

			}

			//is mfn function
			template<typename Fn>
			function(Fn fn, false_, false_, true_)
			{

			}

			//just copy and then move it
			template<typename Fn>
			constexpr function(Fn fn)
				: function(fn,
					typename is_function<Fn>::type{},
					typename is_function_obj<Fn>::type{},
					typename is_mfn_ptr<Fn>::type{})
			{

			}

		};
	}
}