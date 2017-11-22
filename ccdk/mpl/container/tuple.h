#pragma once

#include<ccdk/mpl/base/and_.h>
#include<ccdk/mpl/base/uint_.h>
#include<ccdk/mpl/base/arg_pack.h>
#include<ccdk/mpl/base/index_sequence.h>
#include<ccdk/mpl/type_traits/forward.h>
#include<ccdk/mpl/type_traits/move.h>
#include<ccdk/mpl/type_traits/is_same.h>
#include<ccdk/mpl/type_traits/has_inner_type.h>
#include<ccdk/mpl/container/ebo.h>
#include<ccdk/mpl/base/literals.h>
#include<ccdk/type.h>


namespace ccdk
{
	namespace mpl
	{
		struct tuple_tag {};

		namespace detail
		{
			template<typename T, typename = typename T::tag> struct is_tuple_impl :false_ {};
			template<typename T> struct is_tuple_impl<T, tuple_tag> :true_ {};
		}

		template<typename T> struct is_tuple : public and_<has_inner_tag<T>, detail::is_tuple_impl<T> > {};
		template<typename T> struct is_tuple<const T> :is_tuple<T> {};
		template<typename T> struct is_tuple<T&&> :is_tuple<T> {};
		template<typename T> struct is_tuple<T&> :is_tuple<T> {};

		namespace detail
		{
			//use for dummy
			struct storage_pop_back_t {};
			struct storage_pop_front_t {};
			struct storage_insert_t {};
			constexpr storage_pop_back_t storage_pop_back{};
			constexpr storage_pop_front_t storage_pop_front{};
			constexpr storage_insert_t storage_insert{};

			template<unsigned int size, typename T, typename... Args>
			struct tuple_storage;

			template<unsigned int size, unsigned int... indice, typename... Args>
			struct tuple_storage<size, index_sequence<indice...>, Args...>:public ebo< uint_<indice>, Args>...
			{
				typedef tuple_storage<size, index_sequence<indice...>, Args...> type;
				static constexpr unsigned int L = size;

				constexpr tuple_storage() :ebo< uint_<indice>, Args >{}... {}
		
				//move
				constexpr tuple_storage(Args&&... ys) : ebo< uint_<indice>, Args >(util::move(ys) )...{}

				//copy
				constexpr tuple_storage(Args const&... ys) : ebo< uint_<indice>, Args >(ys)...{}


				//for initalize from another tuple_storage with prefect forward
				template<typename T>
				constexpr tuple_storage(T&& t) : ebo< uint_<indice>, Args>( ebo_value<uint_<indice>>(util::forward<T>(t) ) )...{}

				//merge two tuple_storage
				template<unsigned int size2, unsigned int size3, unsigned int... indice2, unsigned int... indice3, typename... Args2, typename... Args3>
				constexpr tuple_storage(
					tuple_storage<size2, index_sequence<indice2...>, Args2...> && s1,
					tuple_storage<size3, index_sequence<indice3...>, Args3...> && s2):
					ebo< uint_<indice2>, Args2>(ebo_value<uint_<indice2>>( util::move(s1)))... ,     //msvc 2017 bug to deduce
					ebo< uint_<size2+indice3>, Args3>(ebo_value<uint_<indice3>>( util::move(s2)))...
				{
					static_assert(size2 + size3 == size, "tuple storage merge size not fit");
				}

				//tuple_storage back add  another value 
				template<unsigned int size2, unsigned int... indice2, typename... Args2, typename T>
				constexpr tuple_storage(
					tuple_storage<size2, index_sequence<indice2...>, Args2...> && s, T&& t) :
					ebo< uint_<indice2>, Args2>( ebo_value<uint_<indice2>>(s))...,
					ebo< uint_<size2>, T>( util::forward<T>(t))
				{
					static_assert(size2 + 1 == size, "tuple storage push back size not fit");
				}

				//tuple_storage front add  another value 
				template<unsigned int size2, unsigned int... indice2, typename... Args2, typename T>
				constexpr tuple_storage(T&& t,
					tuple_storage<size2, index_sequence<indice2...>, Args2...> && s) :
					ebo< uint_<0>, T>(util::forward<T>(t)),
					ebo< uint_<1+indice2>, Args2>(ebo_value<uint_<indice2>>(s))...
				{
					static_assert(size2 + 1 == size, "tuple storage push front size not fit");
				}

				//move [0, indice-1)  elements of another storage to [0, indice-1) of this storage
				template<unsigned int size2, unsigned int... indice2, typename... Args2>
				constexpr tuple_storage(storage_pop_back_t, 
					tuple_storage<size2, index_sequence<indice2...>, Args2...> && s):
					ebo< uint_<indice>, Args>(ebo_value<uint_<indice>>(s))...
				{ 
					static_assert(size + 1 == size2, "tuple storage pop back size not fit");
				}

