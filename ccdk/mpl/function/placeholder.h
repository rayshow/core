#pragma once

#include<ccdk/type.h>
#include<ccdk/mpl/base/null_.h>
#include<ccdk/mpl/base/uint_.h>
#include<ccdk/mpl/base/enable_if.h>
#include<ccdk/mpl/type_traits/decay.h>
#include<ccdk/mpl/type_traits/move.h>
#include<ccdk/mpl/container/ref_tuple.h>
#include<ccdk/mpl/function/operator.h>

namespace ccdk
{
	namespace mpl
	{
		namespace placeholder
		{
			//declare
			template<uint32 N, typename Op, typename F, typename...>
			struct operation_t;

			template<typename T>
			struct placeholder_t;

			template<typename T>
			struct is_placeholder :false_ {};

			template<typename T>
			struct is_placeholder<placeholder_t<T> > :true_ {};


			// implements

			//binary op, N is placeholder number of expression tree 
			template<uint32 N, typename Op, typename T1, typename T2 >
			struct operation_t<N, Op, T1, T2>
			{
			public:
				static constexpr uint32 L = N;
				typedef operation_t<N, Op, T1, T2> type;
			private:
				Op fn;
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

				constexpr operation_t(T1&& inT1, T2&& inT2)
					: t1{ util::move(inT1) }, t2{ util::move(inT2) }
				{}


				//zero placeholder, just invoke and get result
				template<uint32 S, typename T>
				constexpr decltype(auto)
					__invoke_impl(
						uint_<0>,
						false_,
						false_,
						const T& t 
					) const
				{
					return fn(t1, t2);
				}


				// C >= 1 , left right all is placeholder
				template<uint32 S, uint32 C, typename T>
				constexpr decltype(auto)
					__invoke_impl(
						uint_<C>,
						true_,
						true_,	
						const T& t
					) const
				{
					return fn( t1.template invoke<S>(t), t2.template invoke<S+T1::L>(t) );
				}

				// C >= 1 , left is placeholder
				template<uint32 S, uint32 C, typename T>
				constexpr decltype(auto)
					__invoke_impl(
						uint_<C>,
						true_,
						false_,
						const T& t
					) const
				{
					return fn(t1.template invoke<S>(t), t2);
				}

				// C >= 1 , right is placeholder
				template<
					uint32 S,
					uint32 C,
					typename T
					>
				constexpr decltype(auto)
					__invoke_impl(
						uint_<C>,
						false_,
						true_,
						const T& t
					) const
				{
					return fn(t1, t2.template invoke<S>(t)) ;
				}

				template<
					uint32 S,
					typename T,
					typename = check_t< is_ref_tuple<T> >
					>
					 decltype(auto) 
					invoke( const T& t ) const
				{
					//static dispatch by N and T1,T2 type
					return __invoke_impl<S>(
						uint_<L>{},
						typename is_placeholder<T1>::type{},
						typename is_placeholder<T2>::type{},
						t
					);
				}
			};

	
			template<typename T>
			struct placeholder_t
			{
			public:
				typedef placeholder_t<T> type;
				typedef T* value_type;
				static constexpr uint32  L = T::L;
			private:
				value_type ptr;
			public:
				placeholder_t(value_type inPtr) noexcept
					:ptr(inPtr)
				{}

				placeholder_t(placeholder_t&& p) noexcept
					:ptr{ p.ptr }
				{
					p.ptr = nullptr;
				}

				//just dispatch ptr's invoke
				template<
					uint32 S,
					typename U
					>
				 decltype(auto)
					invoke(
						const U& t
					) const 
				{
					return  ptr->template invoke<S>(t);
				}


				//enter point
				template<
					typename... Args, 
					typename = check< sizeof...(Args) == L >
				>
					decltype(auto)
					operator()(
						Args&&... args
						) const
				{
					//created tuple is right-ref
					return ptr->template invoke<0>(create_ref_tuple(args...));
				}


				~placeholder_t() 
				{
					ptr::safe_delete(ptr);
				}
			};

			template<>
			struct placeholder_t<null_>
			{
				static constexpr uint32 L = 1;

				template<
					uint32 S,
					typename U
				  >
				  decltype(auto)
					invoke(
						U& t
					) const noexcept
				{
					return t.template at<S>();
				}

			};

			// (_ + 2) * (3 + _)
			template<
				typename T,
				typename U,
				typename = check_t< not_<is_placeholder<U>>>
			>
			auto operator+(const placeholder_t<T>& p, U&& u)
			{
				DebugValue("left _");
				typedef operation_t< p.L , op::add_t, placeholder_t<T>, U> operation;
				return placeholder_t< operation >{ new  operation{ const_cast<placeholder_t<T>&&>(p), util::forward<U>(u) } };
			}

			template<
				typename T,
				typename U,
				typename = check_t< not_<is_placeholder<U>>>
			>
			auto operator+(U&& u, const placeholder_t<T>& p)
			{
				DebugValue("right _");
				typedef operation_t< p.L, op::add_t, U, placeholder_t<T>> operation;
				return placeholder_t< operation >{ new  operation{ util::forward<U>(u), const_cast<placeholder_t<T>&&>(p) } };
			}
			
			template<
				typename T,
				typename U
			>
			auto operator+(const placeholder_t<T>& t, const placeholder_t<U>& u)
			{
				DebugValue("2 _");
				typedef operation_t< t.L + u.L , op::add_t, placeholder_t<T>, placeholder_t<U>> operation;
				return placeholder_t< operation >{ new  operation{ const_cast<placeholder_t<T>&&>(t), const_cast<placeholder_t<U>&&>(u) } };
			}

			typedef placeholder_t<null_> terminal_holder;
			constexpr terminal_holder _{};
		}
		
	}
}