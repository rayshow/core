#pragma once

#include<ccdk/mpl/base/and_.h>
#include<ccdk/mpl/base/uint_.h>
#include<ccdk/mpl/base/index_sequence.h>
#include<ccdk/mpl/type_traits/forward.h>
#include<ccdk/mpl/type_traits/move.h>
#include<ccdk/mpl/type_traits/is_same.h>
#include<ccdk/mpl/type_traits/has_inner_type.h>
#include<ccdk/mpl/container/ebo.h>


namespace ccdk
{
	namespace mpl
	{
		struct storage_move {};
		struct storage_merge {};
		struct tuple_tag {};

		namespace detail
		{
			template<typename T, typename = typename T::tag> struct is_tuple_impl :false_ {};

			template<typename T> struct is_tuple_impl<T, tuple_tag> :true_ {};
		}

		template<typename T> struct is_tuple : public and_<has_inner_tag<T>, detail::is_tuple_impl<T> > {};

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


			//for initalize from another tuple_storage
			template<typename T>
			constexpr tuple_storage(storage_move, T&& t) : ebo< uint_<indice>, Args>( ebo_value<uint_<indice>>(util::move(t) ) )...{}

			//merge two tuple_storage
			template<unsigned int size1, unsigned int size2, unsigned int... indice1, unsigned int... indice2, typename... Args1, typename... Args2>
			constexpr tuple_storage(storage_merge,
				tuple_storage<size1, index_sequence<indice1...>, Args1...> const& t1,
				tuple_storage<size2, index_sequence<indice2...>, Args2...> const& t2):
				ebo< uint_<indice1>, Args1>(ebo_value<uint_<indice1>>(t1))...,
				ebo< uint_<size1+indice2>, Args2>(ebo_value<uint_<indice2>>(t2))...
			{
				static_assert(size1 + size2 == size, "tuple storage merge size not fit");
			}

		};

		template<typename... Args>
		struct tuple 
		{ 
			static constexpr unsigned int L = sizeof...(Args);
			typedef tuple_storage<L, make_index_sequence<L>, Args...> value_type;
			typedef tuple<Args...> type;
			typedef tuple_tag tag;
			value_type storage;

			constexpr tuple() : storage{} {}

			//move from args
			explicit constexpr tuple(Args &&... args) : storage(util::move(args)...) {   }

			//copy from args
			explicit constexpr tuple(Args const&... args) : storage(static_cast<const Args&>(args)...) {   }

			//copy from same tuple
			constexpr tuple(tuple const& another_tuple) : storage(storage_move{}, another_tuple.storage) { printf("const tuple copy\n"); }

			//move from same tuple
			constexpr tuple(tuple && another_tuple) : storage(storage_move{}, util::move(another_tuple.storage)) { printf("move copy\n"); }
			
			//copy from not same but compatable tuple
			template<typename... TArgs>
			constexpr tuple(const tuple<TArgs...>& another_tuple) : storage(storage_move{}, util::move(another_tuple.storage)) { printf("const tuple<T> copy\n"); }

			//move from not same but compatable tuple
			template<typename... TArgs>
			constexpr tuple(tuple<TArgs...>&& another_tuple) : storage(storage_move{}, util::move(another_tuple.storage)) { printf("tuple<T> move\n"); }

			template<typename T1, typename T2>
			constexpr tuple(T1 const& t1, T2 const& t2) : storage(storage_merge{}, t1.storage, t2.storage)
			{
				static_assert(and_v< is_tuple<T1>, is_tuple<T2>>, "T1 or T2 must be tuple");
				static_assert(t1.length() + t2.length() == length(), "sum of T1 and T2's length need equal to merge tuple ");
			}

			//length of tuple, compiler constant
			static constexpr unsigned int length() { return L; }

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

			template<typename T>
			constexpr auto&& push_back(const T& t) const &
			{
				typedef tuple<Args..., T> new_type;
			}

			//merge two tuple and create concat tuple
			template<typename... TArgs>
			constexpr auto operator+(tuple<TArgs...> const& another)
			{
				typedef tuple<Args..., TArgs...> new_tuple;
				return new_tuple{*this, another};
			}
		};

		
		
	}
}