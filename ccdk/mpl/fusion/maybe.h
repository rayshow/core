#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/uint32_.h>
#include<ccdk/mpl/base/enable_if.h>
#include<ccdk/mpl/type_traits/is_empty.h>
#include<ccdk/mpl/type_traits/is_final.h>
#include<ccdk/mpl/type_traits/is_convertible.h>
#include<ccdk/mpl/type_traits/max_align_t.h>
#include<ccdk/mpl/util/forward.h>
#include<ccdk/mpl/util/swap.h>
#include<ccdk/mpl/base/make_indice.h>

ccdk_namespace_mpl_fs_start

template<typename T>
class maybe
{
public:
	static constexpr uint32 memory_size = max_align<0, T>::MaxSize;
	typedef maybe              this_type;
	typedef decay_t<T>         value_type;

private:
	char  memory[memory_size];
	bool  initalized;

	CCDK_FORCEINLINE constexpr void construct(T const& t) {
		new(memory) T{ t };
		initalized = true;
	}

	CCDK_FORCEINLINE constexpr void destruct() {
		reinterpret_cast<T>(memory).~T();
	}

public:
	CCDK_FORCEINLINE constexpr maybe() : initalized{ false } {}

	CCDK_FORCEINLINE constexpr maybe(T const& t) { construct(t); }


	constexpr ~maybe() { if (initalized) { destruct(); } }
};


ccdk_namespace_mpl_fs_end