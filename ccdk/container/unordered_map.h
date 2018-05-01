#pragma once

#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/util/destruct.h>
#include<ccdk/mpl/util/construct.h>
#include<ccdk/mpl/util/swap.h>
#include<ccdk/mpl/util/move.h>
#include<ccdk/mpl/iterator/reverse_iterator.h>
#include<ccdk/mpl/iterator/algorithm/distance.h>
#include<ccdk/memory/simple_new_allocator.h>
#include<ccdk/memory/allocator_traits.h>
#include<ccdk/memory/list_allocate_adapter.h>
#include<ccdk/container/container_mudule.h>
#include<ccdk/container/impl/link_node.h>

ccdk_namespace_ct_start

template<
	typename Key,
	typename ValueType,
	typename SizeType = uint32,
	typename HashFn = util::hash_t<Key>,
	typename EqualFn = util::equal_t<Key>,




ccdk_namespace_ct_end