				//move [1, indice+1)  elements of another storage to [0, indice) of this storage
				template<unsigned int size2, unsigned int... indice2, typename... Args2>
				constexpr tuple_storage(storage_pop_front_t,
					tuple_storage<size2, index_sequence<indice2...>, Args2...> && s) :
					ebo< uint_<indice>, Args>(ebo_value<uint_<indice+1>>(s))...
				{
					static_assert(size + 1 == size2, "tuple storage pop back size not fit");
				}

				//
				template<unsigned int index, unsigned int... indice2, unsigned int... indice3, typename... Args2, typename... Args3, typename T, typename P>
				constexpr tuple_storage(storage_insert_t, T&& t, P&& p, 
					uint_<index>,
					index_sequence<indice2...>, index_sequence<indice3...>,
					arg_pack<Args2...>, arg_pack<Args3...>):
					ebo< uint_<index>, P>( util::move(p)),
					ebo< uint_<indice2>, Args2>(ebo_value<uint_<indice2>>(t))...,
					ebo< uint_<indice3+1>, Args3>(ebo_value<uint_<indice3>>(t))...
				{
					static_assert(sizeof...(indice2) == sizeof...(Args2), "index size need equal to Args size");
					static_assert(sizeof...(indice3) == sizeof...(Args3), "index size need equal to Args size");
				}

			};
		}
		struct tuple_push_back_t {};
		struct tuple_push_front_t {};

		template<typename... Args>
		struct tuple 
		{ 
			static constexpr unsigned int L = sizeof...(Args);
			typedef detail::tuple_storage<L, make_index_sequence<L>, Args...> value_type;
			typedef tuple<Args...> type;
			typedef tuple_tag tag;
			struct erase_t {};
			value_type storage;
			

			constexpr tuple() : storage{} {}

			//move from args
			explicit constexpr tuple(Args &&... args) : storage( util::move(args)...) {   }

			//copy from args
			explicit constexpr tuple(Args const&... args) : storage( static_cast<const Args&>(args)...) {   }

			//copy from same tuple
			constexpr tuple(tuple const& t) : storage{ t.storage }
			{ 
				static_assert(t.L == L, "2 tuple length need to be equal");
			}

			//move from same tuple
			constexpr tuple(tuple && t) : storage{ util::move(t.storage) }
			{
				static_assert(t.L == L, "2 tuple length need to be equal");
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
				static_assert(t.length() == L, "2 tuple length need to be equal");
			}

	
			//merge two tuple, note that original 2 tuple will be moved and can't use again
			template<uint32... indice1, uint32... indice2,typename... Args1, typename... Args2>
			constexpr tuple(index_sequence<indice1...>, index_sequence<indice2...>, tuple<Args1...>&& t1, tuple<Args2...>&& t2)
				: storage{ ebo_value<uint_<indice1>>(t1.storage)..., ebo_value<uint_<indice2>>(t2.storage)... }
			{
				static_assert(t1.length() + t2.length() == length(), "sum of T1 and T2's length need equal to merged tuple ");
			}


			//use to push back
			template<uint32... indice, typename... Args1, typename... Args2>
			constexpr tuple( index_sequence<indice...>, tuple<Args1...>&& tp, Args2&&... args)
				: storage{ ebo_value<uint_<indice>>( util::move(tp.storage))..., util::move(args)... }
			{
				static_assert(tp.L + sizeof...(Args2) == L, "pop back tuple and new typle length need equal");
			}

			//use to push front
			template<uint32... indice, typename... Args1, typename... Args2>
			constexpr tuple( int, index_sequence<indice...>, tuple<Args1...>&& tp, Args2&&... args)
				: storage{ util::move(args)..., ebo_value<uint_<indice>>(util::move(tp.storage))...  }
			{
				static_assert(tp.L + sizeof...(Args2) == L, "pop back tuple and new typle length need equal");
			}

			//use to erase or pop front / pop back
			template<uint32... indice, typename... Args2, uint32 len>
			constexpr tuple(index_sequence<indice...>, tuple<Args2...>&& tp, uint_<len>)
				: storage{ ebo_value<uint_<indice>>(util::move(tp.storage))... }
			{
				static_assert(tp.L - len == L, "after erase size not match");
			}



			//template<typename P, typename I, typename Is1, typename Is2, typename As1, typename As2, typename... Args2>
			//constexpr tuple(detail::storage_insert_t, tuple<Args2...>&& t1, P&& p, I i, Is1 is1, Is2 is2, As1 as1, As2 as2)
			//	: storage{ detail::storage_insert,  t1.storage, util::move(p), i, is1, is2, as1, as2}
			//{

