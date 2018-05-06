#pragma once

#include<ccdk/container/container_mudule.h>
#include<ccdk/container/impl/hash_table.h>

ccdk_namespace_ct_start

using namespace mpl;

template<
	typename Key,
	typename Value,
	typename MaxLoadFactor = units::ratio<1, 2>,
	typename Size = uint32,
	typename Alloc = mem::simple_new_allocator<fs::pair<Key, Value>>
>
using unordered_map = hash_table< 
	Key, Value, fs::pair<Key, Value>, 
	fs::get_first_t, fs::get_second_t, 
	MaxLoadFactor, Size, Alloc>;


ccdk_namespace_ct_end