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

		template<unsigned int... args>
		using index_sequence = integer_sequence<unsigned int, args...>;

		template<unsigned int n, unsigned int... args>
		struct make_index_sequence_t:make_index_sequence_t<n-1, n-1, args...> 
		{
			
		};

		template<unsigned int... args>
		struct make_index_sequence_t<0, args...>
		{
			typedef index_sequence< args...> type;
		};

		template<unsigned int n, unsigned count,   unsigned int... args>
		struct make_index_sequence_count_t :make_index_sequence_count_t<n - 1, count-1, n - 1, args...>{};

		template<unsigned int n, unsigned int... args>
		struct make_index_sequence_count_t<n, 0, args...> 
		{
			typedef index_sequence< args...> type;
		};

		template<unsigned int size>
		using make_index_sequence = typename make_index_sequence_t<size>::type;

		template<unsigned int size, unsigned int index>
		using make_index_sequence_until = typename make_index_sequence_count_t<size, size - index>::type;
	}
}