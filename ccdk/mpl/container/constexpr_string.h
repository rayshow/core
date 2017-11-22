
#include<ccdk/type.h>
#include<ccdk/mpl/base/integral_.h>
#include<ccdk/mpl/base/arg_pack.h>
#include<ccdk/mpl/base/index_sequence.h>

namespace ccdk
{
	namespace mpl
	{
		template<typename Ch>
		struct char_traits {};

		template<> 
		struct char_traits<char>
		{
			static constexpr char end = '\n';
		};

		template<>
		struct char_traits<wchar_t>
		{
			static constexpr char end = L'\n';
		};


		template<>
		struct char_traits<char16_t>
		{
			static constexpr char16_t end = u'\n';
		};

		template<>
		struct char_traits<char32_t>
		{
			static constexpr char16_t end = U'\n';
		};

		namespace detail
		{
			template<typename Ch, Ch... args>
			struct constexpr_string;

			template< uint32 start, uint32 len, typename Ch, typename T, typename U = constexpr_string<Ch>>
			struct constexpr_string_split;

			template< uint32 start, uint32 len, typename Ch, Ch c, Ch... args1, Ch... args2>
			struct constexpr_string_split< start, len, Ch, constexpr_string<Ch, c, args1...>, constexpr_string<Ch, args2...>>
				: constexpr_string_split< start - 1, len, Ch, constexpr_string<Ch, args1...>, constexpr_string<Ch, args2..., c>{};

			template< uint32 len, typename Ch, Ch c, Ch... args1, Ch... args2>
			struct constexpr_string_split<0, len, Ch, constexpr_string<Ch, c, args1...>, constexpr_string<Ch, args2...>>
				: constexpr_string_split < 0, len - 1, Ch, constexpr_string<Ch, args1...>, constexpr_string<Ch, args2...>{};

			template< typename Ch, Ch c, Ch... args1, Ch... args2>
			struct constexpr_string_split< 0, 1, Ch, constexpr_string<Ch, c, args1...>, constexpr_string<Ch, args2...>
			{
				typedef constexpr_string<Ch, args2...> head;
				typedef constexpr_string<Ch, args1...> tail;
				typedef constexpr_string<Ch, args2..., args1...> type;
			};

			template<typename Ch, Ch c, Ch... args1, Ch... args2>
			struct constexpr_string_split< 0, 0, Ch, constexpr_string<Ch, c, args1...>, constexpr_string<Ch, args2...>
			{
				typedef constexpr_string<Ch, args2...> head;
				typedef constexpr_string<Ch, c,args2...> tail;
				typedef constexpr_string<Ch, args1..., c,args2...> type;
			};
		}




		template<typename Ch, Ch... args>
		struct constexpr_string
		{
			static constexpr int L = sizeof...(args);
			static constexpr Ch const storage[L+1] = {args..., char_traits<Ch>::end };

			constexpr constexpr_string() = default;

			constexpr constexpr_string(const constexpr_string&) = default;

			constexpr constexpr_string& operator=(const constexpr_string&) = default;

			constexpr int length() { return L;  }

			template< typename T, T v>
			constexpr auto operator[](integer_<T, v>) { return storage[v]; }

			template<Ch... args1>
			constexpr auto push_back()
			{
				return constexpr_string<Ch,args..., args1...>{};
			}

			template<Ch... args1>
			constexpr auto push_back(constexpr_string<Ch, args1...>)
			{
				return constexpr_string<Ch,args..., args1...>{};
			}

			template<Ch... args1> 
			constexpr auto push_front()
			{
				return constexpr_string<Ch, args1..., args...>{};
			}

			template<Ch... args1>
			constexpr auto push_front(constexpr_string<Ch, args1...>)
			{
				return constexpr_string<Ch, args1..., args...>{};
			}

			template<Ch... args1, Ch... args2, Ch... args3>
			constexpr auto __insert_impl(arg_pack<args1...>, arg_pack<arg2...>, arg_pack<args3...>)
			{
				return constexpr_string<Ch, args1..., args2..., args3...>{};
			}

			template<uint32 start, Ch... args1>
			constexpr auto insert()
			{
				static_assert(start >= 0 && start <= L, "constexpr_string insert out of range");
				using pack = arg_pack_split<start, 0, arg_pack<args...>>;
				return __insert_impl(typename pack::head{}, arg_pack<args1...>{}, typename pack::tail{});
			}

			template<uint32 start, Ch... args1>
			constexpr auto insert(constexpr_string<Ch, args1...>)
			{
				return insert<start, args1...>();
			}

			template<Ch... args1>
			constexpr auto __erase_impl(arg_pack<args1...>)
			{
				return constexpr_string<args1...>{};
			}

			template<uint32 start, uint32 end = start + 1>
			constexpr auto erase()
			{
				static_assert(start >= 0 && end <= L && start < end, "constexpr_string erase out of range");
				return __erase_impl(arg_pack_split<start, end - start, arg_pack<args...>>{});
			}

			template<uint32 start, uint32 end = start+1, Ch... args1>
			constexpr auto replace()
			{
				static_assert(start >= 0 && end <= L && start < end, "constexpr_string replace out of range");
				using pack = arg_pack_split<start, end - start, arg_pack<args...>>;
				return __insert_impl(typename pack::head{}, arg_pack<args1...>{}, typename pack::tail{});
			}

			template<Ch c>
			constexpr uint32 find()
			{
				for (int i = 0; i < L; ++i) if (c == storage[i]) return i;
			}

		};
	}
}