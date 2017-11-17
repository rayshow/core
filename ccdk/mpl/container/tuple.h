#pragma once

#include<ccdk/mpl/base/uint_.h>
#include<ccdk/mpl/base/index_sequence.h>
#include<ccdk/mpl/container/ebo.h>

namespace ccdk
{
	namespace mpl
	{
		template<unsigned int size, typename T, typename... Args>
		struct tuple_storage;

		template<unsigned int size, unsigned int... indice, typename... Args>
		struct tuple_storage<size, index_sequence<indice...>, Args...>:public ebo< uint_<indice>, Args>...
		{
			typedef tuple_storage<size, index_sequence<indice...>, Args...> type;

			//for initalize with args
			template<typename... Ys>
			constexpr tuple_storage(Ys&&... ys) : ebo< uint_<indice>, Ys >(util::move(ys) )...{}

			//for initalize from another tuple_storage
			template<typename T>
			constexpr tuple_storage(char, T&& t) : ebo< uint_<indice>, Args>( ebo_value<uint_<indice>>(util::move(t) ) )...{}
		};

		template<typename... Args>
		struct tuple 
		{ 
			static constexpr unsigned int L = sizeof...(Args);
			typedef tuple_storage<L, make_index_sequence<L>, Args...> value_type;
			typedef tuple<Args...> type;
			value_type storage;

			constexpr tuple() : storage() {}

			constexpr tuple(Args const& ... args) : storage(util::move(args)...) {}

			constexpr tuple(Args &&... args) : storage(util::move(args)...) {}

			template<typename... TArgs>
			constexpr tuple(const tuple<TArgs...>& another_tuple) : storage(0, util::move(another_tuple.storage)) {}

			template<typename... TArgs>
			constexpr tuple(tuple<TArgs...>&& another_tuple) : storage(util::move(another_tuple.storage)) {}

			constexpr unsigned int length() { return L; }

			template<typename T, T v>
			constexpr auto operator[](integer_<T, v> index) &
			{
				static_assert(v >= 0 && v < L, "index out of range");
				return ebo_value<uint_<v>>(storage);
			}

			template<typename T, T v>
			constexpr const const auto& operator[](integer_<T, v> index) const&
			{
				static_assert(v >= 0 && v < L, "index out of range");
				return ebo_value<uint_<v>>(storage);
			}

			template<typename T, T v>
			constexpr auto operator[](integer_<T, v> index) &&
			{
				static_assert(v >= 0 && v < L, "index out of range");
				return ebo_value<uint_<v>>(util::move(storage));
			}
		};

		template<>
		struct tuple<> {};
	}
}