
#pragma once

#include<ccdk/type.h>
#include<ccdk/mpl/base/val_pack.h>

namespace ccdk
{
	namespace mpl
	{
		template<typename T, typename... Args>
		struct val_pack_merge
			: val_pack_merge< T, typename val_pack_merge<Args...>::type > {};

		template<typename T, T... args1, T... args2>
		struct val_pack_merge< val_pack<T, args1...>, val_pack<T, args2...>>
		{
			typedef val_pack<T, args1..., args2...> type;
		};

		template<typename T>
		struct val_pack_merge<T> { typedef T type; };
	}
}