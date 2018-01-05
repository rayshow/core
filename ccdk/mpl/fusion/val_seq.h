#pragma once

#include<ccdk/mpl/base/integer_.h>
#include<ccdk/mpl/mcontainer/val_pack_at.h>
#include<ccdk/mpl/mcontainer/val_pack_split.h>
#include<ccdk/mpl/mcontainer/val_pack_find.h>
#include<ccdk/mpl/base/enable_if.h>
#include<ccdk/mpl/mpl_module.h>

ccdk_namespace_mpl_fs_start

	template<typename T, T... args>
	struct val_seq
	{
		typedef val_seq<T, args...> type;
		static constexpr int L = sizeof...(args);

		constexpr int length() const noexcept { return L; }
		constexpr int empty() const noexcept { return L == 0; }

		template< typename U, U v>
		constexpr auto operator[](integer_<U, v>) const noexcept
		{
			static_assert(v < L, "index out of range");
			return val_pack_at_exp<T, v, args...>;
		}

		template<T c, T... args1>
		constexpr auto push_back() const noexcept { return val_seq<T, args..., c, args1...>{}; }

		template<T c, T... args1>
		constexpr auto push_front() const noexcept { return val_seq<T, c, args1..., args...>{}; }

		template<T... args1>
		constexpr auto operator|(val_seq<T, args1...>) const noexcept
		{
			return push_back<args1...>();
		}

		constexpr bool operator==(const type&) const noexcept { return true; }

		template<T... args1, T... args2, T... args3>
		constexpr auto __merge3(val_pack<T, args1...>, val_pack<T, args2...>, val_pack<T, args3...>) const noexcept
		{
			return val_seq<T, args1..., args2..., args3...>{};
		}

		template<uint32 start, T c, T... args1>
		constexpr auto insert() const noexcept
		{
			static_assert(start >= 0 && start <= L, "type_seq insert out of range");
			using pack = val_pack_split_exp<T, start, start, args... >;
			return __merge3(typename pack::head{}, val_pack<T, c, args1...>{}, typename pack::tail{});
		}

		template<typename I, T index, T c, T... args1>
		constexpr auto insert(integer_<I, index>, val_seq<T, c, args1...>) const noexcept
		{
			return insert< index, c, args1...>();
		}

		template<uint32 index, T c, T... args1>
		constexpr auto insert(val_seq<T, c, args1...>) const noexcept
		{
			return insert<index, c, args1...>();
		}

		template<T... args>
		constexpr auto __erase_impl(val_pack<T, args...>)
		{
			return val_seq<T, args...>{};
		}

		template<uint32 start, uint32 end = start + 1, typename = check_in_range2<start, end, L> >
		constexpr auto erase() const noexcept
		{
			static_assert(start >= 0 && end <= L && start < end, "index out of range");

			return  __erase_impl( typename val_pack_split_exp<T, start, end, args...>::type{} );
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

		template<uint32 start, uint32 end, T... args1, typename = check_in_range2<start, end, L>>
		constexpr auto replace() const noexcept
		{
			static_assert(start >= 0 && end <= L && start < end, "out of range");
			using pack = val_pack_split_exp< T, start, end, args...>;
			return __merge3(typename pack::head{}, val_pack<T, args1...>{}, typename pack::tail{});
		}

		//find single char
		template<T c>
		constexpr auto find() const noexcept
		{
			static_assert(L >= 1, " empty string can't find");
			return val_pack_find_exp<T, c, args...>;
		}

		template<T c>
		constexpr bool contain() const noexcept
		{
			return find<c>() != L;
		}
	};

ccdk_namespace_mpl_fs_end