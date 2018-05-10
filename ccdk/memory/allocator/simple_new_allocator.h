#pragma once

#include<ccdk/memory/memory_module.h>

ccdk_namespace_memory_start

// allocator example
// just a wrap of ::new and ::delete
template<typename T, typename Size>
class simple_new_allocator
{
public:
	using value_type = T;
	using pointer    = T*;
	using size_type  = Size;

	template<typename U>
	using rebind = simple_new_allocator<U,Size>;

	//allocate n numbers of T
	CCDK_FORCEINLINE pointer allocate(size_type n) {
		ccdk_assert(n > 0); 
		return static_cast<pointer>(::operator new(n * sizeof(T)));
	}
	//allocate n numbers of T around hint
	CCDK_FORCEINLINE pointer allocate(size_type n, const void* hint) { 
		allocate(n); 
	}
	//deallocate n numbers of T memory
	CCDK_FORCEINLINE void deallocate(void* memory, size_type n) noexcept { 
		::operator delete(memory); 
	}
};



ccdk_namespace_memory_end