			//}



			template<typename T, T v>
			constexpr auto& operator[](integer_<T, v> index) &
			{
				static_assert(v >= 0 && v < L, "index out of range");
				return ebo_value<uint_<v>>(storage);
			}

			template<typename T, T v>
			constexpr const auto& operator[](integer_<T, v> index) const&
			{
				static_assert(v >= 0 && v < L, "index out of range");
				return ebo_value<uint_<v>>(storage);
			}

			template<typename T, T v>
			constexpr auto&& operator[](integer_<T, v> index) &&
			{
				static_assert(v >= 0 && v < L, "index out of range");
				return ebo_value<uint_<v>>(util::move(storage));
			}
			
			//length of tuple, compiler constant
			static constexpr unsigned int length() { return L; }

			template<typename... Args1>
			constexpr auto push_back(Args1... args)
			{
				return tuple<Args..., Args1...>{ make_index_sequence<L>{}, util::move(*this), util::move(args)... };
			}

			template<typename... Args1>
			constexpr auto push_front(Args1... args)
			{
				//0 use for dummy
				return tuple<Args1... , Args... >{ 0, make_index_sequence<L>{},  util::move(*this), util::move(args)... };
			}


			//merge two tuple and create concat tuple , note both tuple will be moved for effcient and useless
			template<typename... Args2>
			constexpr auto operator|(tuple<Args2...>& tp)
			{
				typedef tuple<Args..., Args2...> new_tuple;
				return new_tuple{ make_index_sequence<L>{}, make_index_sequence<tp.L>{}, util::move(*this), util::move(tp) };
			}

			template<unsigned int index, typename T, typename... Args2, typename... Args3>
			constexpr auto __insert_impl(T&& t, arg_pack<Args2...> as1, arg_pack<Args3...> as2)
			{
				typedef tuple<Args2..., T, Args3...> new_tuple;
				typedef make_index_sequence<index> front_index;          //0 - index - 1
				typedef make_index_sequence_until<L, index> back_index;  //index ~ L - 1
				return new_tuple{ detail::storage_insert, util::move(*this), util::move(t), uint_<index>{}, front_index{}, back_index{}, as1, as2 };
			}

			template<uint32 len, typename... Args2>
			constexpr auto __pop_back_impl(arg_pack<Args2...>)
			{
				typedef tuple<Args2...> new_tuple;
				return new_tuple{ make_index_sequence<L - 1>{}, util::move(*this),  uint_<len>{} };
			}

			//dispatch to remove last type
			template<uint32 len>
			constexpr auto pop_back()
			{
				return __pop_back_impl<len>(typename arg_pack_split<L - 1, len, arg_pack<Args...>>::head{});
			}

			template<uint32 len, typename ... Args1>
			constexpr auto __pop_front_impl(arg_pack<Args1...>)
			{
				typedef tuple<Args1...> new_tuple;
				return new_tuple{ make_index_sequence_until<L,len>{}, util::move(*this),  uint_<len>{} };
			}

			//dispatch to remove last type
			template<uint32 len>
			constexpr auto pop_front()
			{
				return __pop_front_impl<len>(typename arg_pack_split<0, 1, arg_pack<Args...>>::tail{});
			}

			//
			template<unsigned int index, typename T>
			constexpr auto insert(T t)
			{
				typedef arg_pack_split<index, 0, arg_pack<Args...>> split_type;
				return __insert_impl<index>(util::move(t), typename split_type::head{}, typename split_type::tail{});
			}

			//erase elements [start, end)
			template<unsigned int start, unsigned int end>
			constexpr auto erase()
			{

			}

			template<unsigned int index, typename T>
			constexpr auto replace(T t)
			{

			}
		};

		//tuple back add a new element
		//note orginal tuple will moved to new tuple for effcient and will be  useless
		//note if T is lvalue, will do once copy and once move constructor to final storage
		//     if T is rvalue, will do two move constructor to final storage
		template<typename T, typename... Args>
		constexpr auto operator+(tuple<Args...>& tp, const T& t )
		{
			return tp.push_back(t);
		}
		//tuple front add a new element
		//note orginal tuple will moved to new tuple for effcient and will be useless
		//note if T is lvalue, will do once copy and once move constructor to final storage
		//     if T is rvalue, will do two move constructor to final storage
		template<typename T, typename... Args>
		constexpr auto operator+(const T& t, tuple<Args...>& tp)
		{
			return tp.push_front(t);
		}
		
		
	}
}