#pragma once

#include<ccdk/type.h>
#include<ccdk/mpl/base/null_.h>
#include<ccdk/mpl/base/enable_if.h>
#include<ccdk/mpl/type_traits/decay.h>
#include<ccdk/mpl/type_traits/move.h>
#include<ccdk/mpl/function/operator.h>

namespace ccdk
{
	namespace mpl
	{
		namespace placeholder
		{
			template<typename T>
			struct placeholder_t;

			template<typename T>
			struct is_placeholder :false_ {};

			template<typename T>
			struct is_placeholder<placeholder_t<T> >:true_{};


			typedef placeholder_t<null_> terminal_holder;
			constexpr terminal_holder _{};


			//binary op
			template<typename Op, typename T1, typename T2, uint32 N == 0>
			struct operation_t
			{
			private:
				Op invoke;
				T1 t1;
				T2 t2;
			public:
				constexpr operation_t(const T1& inT1, const T2& inT2)
					: t1{ inT1 }, t2{ inT2 }
				{}

				constexpr operation_t(T1&& inT1, const T2& inT2)
					: t1{ util::move(inT1) }, t2{ inT2 }
				{}

				constexpr operation_t(const T1& inT1, T2&& inT2)
					: t1{ inT1 }, t2{ util::move(inT2) }
				{}

				template<typename...Args, typename = check< sizeof...(Args) == N>>
				constexpr operator()


			};

	
			template<typename T>
			struct placeholder_t
			{
			public:
				typedef placeholder_t<T> type;
				typedef T* value_type;
			private:
				value_type* ptr;

				placeholder_t(value_type* inPtr) noexcept
					:ptr{inPtr}
				{}

				placeholder_t(placeholder&& p) noexcept
					:ptr{ p.ptr }
				{
					p.ptr = nullptr;
				}

				~placeholder_t() 
				{
					ptr::safe_delete(ptr);
				}
			};

			template<>
			struct placeholder_t<null_>
			{
				placeholder_t() = default;
			};

			

			// (_ + 2) * (3 + _)
			template<typename T, typename U>
			auto operator+(placeholder_t<T>& p, U&& u)
			{
				typedef operation_t<1, op::add_t, placeholder_t<T>, U> operation;
				return placeholder_t< operation >{ new  operation{  } };
			}
		}
		
	}
}