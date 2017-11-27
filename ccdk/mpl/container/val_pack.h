#pragma once

#include<ccdk/type.h>
#include<ccdk/mpl/base/integer_.h>
#include<ccdk/mpl/base/uint_.h>
#include<ccdk/mpl/base/enable_if.h>
#include<ccdk/mpl/type_traits/is_integer.h>


namespace ccdk
{
	namespace mpl
	{
		template<typename T, T... args>
		struct val_pack;

		template<uint32... indice>
		using indice_pack = val_pack<uint32, indice...>;

		namespace detail
		{
			template<typename P, uint32 start, uint32 len, typename T = int32, typename U = val_pack<T>>
			struct val_pack_split_impl;

			template< uint32 start, uint32 len, typename T, T c, T... args1, T... args2>
			struct val_pack_split_impl< val_pack<T, c, args1...>, start, len, T, val_pack<T, args2...>>
				: val_pack_split_impl< val_pack<T, args1...>, start - 1, len, T, val_pack<T, args2..., c>> {};

			template< uint32 len, typename T, T c, T... args1, T... args2>
			struct val_pack_split_impl< val_pack<T, c, args1...>, 0, len, T, val_pack<T, args2...>>
				: val_pack_split_impl < val_pack<T, args1...>, 0, len - 1, T, val_pack<T, args2...>> {};

			// >=1 len
			template< typename T, T c, T... args1, T... args2>
			struct val_pack_split_impl< val_pack<T, c, args1...>, 0, 1, T, val_pack<T, args2...>>
			{
				typedef val_pack<T, args2...> head;
				typedef val_pack<T, args1...> tail;
				typedef val_pack<T, args2..., args1...> type;
			};
			// 0 len
			template<typename T, T c, T... args1, T... args2>
			struct val_pack_split_impl< val_pack<T, c, args1...>, 0, 0, T, val_pack<T, args2...>>
			{
				typedef val_pack<T, args2...> head;
				typedef val_pack<T, c, args1...> tail;
				typedef val_pack<T, c, args1..., args2...> type;
			};

			//empty P
			template< uint32 start, uint32 len, typename T, T... args>
			struct val_pack_split_impl< val_pack<T>, start, len, T, val_pack<T, args...>>
			{
				typedef val_pack<T, args...> head;
				typedef val_pack<T> tail;
				typedef val_pack<T, args...> type;
			};

			template<typename T, typename P, uint32 i, T v, bool found>
			struct val_pack_find_impl;

			template<typename T, uint32 i, T v, T c, T... args>
			struct val_pack_find_impl< T, val_pack<T, c, args...>, i, v, false> :
				val_pack_find_impl<T, val_pack<T, args...>, i + 1, v, c == v> {};

			template<typename T, uint32 i, T v>
			struct val_pack_find_impl<T, val_pack<T>, i, v, false> :uint_<i> {};

			template<typename T, uint32 i, T v, T c, T... args>
			struct val_pack_find_impl<T, val_pack<T, c, args...>, i, v, true> :uint_<i-1> {};

			template<typename T, typename P, uint32 n, uint32 i = 0>
			struct val_pack_at_impl;

			template<typename T, uint32 n, uint32 i, T c, T... args>
			struct val_pack_at_impl<T, val_pack<T, c, args...>, n, i> :
				val_pack_at_impl<T, val_pack<T, args...>, n, i + 1> 
			{};

			template<typename T, uint32 n, T c, T... args>
			struct val_pack_at_impl<T, val_pack<T, c, args...>, n, n>
			{
				static constexpr T value = c;
			};

			template<typename T, T start, T end, T... args> 
			struct val_pack_create_impl :val_pack_create_impl<T, start+1, end, args..., start> {};

			template<typename T, T end, T... args> 
			struct val_pack_create_impl<T, end, end, args...> { typedef val_pack< T,args...> type; };
		}

		template<typename T, T v, T... args>
		static constexpr uint32 val_pack_find_e = detail::val_pack_find_impl<T, val_pack<T, args...>, 0, v, false>::value;

		template<typename T, T v, typename P>
		static constexpr uint32 val_pack_find = detail::val_pack_find_impl<T, P, 0, v, false>::value;
		
