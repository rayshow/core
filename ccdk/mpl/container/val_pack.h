#pragma once

#include<ccdk/type.h>
#include<ccdk/mpl/base/integral_.h>
#include<ccdk/mpl/base/uint_.h>


namespace ccdk
{
	namespace mpl
	{
		template<typename T, T... args>
		struct val_pack;

		template<typename P, uint32 start, uint32 len, typename T = int32, typename U = val_pack<T>>
		struct val_pack_split;

		template< uint32 start, uint32 len, typename T, T c, T... args1, T... args2>
		struct val_pack_split< val_pack<T, c, args1...>, start, len, T , val_pack<T, args2...>>
			: val_pack_split< val_pack<T, args1...>, start - 1, len, T, val_pack<T, args2..., c>> {};

		template< uint32 len, typename T, T c, T... args1, T... args2>
		struct val_pack_split< val_pack<T, c, args1...>, 0, len, T, val_pack<T, args2...>>
			: val_pack_split < val_pack<T, args1...>, 0, len - 1, T, val_pack<T, args2...>> {};

		// >=1 len
		template< typename T, T c, T... args1, T... args2>
		struct val_pack_split< val_pack<T, c, args1...>, 0, 1, T, val_pack<T, args2...>>
		{
			typedef val_pack<T, args2...> head;
			typedef val_pack<T, args1...> tail;
			typedef val_pack<T, args2..., args1...> type;
		};
		// 0 len
		template<typename T, T c, T... args1, T... args2>
		struct val_pack_split< val_pack<T, c, args1...>, 0, 0, T, val_pack<T, args2...>>
		{
			typedef val_pack<T, args2...> head;
			typedef val_pack<T, c, args2...> tail;
			typedef val_pack<T, args1..., c, args2...> type;
		};

		template<typename T, typename P, uint32 i, T v, bool found>
		struct val_pack_find;

		template<typename T, uint32 i,  T v, T c, T... args>
		struct val_pack_find< T, val_pack<T, c, args...>, i, v, false> :
			val_pack_find<T, val_pack<T,args...>, i+1, v, c==v> {};

		template<typename T, uint32 i, T v>
		struct val_pack_find<T, val_pack<T>, i, v, false>:uint_<i> {};

		template<typename T, uint32 i, T v, T c, T... args>
		struct val_pack_find<T, val_pack<T,c, args...>, i, v, true> :uint_<i> {};


		template<typename T, typename P, uint32 n, uint32 i = 0>
		struct val_pack_at;

		template<typename T, uint32 n, uint32 i, T c, T... args>
		struct val_pack_at<T, val_pack<T, c, args...>, n, i> :
			val_pack_at<T, val_pack<args...>, n, i + 1> {};

		template<typename T, uint32 n, T c, T... args>
		val_pack_at<T, val_pack<T, c, args...>, n, n>
		{
			static constexpr T value = c; 
		};
		
		template<typename T, T... args>
		struct val_pack
		{
			typedef val_pack<T, args...> type;
			static constexpr int L = sizeof...(args);

			constexpr int length() const noexcept { return L; }
			constexpr int empty() const noexcept { return L == 0; }

			template< typename U, U v>
			constexpr auto operator[](integer_<U, v>) const noexcept { return val_pack_at<T, val_pack<args...>, v>::value; }

			template<T c, T... args1>
			constexpr auto push_back() const noexcept { return val_pack<T, args..., c , args1...>{}; }

			template<T c, T... args1>
			constexpr auto push_front() const noexcept { return val_pack<T, c, args1..., args...>{}; }

			template<T... args1>
			constexpr auto operator+(val_pack<T, args1...>) const noexcept
			{
				return push_back<args1...>();
			}

			constexpr bool operator==(const type&) const noexcept { return true; }

			template<T... args1, T... args2, T... args3>
			constexpr auto __merge3(arg_pack<T, args1...>, arg_pack<T, args2...>, arg_pack<T, args3...>) const noexcept
			{
				return arg_pack<T, args1..., args2..., args3...>{};
			}

			template<uint32 start, T... args1>
			constexpr auto insert() const noexcept
			{
				static_assert(start >= 0 && start <= L, "type_seq insert out of range");
				using pack = val_pack_split<T, val_pack<T,args...> , T, this_type >;
				return __merge3(typename pack::head{}, type_seq<T, args1...>{}, typename pack::tail{});
			}

			template<typename T, T index, T... args1>
			constexpr auto insert(integer_<T, index>, type_seq<T, args1...>)
			{
				return insert< index, args1...>();
			}

			template<uint32 index, T... args1>
			constexpr auto insert(type_seq<T, args1...>) const noexcept
			{
				return insert<index, args1...>();
			}


			template<uint32 start, uint32 end = start + 1>
			constexpr auto erase() const noexcept
			{
				static_assert(start >= 0 && end <= L && start < end, "type_seq erase out of range");
				return typename detail::type_seq_split<start, end - start, T, this_type>::type{};
			}

			template<uint32 len = 1>
			constexpr auto pop_front()  const noexcept
			{
				return erase<0, len>();
			}

			template<uint32 len = 1>
			constexpr auto pop_back() const noexcept
			{
				return erase<L - len, L>();
			}

			template<uint32 start, T... args1, uint32 end = start + 1>
			constexpr auto replace() const noexcept
			{
				static_assert(start >= 0 && end <= L && start < end, "type_seq replace out of range");
				using pack = detail::type_seq_split<start, end - start, T, this_type>;
				return __merge3(typename pack::head{}, type_seq<T, args1...>{}, typename pack::tail{});
			}

			template<T... args1, typename T, uint32 start, uint32 end = start + 1>
			constexpr auto replace(integer_<T, start>) const noexcept
			{
				return replace<start, args1...>();
			}

			//find single char
			template<T c>
			constexpr auto find() const noexcept
			{ 
				//return uint_<view.find(c)>{};
				static_assert(L >= 1, " empty string can't find");
				return typename detail::type_seq_find<T, c, args...>::type{};
			}

			template<T... args1>
			constexpr auto find(type_seq<T, args1...>) const noexcept
			{

			}
		};


	}
}