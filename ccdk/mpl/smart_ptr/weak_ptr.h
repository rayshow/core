#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/enable_if.h>
#include<ccdk/mpl/type_traits/is_compatible.h>
#include<ccdk/mpl/type_traits/is_convertible.h>
#include<ccdk/mpl/util/move.h>
#include<ccdk/mpl/smart_ptr/default_ref_count.h>
#include<ccdk/mpl/smart_ptr/default_deleter.h>
#include<ccdk/mpl/smart_ptr/smart_ptr_fwd.h>

ccdk_namespace_mpl_sp_start

template<
	typename T, 
	typename Deleter = default_deleter<T>,
	typename RefCount = default_ref_count<> 
>
class weak_ptr
{
public:
	typedef weak_ptr this_type;
	typedef RefCount ref_count_type;
	typedef share_ptr<T, Deleter, RefCount> share_type;
	typedef typename share_type::pointer_type pointer_type;
private:
	pointer_type      ptr;
	ref_count_type*   ref_count;
public:

	//default constructor
	CCDK_FORCEINLINE weak_ptr() noexcept : ptr{ nullptr }, ref_count{ nullptr } {}

	//nullptr constructor
	CCDK_FORCEINLINE weak_ptr(ptr::nullptr_t) noexcept : ptr{ nullptr }, ref_count{ nullptr } {}

	//from share_ptr<T2,D2,R2>
	template<typename T2, typename D2, typename R2, typename = check_t< is_convertible< share_ptr<T2,D2,R2>, share_type > > >
	CCDK_FORCEINLINE weak_ptr(const share_ptr<T2,D2,R2>& sp) noexcept : ptr{ sp.content }, ref_count{ sp.ref_count } { if (ref_count) ref_count->inc_ref_count(); }

	//copy from weak_ptr<T2,D2,R2>
	template< typename T2, typename D2, typename R2, typename = check_t< is_convertible< share_ptr<T2, D2, R2>, share_type > > >
	CCDK_FORCEINLINE weak_ptr(const weak_ptr<T2, D2, R2>& other) noexcept :ptr{ other.ptr }, ref_count{ other.ref_count } { if (ref_count) ref_count->inc_ref_count(); }
	
	//move from weak_ptr<T2,D2,R2>
	template< typename T2, typename D2, typename R2, typename = check_t< is_convertible< share_ptr<T2, D2, R2>, share_type > > >
	CCDK_FORCEINLINE weak_ptr(weak_ptr<T2, D2, R2>&& other) noexcept :ptr{ other.ptr }, ref_count{ other.ref_count } { other.ptr = nullptr; other.ref_count = nullptr; }

	//swap with weak_ptr<T2,D2,R2>
	template< typename T2, typename D2, typename R2, typename = check_t< is_compatible< share_ptr<T2, D2, R2>, share_type > > >
	CCDK_FORCEINLINE void swap(weak_ptr<T2, D2, R2>& other) noexcept { util::swap(ptr, other.ptr); util::swap(ref_count, other.ref_count); }

	//assign from share_ptr<T2,D2,R2>
	template< typename T2, typename D2, typename R2, typename = check_t< is_convertible< share_ptr<T2, D2, R2>, share_type > > >
	CCDK_FORCEINLINE weak_ptr& operator=(const share_ptr<T2, D2, R2>& sp)  noexcept { weak_ptr{sp}.swap(*this); return *this; }

	//copy assign, prove self assign
	template< typename T2, typename D2, typename R2, typename = check_t< is_convertible< share_ptr<T2, D2, R2>, share_type > > >
	CCDK_FORCEINLINE weak_ptr& operator=(const weak_ptr<T2, D2, R2 >& other) noexcept { weak_ptr{ other }.swap(*this); return *this; }

	//move assign
	template< typename T2, typename R2, typename = check_t< is_convertible< share_ptr<T2, D2,  R2>, share_type > > >
	CCDK_FORCEINLINE weak_ptr& operator=(weak_ptr<T2, R2 >&& other) { weak_ptr{util::move(other)}.swap(other); return *this; }

	// share count
	CCDK_FORCEINLINE uint32 share_count() const noexcept  { if (ref_count) return ref_count->share_count(); return 0; }

	// expired
	CCDK_FORCEINLINE bool expired() const noexcept { return share_count() == 0; }

	// lock 
	CCDK_FORCEINLINE share_type lock() const noexcept { return share_type{ *this }; }
};



ccdk_namespace_mpl_sp_end