		template<typename T,uint32 start, uint32 end, typename P>
		using val_pack_split =  detail::val_pack_split_impl< P, start, end-start, T>;

		template<typename T, uint32 start, uint32 end, T... args>
		using val_pack_split_e =  detail::val_pack_split_impl< val_pack<T, args...>, start, end-start, T>;

		template<typename T, uint32 index, typename P>
		static constexpr  T val_pack_at = detail::val_pack_at_impl< T, P, index>::value;
		
		template<typename T, uint32 index, T... args>
		static constexpr  T val_pack_at_e = detail::val_pack_at_impl< T, val_pack<T, args...>, index>::value;

		template<typename T, T start, T end>
		using val_pack_create = typename detail::val_pack_create_impl<T, start, end>::type;

		template<uint32 end>
		using make_indice = typename val_pack_create<uint32, 0, end>::type;

		template<uint32 start, uint32 end, uint32... args>
		using make_indice_from = typename val_pack_create<uint32, start, end>::type;

		template<uint32 length, uint32 start, uint32 end>
		using make_indice_ingore = typename val_pack_split< uint32, start, end, make_indice<length> >::type;
		
		template<typename T, T... args>
		struct val_pack
		{
			typedef val_pack<T, args...> type;
			static constexpr int L = sizeof...(args);

			constexpr int length() const noexcept { return L; }
			constexpr int empty() const noexcept { return L == 0; }

			template< typename U, U v>
			constexpr auto operator[](integer_<U, v>) const noexcept
			{ 
				static_assert(v < L, "index out of range");
				return val_pack_at_e<T, v, args...>; 
			}

			template<T c, T... args1>
			constexpr auto push_back() const noexcept { return val_pack<T, args..., c , args1...>{}; }

			template<T c, T... args1>
			constexpr auto push_front() const noexcept { return val_pack<T, c, args1..., args...>{}; }

			template<T... args1>
			constexpr auto operator|(val_pack<T, args1...>) const noexcept
			{
				return push_back<args1...>();
			}

			constexpr bool operator==(const type&) const noexcept { return true; }

			template<T... args1, T... args2, T... args3>
			constexpr auto __merge3(val_pack<T, args1...>, val_pack<T, args2...>, val_pack<T, args3...>) const noexcept
			{
				//DebugFunctionName();
				return val_pack<T, args1..., args2..., args3...>{};
			}

			template<uint32 start, T c,T... args1>
			constexpr auto insert() const noexcept
			{
				static_assert(start >= 0 && start <= L, "type_seq insert out of range");
				using pack = val_pack_split_e<T, start, start, args... >;
				return __merge3(typename pack::head{}, val_pack<T, c,args1...>{}, typename pack::tail{});
			}

			template<typename T, T index, T c, T... args1>
			constexpr auto insert(integer_<T, index>, val_pack<T, c ,args1...>) const noexcept
			{
				return insert< index, c,args1...>();
			}

			template<uint32 index, T c,T... args1>
			constexpr auto insert(val_pack<T, c,args1...>) const noexcept
			{
				return insert<index, c,args1...>();
			}


			template<uint32 start, uint32 end = start + 1, typename=check_in_range2<start,end,L> >
			constexpr auto erase() const noexcept
			{
				static_assert(start >= 0 && end <= L && start < end, "index out of range");
				return typename val_pack_split_e<T, start, end, args...>::type{};
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

			template<uint32 start, uint32 end ,T... args1, typename=check_in_range2<start,end,L>>
			constexpr auto replace() const noexcept
			{
				static_assert(start >= 0 && end <= L && start < end, "out of range");
				using pack = val_pack_split_e< T, start, end, args...>;
				return __merge3(typename pack::head{}, val_pack<T, args1...>{}, typename pack::tail{});
			}

			//find single char
			template<T c>
			constexpr auto find() const noexcept
			{ 
				static_assert(L >= 1, " empty string can't find");
				return val_pack_find_e<T, c, args...>;
			}

			template<T c>
			constexpr bool contain() const noexcept
			{
				return find<c>() != L;
			}
		};
	}
}