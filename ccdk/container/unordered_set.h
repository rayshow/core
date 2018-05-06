#pragma once

#include<ccdk/mpl/function/identity.h>
#include<ccdk/container/container_mudule.h>
#include<ccdk/container/impl/hash_table.h>

ccdk_namespace_ct_start

using namespace mpl;

template<
	typename T,
	typename MaxLoadFactor = units::ratio<1, 2>,
	typename Size = uint32,
	typename Alloc = mem::simple_new_allocator<T>,
	typename Container = hash_table<
		T, T, T, fn::identity_t,
		fn::identity_t, MaxLoadFactor, Size, Alloc>
>
class unordered_set
{
public:
	
private:
	Container content;

public:

	CCDK_FORCEINLINE  insert(T const& t) { content.insert(t); }

};


ccdk_namespace_ct_end