#pragma once

#include<ccdk/memory/memory_module.h>

ccdk_namespace_memory_start

/*
	just a wrap of ::new and ::delete
*/
template<typename T>
class simple_new_allocator
{
public:
	typedef T                     value_type;
	template<typename U>
	using rebind = simple_new_allocator<U>;

	CCDK_FORCEINLINE void* allocate(ptr::size_t n) { ccdk_assert(n > 0); return ::operator new(n * sizeof(T)); }
	CCDK_FORCEINLINE void* allocate(ptr::size_t n, const void* hint) { allocate(n); }
	void deallocate(void* memory, ptr::size_t n) noexcept { ::operator delete(memory); }
};



ccdk_namespace_memory_end