#pragma once

namespace ccdk
{
	namespace mpl
	{
		template<typename T, T... args>
		struct integer_sequence
		{
			//static constexpr unsigned int size = sizeof...(args);
			typedef integer_sequence<T, args...> type;
			typedef T value_type;
		};

		template<unsigned int... args> using index_sequence = integer_sequence<unsigned int, args...>;
		
		template<unsigned int n, unsigned int... args> struct make_index_sequence_t:make_index_sequence_t<n-1, n-1, args...> {};
		template<unsigned int... args> struct make_index_sequence_t<0, args...> { typedef index_sequence< args...> type; };

		//generate 0 .. n -1 sequence
		template<unsigned int n>
		using make_index_sequence = typename make_index_sequence_t<n>::type;

		template<unsigned int len, unsigned int first, unsigned int second, unsigned int... args>
		struct make_index_sequence_ingore_t :make_index_sequence_ingore_t<len - 1, first - 1, second, len - 1, args...> {};

		template<unsigned int len, unsigned int second, unsigned int... args>
		struct make_index_sequence_ingore_t<len, 0, second, args...> :make_index_sequence_ingore_t<len, 0, second - 1, second - 1, args...> {};

		template<unsigned int len, unsigned int... args>
		struct make_index_sequence_ingore_t<len, 0, 0, args...>
		{
			typedef index_sequence< args...> type;
		};

		//generate 0 .. index - 1 index+len .. n-1 sequence
		template<unsigned int n, unsigned int index, unsigned int len=1>
		using make_index_sequence_ingore = typename make_index_sequence_ingore_t<n, n - index - len, index>::type;

		
		template<unsigned int n, unsigned count,   unsigned int... args>
		struct make_index_sequence_util_t :make_index_sequence_util_t<n - 1, count-1, n - 1, args...>{};

		template<unsigned int n, unsigned int... args>
		struct make_index_sequence_util_t<n, 0, args...>
		{
			typedef index_sequence< args...> type;
		};

		// generate index  .. n-1 sequence
		template<unsigned int n, unsigned int index>
		using make_index_sequence_until = typename make_index_sequence_util_t<n, n - index>::type;
	}
}