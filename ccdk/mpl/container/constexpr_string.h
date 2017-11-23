
#include<ccdk/type.h>
#include<ccdk/mpl/base/integral_.h>
#include<ccdk/mpl/base/uint_.h>
#include<ccdk/mpl/base/arg_pack.h>
#include<ccdk/mpl/base/index_sequence.h>
#include<ccdk/mpl/container/string_view.h>

namespace ccdk
{
	namespace mpl
	{
		template<typename Ch> struct char_traits  {};

		template<> 
		struct char_traits<char>
		{
			static constexpr char end = '\0';
		};

		template<>
		struct char_traits<wchar_t> 
		{
			static constexpr char end = L'\0';
		};

		template<>
		struct char_traits<char16_t>
		{
			static constexpr char16_t end = u'\0';
		};

		template<>
		struct char_traits<char32_t>
		{
			static constexpr char16_t end = U'\0';
		};
		template<typename Ch, Ch... args>
		struct constexpr_string;

		namespace detail
		{
			template< uint32 start, uint32 len, typename Ch, typename T, typename U = constexpr_string<Ch>>
			struct constexpr_string_split;

			template< uint32 start, uint32 len, typename Ch, Ch c, Ch... args1, Ch... args2>
			struct constexpr_string_split< start, len, Ch, constexpr_string<Ch, c, args1...>, constexpr_string<Ch, args2...>>
				: constexpr_string_split< start - 1, len, Ch, constexpr_string<Ch, args1...>, constexpr_string<Ch, args2..., c>>{};

			template< uint32 len, typename Ch, Ch c, Ch... args1, Ch... args2>
			struct constexpr_string_split<0, len, Ch, constexpr_string<Ch, c, args1...>, constexpr_string<Ch, args2...>>
				: constexpr_string_split < 0, len - 1, Ch, constexpr_string<Ch, args1...>, constexpr_string<Ch, args2...>>{};

			template< typename Ch, Ch c, Ch... args1, Ch... args2>
			struct constexpr_string_split< 0, 1, Ch, constexpr_string<Ch, c, args1...>, constexpr_string<Ch, args2...>>
			{
				typedef constexpr_string<Ch, args2...> head;
				typedef constexpr_string<Ch, args1...> tail;
				typedef constexpr_string<Ch, args2..., args1...> type;
			};

			template<typename Ch, Ch c, Ch... args1, Ch... args2>
			struct constexpr_string_split< 0, 0, Ch, constexpr_string<Ch, c, args1...>, constexpr_string<Ch, args2...>>
			{
				typedef constexpr_string<Ch, args2...> head;
				typedef constexpr_string<Ch, c,args2...> tail;
				typedef constexpr_string<Ch, args1..., c,args2...> type;
			};

			template<typename Ch, uint32 , bool , Ch c, Ch... args>
			struct constexpr_string_find_impl;

			template<typename Ch, uint32 n, Ch c, Ch f, Ch... args>
			struct constexpr_string_find_impl<Ch, n, false, c, f, args...> :
				constexpr_string_find_impl<Ch, n + 1, c == f, c, args...> {};

			template<typename Ch, uint32 n, Ch c>
			struct constexpr_string_find_impl<Ch, n, false, c>
			{
				typedef uint_<-1> type;       // not found
			};

			template<typename Ch, uint32 n, Ch c, Ch f, Ch... args>
			struct constexpr_string_find_impl<Ch, n, true, c, f, args...>
			{
				typedef uint_<n> type;       // found at n
			};

			template<typename Ch, Ch... args>
			struct constexpr_string_find;
			
			template<typename Ch, Ch c, Ch f, Ch... args>
			struct constexpr_string_find<Ch, c,f, args...> 
				: constexpr_string_find_impl<Ch, 0, c == f, c, args...>{};
		}

		template<typename Ch, Ch... args>
		struct constexpr_string
		{
			static constexpr int L = sizeof...(args);
			Ch  storage[L + 1] = { args..., char_traits<Ch>::end };
			
