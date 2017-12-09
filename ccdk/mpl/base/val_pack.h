#pragma once

#include<ccdk/mpl/base/bool_.h>

namespace ccdk
{
	namespace mpl
	{
		template<typename T, T... args> struct val_pack { typedef val_pack<T, args...> type; };

		template<uint32... indice> using indice_pack = val_pack<uint32, indice...>;

		template<typename T> struct is_indice_pack :false_ {};
		template<uint32... indice> struct is_indice_pack< indice_pack<indice...>> :true_ {};

		template<typename T, T c, T... args> struct val_first { static constexpr T value = c; };
	}
}