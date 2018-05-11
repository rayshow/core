#pragma once

#include<ccdk/container/container_mudule.h>
#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/fusion/pair.h>
#include<ccdk/memory/allocator/simple_new_allocator.h>
#include<ccdk/container/impl/rb_tree.h>

ccdk_namespace_ct_start

using namespace mpl;

template<
	typename Key,
	typename Value,
	typename Size = uint32,
	typename Alloc = mem::simple_new_allocator<fs::pair<Key, Value>, Size>,
	typename Super = rb_tree<Key,Value,fs::pair<Key,Value>,Size,Alloc>
>
class map: public Super
{

};



ccdk_namespace_ct_end