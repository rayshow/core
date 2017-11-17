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
			static_assert(n >= 0, "index_sequence size need greater or equal then 0");
		};

		template<unsigned int... args>
		struct make_index_sequence_t<0, args...>
		{
			typedef index_sequence< args...> type;
		};

		template<unsigned int size>
		using make_index_sequence = typename make_index_sequence_t<size>::type;
	}
}