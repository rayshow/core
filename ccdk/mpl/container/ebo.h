#pragma once

#include<ccdk/mpl/type_traits/is_empty.h>
#include<ccdk/mpl/type_traits/is_final.h>
#include<ccdk/mpl/type_traits/move.h>
#include<ccdk/mpl/type_traits/forward.h>

namespace ccdk
{
	namespace mpl
	{
		//empty class optimization if V is empty
		// V and U can be different type but U can be convert to V
		template<typename K, typename V, bool = is_empty_v<V> || !is_final_v<V> >
		struct ebo:public V
		{
			constexpr ebo() :V() {}
			template<typename U>
			constexpr ebo(U&& inU) : V(move(inU)) {}
		};

		//V is not empty
		template<typename K, typename V>
		struct ebo<K,V,false>
		{
			V v;
			constexpr ebo() :v() {}
			template<typename U>
			constexpr ebo(U&& inU) :v( move(inU) ) {}
		};

		//empty class
		template<typename K, typename V> constexpr V& ebo_value(ebo<K, V, true>& inEbo) noexcept { return inEbo; }
		template<typename K, typename V> constexpr V const& ebo_value(ebo<K, V, true> const& inEbo) noexcept { return inEbo; }
		template<typename K, typename V> constexpr V&& ebo_value(ebo<K, V, true>&& inEbo) noexcept { return forward<V&&>(inEbo); }
		//non-empty
		template<typename K, typename V> constexpr V& ebo_value(ebo<K, V, false>& inEbo) noexcept { return inEbo.v; }
		template<typename K, typename V> constexpr V const& ebo_value(ebo<K, V, false> const& inEbo) noexcept { return inEbo.v; }
		template<typename K, typename V> constexpr V&& ebo_value(ebo<K, V, false>&& inEbo) noexcept { return forward<V&&>(inEbo.v); }
	}
}