#pragma once

#include<ccdk/type.h>
#include<ccdk/mpl/util/swap.h>

ccdk_namespace_mpl_fs_start

template<typename T>
struct range
{ 
	T start;
	T end;

	CCDK_FORCEINLINE ptr::size_t size() { return end - start; }

	CCDK_FORCEINLINE bool valid() { return end < start; }

	CCDK_FORCEINLINE void swap(range & other) {  util::swap(start, other.start); util::swap(end, other.end); }

};

ccdk_namespace_mpl_fs_end