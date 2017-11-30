#pragma once

#include<ccdk/mpl/base/uint_.h>
#include<ccdk/mpl/type_traits/is_empty.h>
#include<ccdk/mpl/type_traits/is_final.h>
#include<ccdk/mpl/type_traits/forward.h>

namespace ccdk
{
	namespace mpl
	{
		//empty class optimization if V is empty
		// V and U can be different type but U can be convert to V
		template<typename K, typename V, bool = is_empty_v<V> && !is_final_v<V> >
		struct ebo:public V
		{
			constexpr ebo() :V() {}

			//prefect forward
			template<typename U>
			constexpr ebo(U&& inU) : V(util::forward<U>(inU)) {}

			template<typename U>
			constexpr ebo(const U& inU): V(inU) {}

		};

		//V is not empty
		template<typename K, typename V>
		struct ebo<K,V,false>
		{
			V v;
			constexpr ebo() :v() {}

			//use () initalize to allow narrow conversion like int to float
			template<typename U>
			constexpr ebo(U && inU) : v{ util::forward<U>(inU) } {}

			template<typename U>
			constexpr ebo(const U& inU) : v{ inU } {}
		};

		template<uint32 index, typename V>
		using iebo = ebo<uint_<index>, V>;

		//empty class
		template<typename K, typename V> inline constexpr V& ebo_value(ebo<K, V, true>& inEbo) noexcept { return inEbo; }
		template<typename K, typename V> inline constexpr V const& ebo_value(ebo<K, V, true> const& inEbo) noexcept { return inEbo; }
		template<typename K, typename V> inline constexpr V&& ebo_value(ebo<K, V, true>&& inEbo) noexcept { return util::move(inEbo); }
		//non-empty
		template<typename K, typename V> inline constexpr V& ebo_value(ebo<K, V, false>& inEbo) noexcept { return inEbo.v; }
		template<typename K, typename V> inline constexpr V const& ebo_value(ebo<K, V, false> const& inEbo) noexcept { return inEbo.v; }
		template<typename K, typename V> inline constexpr V&& ebo_value(ebo<K, V, false>&& inEbo) noexcept { return util::move(inEbo.v); }

		//empty class
		template<uint32 index, typename V> inline constexpr V& ebo_at(ebo<uint_<index>, V, true>& inEbo) noexcept { return inEbo; }
		template<uint32 index, typename V> inline constexpr V const& ebo_at(ebo<uint_<index>, V, true> const& inEbo) noexcept { return inEbo; }
		template<uint32 index, typename V> inline constexpr V&& ebo_at(ebo<uint_<index>, V, true>&& inEbo) noexcept { return util::move(inEbo); }
		//non-empty
		template<uint32 index, typename V> inline constexpr V& ebo_at(ebo<uint_<index>, V, false>& inEbo) noexcept { return inEbo.v; }
		template<uint32 index, typename V> inline constexpr V const& ebo_at(ebo<uint_<index>, V, false> const& inEbo) noexcept { return inEbo.v; }
		template<uint32 index, typename V> inline constexpr V&& ebo_at(ebo<uint_<index>, V, false>&& inEbo) noexcept { return util::move(inEbo.v); }
	}
}