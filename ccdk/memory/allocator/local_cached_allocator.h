#pragma once

#include<ccdk/memory/memory_module.h>

ccdk_namespace_memory_start

struct cache_node {
	cache_node * next;
};

//max 65535 element
template<uint32 Limit, typename T, typename Size>
class local_cache
{
public:
	using link_type = cache_node * ;
	using size_type = Size;
private:
	link_type head;
	uint16    len;
public:
	local_cache() noexcept : head{ nullptr }, len{ 0 } {}

	//if not reach limit, cache it
	bool push_block(T* memory) noexcept {
		if (len + 1 > Limit) return false;
		link_type node = reinterpret_cast<link_type>(memory);
		node->next = head;
		head = node;
		++len;
		return true;
	}

	T* pop_block() noexcept {
		if (len == 0) return nullptr;
		link_type node = head;
		head = node->next;
		--len;
		return reinterpret_cast<T*>(node);
	}

	bool empty() const noexcept { return len == 0; }
};

/*
	this is a mid-ware-allocator will hold a node cache (
		suitible for these fix-size-node contianer like list and tree )
	when cache is not full, deallocate will put memory block to it, and allocate
		will get memory block from it
	note:
		only allocate/deallocate 1 element is premit because cache is not continuous
*/
template<
	uint32 Limit,    //max nodes to cache
	typename Alloc   //upstream alloctor
>
class local_cached_allocator
{
public:
	using upstream_allocator = Alloc;
	using value_type         = typename Alloc::value_type;
	using pointer            = value_type* ;

	template<typename U>
	using rebind = local_cached_allocator<Limit, typename Alloc::template rebind<U> >;

private:
	local_cache<Limit, value_type, uint16>  cache;    //max 65535 node 
	upstream_allocator                      upsource; //upsource allocator

public:

	~local_cached_allocator() {
		pointer node{ nullptr };
		while (node = cache.pop_block()) {
			upsource.deallocate(node, 1);
		}
	}

	CCDK_FORCEINLINE auto allocate(ptr::size_t n) {
		//because cache is not continuous, only 1 node is permit
		ccdk_assert(n == 1);
		pointer ret{ nullptr };
		//cache is not empty
		if (ret = cache.pop_block()) {
			DebugValue("allocate from cache");
			return ret;
		}
		DebugValue("allocate from heap");
		return upsource.allocate(1);
	}

	CCDK_FORCEINLINE pointer allocate(ptr::size_t n, const void* hint) { allocate(n); }

	CCDK_FORCEINLINE void deallocate(pointer p, ptr::size_t n) {
		ccdk_assert(n == 1);
		if (!cache.push_block(p))
		{
			DebugValue("deallocate to heap");
			upsource.deallocate(p, 1);
			return;
		}
		DebugValue("deallocate to cache");
	}
};



ccdk_namespace_memory_end