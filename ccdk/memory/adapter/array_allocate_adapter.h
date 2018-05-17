#pragma once

#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/base/compatible_op.h>
#include<ccdk/mpl/type_traits/declval.h>
#include<ccdk/mpl/type_traits/has_attribute.h>
#include<ccdk/mpl/function/operator.h>
#include<ccdk/mpl/fusion/pair.h>
#include<ccdk/memory/memory_module.h>
#include<ccdk/memory/allocator_traits.h>

ccdk_namespace_memory_start

using namespace ccdk::mpl;

template<uint32 LeastCount, typename Ratio, typename Alloc>
class array_allocate_adapter
{
public:
	using upstream_adapter = allocator_traits<Alloc>;
	using value_type      = typename upstream_adapter::value_type;
	using pointer         = typename upstream_adapter::pointer;
	using size_type       = typename upstream_adapter::size_type;
	using difference_type = typename upstream_adapter::difference_type;
	using allocate_ratio  = Ratio;

	// rebind to another T
	template<typename U>
	using rebind = array_allocate_adapter<LeastCount,
		Ratio, typename Alloc::template rebind<U>>;

	// forward to upstream
	static auto deallocate(Alloc &alloc, pointer address, size_type n) noexcept {
		return upstream_adapter::deallocate(alloc, address, n);
	}

	static pointer allocate(Alloc& alloc, size_type n, size_type& cap) {
		size_type actual_size = fn::max(LeastCount, n);
		pointer memory = upstream_adapter::allocate(alloc, actual_size);
		cap = actual_size;
		return memory;
	}

	// ext-allocate function, allocate memory and check weather overflow
	// set capacity and length 
	static pointer allocate_inc(Alloc& alloc, size_type n, size_type* cap, size_type* len) {
		//compute actually allocating size
		size_type actual_size = fn::max(LeastCount, allocate_ratio::multiply(n));
		// size overflow
		if (n == size_type(-1) || cge<difference_type>(n, actual_size))
			throw std::bad_alloc{};
		//allocate
		pointer memory = upstream_adapter::allocate(alloc, actual_size);
		//allocate success
		if (cap) *cap = actual_size;
		if (len) *len = n;
		return memory;
	}

	static pointer allocate_inc(Alloc& alloc, size_type n, size_type* cap) {
		return allocate_inc(alloc, n, cap, nullptr);
	}

	static pointer allocate_inc(Alloc& alloc, size_type n) {
		return allocate_inc(alloc, n, nullptr, nullptr);
	}

	CCDK_FORCEINLINE static constexpr size_type max_allocate_size() noexcept {
		return size_type(-1) / sizeof(value_type);
	}
};

ccdk_namespace_memory_end