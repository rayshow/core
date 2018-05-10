#pragma once

#include<ccdk/mpl/base/arithmetic_.h>
#include<ccdk/memory/memory_module.h>

ccdk_namespace_memory_start

using namespace mpl;

/*
	if allocated memory not over StackSize, just use stack memory
	else heap allocated from upstream Alloc, 
	note: only for continuous container
*/
template<uint32 KLeastElements, typename Alloc>
class semi_stack_allocator
{
public:
	using upstream_allocator = Alloc;
	using value_type = typename Alloc::value_type;
	using pointer    = typename Alloc::pointer;
	using size_type  = typename Alloc::size_type;

	template<typename U>
	using rebind = semi_stack_allocator<KLeastElements, U>;

private:
	uint8              stack_memory[KLeastElements*sizeof(value_type)];
	upstream_allocator upstream;
public:

	//first allocate stack size memory if size ok
	//then increase will use heap memory
	CCDK_FORCEINLINE pointer allocate(ptr::size_t n) {
		if (n == KLeastElements) return reinterpret_cast<pointer>( stack_memory);
		else upstream.allocate(n);
	}

	CCDK_FORCEINLINE pointer allocate(ptr::size_t n, const void* hint) { allocate(n); }
	CCDK_FORCEINLINE void deallocate(void* memory, ptr::size_t n) noexcept {
		if (memory != stack_memory) {
			upstream.deallocate(memory, n);
		}
	}
};

ccdk_namespace_memory_end