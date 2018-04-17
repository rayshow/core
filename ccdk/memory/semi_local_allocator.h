#pragma once

#include<ccdk/memory/memory_module.h>
ccdk_namespace_memory_start

/*
	if allocated memory not over StackSize, just use stack memory
	else heap allocated from upstream Alloc
*/
template<uint32 StackSize, typename Alloc>
class semi_local_allocate_adapter
{
public:
	using value_type = Alloc::value_type;

	template<uint32 StackSize2, typename U>
	using rebind = semi_local_allocate_adapter<StackSize2,U>;

private:

public:

	CCDK_FORCEINLINE value_type* allocate(ptr::size_t n) {
		ccdk_assert(n > 0);
		return static_cast<T*>(::operator new(n * sizeof(T)));
	}


	CCDK_FORCEINLINE T* allocate(ptr::size_t n, const void* hint) { allocate(n); }
	void deallocate(void* memory, ptr::size_t n) noexcept { ::operator delete(memory); }
};

ccdk_namespace_memory_end