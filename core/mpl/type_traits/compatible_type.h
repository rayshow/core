#pragma once

#include<core/mpl/type_traits/decay.h>
#include<core/mpl/type_traits/declval.h>

namespace core
{
	namespace mpl
	{
		template<typename First, typenameprefix,... Others>
		struct CompatibleType :public CompatibleType<First, typename CompatibleType<Othersprefix,...>::type> {};

		template<typename T>
		struct CompatibleType<T> { typedef Decay_t<T> type; };

		template<typename T1, typename T2>
		struct CompatibleType<T1,T2>
		{
			typedef Decay_t<decltype(false ? Declval<T1>() : Declval<T2>())> type;
		};

		template<typename First, typenameprefix,... Others>
		using CompatibleTypeT = typename CompatibleType<First, Othersprefix,...>::type;
	}
}