			//static constexpr int a[2] = {1,1};
			
			typedef constexpr_string<Ch, args...> this_type;

			constexpr constexpr_string() = default;

			constexpr constexpr_string(const constexpr_string&) = default;

			constexpr constexpr_string& operator=(const constexpr_string&) = default;

			constexpr int length() const noexcept { return L; }

			constexpr int empty() const noexcept { return L == 0; }

			constexpr const Ch* c_str() const noexcept { return &storage[0]; }

			template< typename T, T v>
			constexpr auto operator[](integer_<T, v>) const noexcept { return storage[v]; }

			template<Ch c,Ch... args1>
			constexpr auto push_back() const noexcept
			{
				return constexpr_string<Ch, args..., c ,args1...>{};
			}
			
			template<Ch c,Ch... args1>
			constexpr auto push_front() const noexcept
			{
				return constexpr_string<Ch, c,args1..., args...>{};
			}

			template<Ch... args1>
			constexpr auto operator+(constexpr_string<Ch, args1...>) const noexcept
			{
				return push_back<args1...>();
			}

			template<Ch... args1>
			constexpr bool operator==(const this_type&) const noexcept { return true; }

			template<Ch... args1, Ch... args2, Ch... args3>
			constexpr auto __merge3(constexpr_string<Ch,args1...>, constexpr_string<Ch,args2...>, constexpr_string<Ch,args3...>) const noexcept
			{
				return constexpr_string<Ch, args1..., args2..., args3...>{};
			}

			template<uint32 start, Ch... args1>
			constexpr auto insert() const noexcept
			{
				static_assert(start >= 0 && start <= L, "constexpr_string insert out of range");
				using pack = detail::constexpr_string_split<start, 0, Ch, this_type >;
				return __merge3(typename pack::head{}, constexpr_string<Ch,args1...>{}, typename pack::tail{});
			}

			template<typename T, T index, Ch... args1>
			constexpr auto insert(integer_<T,index>, constexpr_string<Ch, args1...>)
			{	
				return insert< index, args1...>();
			}

			template<uint32 index, Ch... args1>
			constexpr auto insert(constexpr_string<Ch, args1...>) const noexcept
			{
				return insert<index, args1...>();
			}


			template<uint32 start, uint32 end = start + 1>
			constexpr auto erase() const noexcept
			{
				static_assert(start >= 0 && end <= L && start < end, "constexpr_string erase out of range");
				return typename detail::constexpr_string_split<start, end - start, Ch, this_type>::type{};
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

			template<uint32 start, Ch... args1, uint32 end = start + 1>
			constexpr auto replace() const noexcept
			{
				static_assert(start >= 0 && end <= L && start < end, "constexpr_string replace out of range");
				using pack = detail::constexpr_string_split<start, end - start, Ch, this_type>;
				return __merge3(typename pack::head{}, constexpr_string<Ch,args1...>{}, typename pack::tail{});
			}

			template<Ch... args1, typename T, uint32 start, uint32 end = start+1>
			constexpr auto replace(integer_<T,start>) const noexcept
			{
				return replace<start, args1...>();
			}

			//find single char
			template<Ch c>
			constexpr auto find() const noexcept
			{
				//return uint_<view.find(c)>{};
				static_assert(L >= 1, " empty string can't find");
				return typename detail::constexpr_string_find<Ch, c, args...>::type{};
			}

			template<Ch... args1>
			constexpr auto find(constexpr_string<Ch, args1...>) const noexcept
			{

			}
		};

		namespace literals
		{

			constexpr auto operator""_cs(const char* str, const std::size_t len)
			{
				struct tmp {
					int a;
					constexpr tmp(int inA) :a(inA) {}
				};

				//uint_<len>{};
				return len;
				//return constexpr_string<char, args...>{};
			}

			template<char... args>
			constexpr auto operator""_cc()
			{
				return constexpr_string<char, args...>{};
			}
		}
	}
}