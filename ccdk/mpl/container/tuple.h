#pragma once

#include<ccdk/type.h>
#include<ccdk/mpl/base/and_.h>
#include<ccdk/mpl/base/uint_.h>
#include<ccdk/mpl/type_traits/decay.h>
#include<ccdk/mpl/type_traits/is_same.h>
#include<ccdk/mpl/type_traits/has_inner_type.h>
#include<ccdk/mpl/util/forward.h>
#include<ccdk/mpl/util/move.h>
#include<ccdk/mpl/container/tuple_storage.h>
#include<ccdk/mpl/base/arg_pack_split.h>
#include<ccdk/mpl/base/make_indice.h>


namespace ccdk
{
	namespace mpl
	{
		struct tuple_tag {};


		template<typename... Args>
		struct tuple 
		{ 
			static constexpr unsigned int L = sizeof...(Args);
			typedef tuple_storage<L, make_indice<L>, decay_t<Args>...> value_type;
			typedef tuple<Args...> type;
			typedef tuple_tag tag;
			struct erase_t {};


			value_type storage;
			
			constexpr tuple() : storage{} {}

			//move from args
			explicit constexpr tuple(Args &&... args) 
				: storage( util::move(args)...) {}

			//copy from args
			explicit constexpr 
				tuple(Args const&... args) 
				: storage( static_cast<const Args&>(args)...) {}

			//copy from same tuple
			constexpr tuple(tuple const& t) 
				: storage{ t.storage }
			{ 
				static_assert(t.L == L, 
					"2 tuple length need to be equal");
			}

			//move from same tuple
			constexpr tuple(tuple && t) 
				: storage{ util::move(t.storage) }
			{
				static_assert(t.L == L, 
					"2 tuple length need to be equal");
			}
			
			//copy from not same but compatable tuple
			template<typename... Args2>
			constexpr tuple(const tuple<Args2...>& t)
				: storage{ util::move(t.storage) }
			{ 
				static_assert(t.L == L, "2 tuple length need to be equal");
			}

			//move from not same but compatable tuple
			template<typename... Args2>
			constexpr tuple(tuple<Args2...>&& t) 
				: storage{ util::move(t.storage) }
			{
				static_assert(t.L == L,
					"2 tuple length need to be equal");
			}

	
			//merge two tuple, note that original 2 tuple will be moved and can't use again
			template<
				uint32... indice1,
				uint32... indice2,
				typename... Args1,
				typename... Args2
			>
			constexpr tuple(
				indice_pack<indice1...>, 
				indice_pack<indice2...>, 
				tuple<Args1...>&& t1,
				tuple<Args2...>&& t2)
				: storage{ ebo_at<indice1>(t1.storage)...,
						   ebo_at<indice2>(t2.storage)... }
			{
				static_assert(t1.L + t2.L == L,
					"sum of T1 and T2's length need equal to merged tuple ");
			}


			//use to push back
			template<
				uint32... indice,
				typename... Args1,
				typename... Args2
			>
			constexpr tuple(
				indice_pack<indice...>,
				tuple<Args1...>&& tp,
				Args2&&... args)
				: storage{ 
					ebo_at<indice>( 
						util::move(tp.storage))...,
						util::move(args)... }
			{
				static_assert(tp.L + sizeof...(Args2) == L,
					"pop back tuple and new typle length need equal");
			}

			//use to push front
			template<
				uint32... indice,
				typename... Args1,
				typename... Args2
			>
			constexpr tuple( 
				int, 
				indice_pack<indice...>,
				tuple<Args1...>&& tp,
				Args2&&... args)
				: storage{ util::move(args)...,
					ebo_at<indice>(util::move(tp.storage))...  }
			{
				static_assert(tp.L + sizeof...(Args2) == L,
					"pop back tuple and new typle length need equal");
			}

			//use to erase or pop front / pop back
			template<uint32... indice, typename... Args2, uint32 len>
			constexpr tuple(indice_pack<indice...>,
				tuple<Args2...>&& tp, 
				uint_<len>)
				: storage{ ebo_at<indice>(util::move(tp.storage))... }
			{
				static_assert(tp.L - len == L,
					"after erase size not match");
			}

			//use to replace and insert 
			template<
				uint32... indice1,
				uint32... indice2,
				uint32... indice3,
				typename Tp,
				typename... Args1
			>
			constexpr 
				tuple(
				indice_pack<indice1...>,
				indice_pack<indice2...>,
				indice_pack<indice3...>,
				Tp&& tp,
				Args1&&... args )
				: storage{ 
					indice_pack<indice1...,indice2...>{},
					ebo_at<indice3>(util::move(tp.storage))...,
					util::move(args)...  }
			{
				static_assert(sizeof...(indice1)+sizeof...(indice2) == L,
					"replace or insert length not fit");
			}

			template<typename T, T v>
			constexpr auto& operator[](integer_<T, v> index) &
			{
				static_assert(v >= 0 && v < L, "index out of range");

				return ebo_at<v>(storage);
			}

			template<typename T, T v>
			constexpr const auto& operator[](integer_<T, v> index) const&
			{
				static_assert(v >= 0 && v < L, "index out of range");

				return ebo_at<v>(storage);
			}

