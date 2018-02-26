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
template<typename T, bool = mpl::has_inner_size_type_v<T>>
struct get_size_type { typedef typename T::size_type type;};
template<typename T> struct get_size_type<T, false> { typedef ptr::size_t type; };

/* 
	if   T::different_type exists, type = T::different_type
	else type = ptr::diff_t
*/
template<typename T, bool = mpl::has_inner_different_type_v<T>>
struct get_diff_type { typedef typename T::different_type type; };
template<typename T> struct get_diff_type<T, false> { typedef ptr::diff_t type; };

/*
	uniform interface between user and allocator
*/
template<typename Alloc>
class allocator_traits
{
public:
	typedef Alloc                               allocator_type;
	typedef typename Alloc::value_type          value_type;
	typedef value_type*                         pointer;
	typedef value_type const*                   const_pointer;
	typedef value_type&                         reference;
	typedef value_type const&                   const_reference;
	typedef typename get_size_type<Alloc>::type size_type;
	typedef typename get_diff_type<Alloc>::type different_type;
	template<typename P>
	using rebind = Alloc::template rebind<P>;

	CCDK_FORCEINLINE static pointer allocate(allocator_type& alloc, size_type n) 
	{
		if (n == 0) return nullptr;
		return static_cast<pointer>(alloc.allocate(n)); 
	}
	CCDK_FORCEINLINE static pointer allocate(allocator_type& alloc, size_type n, const void* hint)
	{
		if (n == 0) return nullptr;
		return static_cast<pointer>(alloc.allocate(n, hint)); 
	}

	CCDK_FORCEINLINE static void deallocate(allocator_type& alloc, pointer& p, size_type n) 
	{
		sizeof(*p); /* check imcomplete type */
		if(p!=nullptr) alloc.deallocate(p,n); 
		p = nullptr;
	}

	CCDK_FORCEINLINE static constexpr size_type max_allocate_size() noexcept { return size_type(-1) / sizeof(value_type); }
};


ccdk_namespace_memory_end