#pragma once

namespace ccdk
{
	namespace mpl
	{
		template<typename... Args> struct arg_pack_first;

		template<typename T, typename... Args> struct arg_pack_first<T, Args...> { typedef T type; };

		template<typename... Args> using arg_pack_first_t = typename arg_pack_first<Args...>::type;
	}
}