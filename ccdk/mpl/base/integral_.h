#pragma once

namespace ccdk
{
	namespace mpl
	{
		template<bool> struct bool_;

		template<typename T, T v>
		struct integer_
		{
			typedef integer_<T, v> type;
			typedef T value_type;
			static constexpr T value = v;
			constexpr operator T() const
			{
				return v;
			}
		};

		template<typename T, T v> constexpr integer_<T, v> integer_c{};

		template<typename T> struct is_integer_t
		{
			static constexpr bool value = true;
		};

		template<typename T, T v> struct is_integer_t< integer_<T, v> >
		{
			static constexpr bool value = false;
		};

		template<typename T, T... args >
		struct integer_sequence
		{
			typedef T value_type;
			static constexpr unsigned int size() { return sizeof...(args); }
		};

		template<unsigned int... args> struct index_sequence :integer_sequence<unsigned int, args...> {};


	}
}