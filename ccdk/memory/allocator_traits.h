#pragma once

#include<ccdk/memory/memory_module.h>
#include<ccdk/mpl/base/derive_if.h>
#include<ccdk/mpl/base/identity.h>
#include<ccdk/mpl/type_traits/has_inner_type.h>

ccdk_namespace_memory_start

/*
	if   T::size_type exists, type = T::size_type
	else type = ptr::size_type
*/
template<typename T, bool = mpl::has_nest_size_type_v<T>>
struct get_size_type { typedef typename T::size_type type;};
template<typename T> struct get_size_type<T, false> { typedef ptr::size_t type; };

/* 
	if   T::different_type exists, type = T::different_type
	else type = ptr::diff_t
*/
template<typename T, bool = mpl::has_nest_different_type_v<T>>
struct get_diff_type { typedef typename T::different_type type; };
template<typename T> struct get_diff_type<T, false> { typedef ptr::diff_t type; };

/*
	allocate memory of n * increase_ratio 
*/
#define ccdk_increase_allocate1(n, content)                                       \
	size_type actual_size = fn::max(kLeastElements,increase_ratio::multiply(n));  \
	if (n == size_type(-1) || actual_size < n) throw std::bad_alloc{};            \
	content = allocator_type::allocate(*this, actual_size );	                  

#define ccdk_increase_allocate2(n, content, cap)                          \
	ccdk_increase_allocate1(n,content)                                    \
	cap = actual_size;

#define ccdk_increase_allocate3(n, content, cap, len)                     \
	ccdk_increase_allocate2(n,content,cap)                                \
	len = n;                                                               

/*
	uniform interface between user and allocator
*/
template<typename Alloc>
class allocator_traits
{
public:
	using value_type = typename Alloc::value_type;
	using pointer = value_type * ;
	using const_pointer = value_type const*;
	using reference = value_type &;
	using const_reference = value_type const&;
	using size_type = typename get_size_type<Alloc>::type ;
	using difference_type = to_signed_t<size_type>;

	template<typename P>
	using rebind = allocator_traits<P>;

	CCDK_FORCEINLINE static pointer allocate(Alloc& alloc, size_type n) {
		ccdk_assert(n > 0);
		return static_cast<pointer>(alloc.allocate(n)); 
	}
	CCDK_FORCEINLINE static pointer allocate(Alloc& alloc, size_type n, const void* hint){
		if (n == 0) return nullptr;
		return static_cast<pointer>(alloc.allocate(n, hint)); 
	}

	CCDK_FORCEINLINE static void deallocate(Alloc& alloc, pointer& p, size_type n) {
		sizeof(*p); /* check imcomplete type */
		if(p) alloc.deallocate(p,n); 
		p = nullptr;
	}

	CCDK_FORCEINLINE static constexpr size_type max_allocate_size() noexcept { 
		return size_type(-1) / sizeof(value_type); 
	}
};


ccdk_namespace_memory_end