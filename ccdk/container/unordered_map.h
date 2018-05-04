#pragma once

#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/util/destruct.h>
#include<ccdk/mpl/util/construct.h>
#include<ccdk/mpl/util/swap.h>
#include<ccdk/mpl/util/move.h>
#include<ccdk/mpl/fusion/pair.h>
#include<ccdk/mpl/iterator/reverse_iterator.h>
#include<ccdk/mpl/iterator/algorithm/distance.h>
#include<ccdk/memory/simple_new_allocator.h>
#include<ccdk/memory/allocator_traits.h>
#include<ccdk/container/container_mudule.h>
#include<ccdk/container/impl/hash_table.h>

ccdk_namespace_ct_start

using namespace mpl;

template<
	typename Key,
	typename Value,
	typename MaxLoadFactor = units::ratio<1,2>,
	typename Size = uint32,
	typename Alloc = mem::simple_new_allocator<fs::pair<Key,Value>>,
	typename HashContainer = hash_table<
		Key, Value, 
		fs::pair<Key,Value>,
		fs::get_first_t,
		fs::get_second_t,
		MaxLoadFactor, Size, Alloc>
>
class unordered_map: public HashContainer
{
public:
	using this_type       = unordered_map;
	using super_type      = HashContainer;
	using key_type        = typename super_type::key_type;
	using mapped_type     = typename super_type::mapped_type;
	using value_type      = typename super_type::value_type;
	using pointer         = typename super_type::pointer;
	using const_pointer   = typename super_type::const_pointer;
	using reference       = typename super_type::reference;
	using const_reference = typename super_type::const_reference;

	using iterator       = typename super_type::iterator;
	using const_iterator = typename super_type::const_iterator;

	//defualt and nullptr
	using super_type::super_type;
	using super_type::operator=;
	using super_type::swap;
	using super_type::insert;
	using super_type::erase;
	using super_type::find;
	using super_type::size;
	using super_type::empty;
};

ccdk_namespace_ct_end