			template<typename T, T v>
			constexpr auto&& operator[](integer_<T, v> index) &&
			{
				static_assert(v >= 0 && v < L, "index out of range");

				return ebo_at<v>(util::move(storage));
			}
			
			//length of tuple, compiler constant
			static constexpr uint32
				length() { return L; }

			template<typename... Args1>
			constexpr auto push_back(Args1... args)
			{
				return tuple<
					Args...,
					Args1...>{ 
						make_indice<L>{},
						util::move(*this),
						util::move(args)...
				};
			}

			template<typename... Args1>
			constexpr auto push_front(Args1... args)
			{
				return tuple<
					Args1... ,
					Args... >{ 
						0,
						make_indice<L>{},
						util::move(*this),
						util::move(args)...
				};
			}

			//merge two tuple and create concat tuple , note both tuple will be moved for effcient and useless
			template<typename... Args2>
			constexpr auto 
				operator|(tuple<Args2...>& tp)
			{
				return tuple<
					Args...,
					Args2...>{ 
						make_indice<L>{},
						make_indice<tp.L>{},
						util::move(*this),
						util::move(tp) 
				};
			}

			template<
				uint32 len,
				typename... Args2
			>
			constexpr auto 
				__pop_back_impl(arg_pack<Args2...> t)
			{
				return tuple<Args2...>{ 
						make_indice<L-len>{}, 
						util::move(*this), 
						uint_<len>{}
				};
			}

			//dispatch to remove last len elements
			template<uint32 len=1>
			constexpr auto 
				pop_back()
			{
				static_assert(len >= 1 && len < L, "tuple pop out of range");

				return __pop_back_impl<len>(
					typename arg_pack_split<
						L - len,
						len, 
						arg_pack<Args...>
					>::head{}
				);
			}

			template<
				uint32 len,
				typename ... Args1
			>
			constexpr auto 
				__pop_front_impl(arg_pack<Args1...>)
			{
				return tuple<Args1...>{ 
					make_indice_from<len,L>{},
					util::move(*this),
					uint_<len>{} 
				};
			}

			//dispatch to remove last type
			template<uint32 len = 1>
			constexpr auto 
				pop_front()
			{
				static_assert(len >= 1 && len < L, "tuple pop out of range");

				return __pop_front_impl<len>(
					typename arg_pack_split<
						0,
						len,
						arg_pack<Args...>
					>::tail{}
				);
			}

			template<uint32 start, uint32 end, typename... Args1>
			constexpr auto 
				__erase_impl(arg_pack<Args1...>)
			{
				return tuple<Args1...>{
					make_indice_ingore<L,start, end>{},
					util::move(*this),
					uint_<end-start>{}
				};
			}

			//erase elements [start, end)
			template<
				uint32 start, 
				uint32 end = start+1
			>
			constexpr auto 
				erase()
			{
				static_assert(start >= 0 && end <= L, "tuple erase out of range");
				static_assert(end > start, "erase end need greater then start");
				return __erase_impl<start, end>(
					typename arg_pack_split<start, end - start, arg_pack<Args...>>::type{}
				);
			}

			template<
				uint32 start,
				uint32 end,
				uint32 len,
				typename... Args1,
				typename... Args2,
				typename... Args3>
			constexpr auto 
				__replace_impl(
					arg_pack<Args1...>,
					arg_pack<Args3...>,
					Args2&&... args
				)
			{
				return tuple<
					Args1...,
					Args2..., 
					Args3...>
				{ 
					make_indice_ingore<L-(end-start)+len, start, end>{},
					make_indice_from< start, end>{},
					util::move(*this), util::move(args)... 
				};
			}

			template<
				uint32 start,
				uint32 end ,
				typename T,
				typename... Args1
			>
			constexpr auto 
				replace(
					T t,
					Args1... args
				)
			{
				typedef arg_pack_split<
					start, 
					end-1,
					arg_pack<Args...>
				> pack;

				return __replace_impl<start, end>(
					typename pack::head{}, 
					typename pack::tail{}, 
					util::move(args)...
				);
			}

			template<
				uint32 start,
				uint32 end,
				typename... Args1,
				typename... Args2,
				typename... Args3
			>
			constexpr auto 
				__insert_impl(
					arg_pack<Args1...>,
					arg_pack<Args3...>,
					Args2&&... args
				)
			{
				return tuple<
					Args1...,
					Args2...,
					Args3...>
				{
					make_indice_ingore<L + end-start, start, end>{},
					make_indice_from<start, end>{},
					make_indice<L>{},
					util::move(*this),
					util::move(args)...
				};
			}

			template<
				uint32 start,
				typename... Args1
			>
			constexpr auto 
				insert(Args1... args)
			{
				typedef arg_pack_split<start, 0, arg_pack<Args...>> pack;
				return __insert_impl<
					start,
					start+ sizeof...(Args1)>
					(
						typename pack::head{},
						typename pack::tail{},
						util::move(args)...
					);
			}
		};

		template<
			typename T,
			typename... Args
		>
		constexpr auto
			operator+(
				tuple<Args...>& tp,
				const T& t 
			)
		{
			return tp.push_back(t);
		}

		template<
			typename T,
			typename... Args
		>
		constexpr auto 
			operator+(
				const T& t, 
				tuple<Args...>& tp
			)
		{
			return tp.push_front(t);
		}
	}
}