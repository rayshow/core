#pragma once

#include<ccdk/mpl/type_traits/is_empty.h>
#include<ccdk/mpl/type_traits/is_final.h>
#include<ccdk/mpl/type_traits/forward.h>

namespace ccdk
{
	namespace mpl
	{
		//empty class optimization if V is empty
		template<typename K, typename V, bool = is_empty_v<V> || !is_final_v<V> >
		struct ebo:public V
		{

		};

		//V is not empty
		template<typename K, typename V>
		struct ebo<K,V,false>
		{
			V v;
			constexpr ebo(V&& inV) :v(inV) {}
		};
	}
}