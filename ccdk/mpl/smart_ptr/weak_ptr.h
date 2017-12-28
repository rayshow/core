#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/enable_if.h>
#include<ccdk/mpl/type_traits/is_compatible.h>
#include<ccdk/mpl/type_traits/is_convertible.h>
#include<ccdk/mpl/util/move.h>
#include<ccdk/mpl/smart_ptr/default_ref_count.h>
#include<ccdk/mpl/smart_ptr/default_deleter.h>
#include<ccdk/mpl/smart_ptr/smart_ptr_fwd.h>
#include<ccdk/mpl/smart_ptr/bad_weak_ptr.h>

ccdk_namespace_mpl_sp_start

template<
	typename T, 
	typename Deleter = default_deleter<T>,
	typename RefCount = default_ref_count<> 
>
class weak_ptr
{
public:
	typedef weak_ptr                          this_type;
	typedef RefCount                          ref_count_type;
	typedef share_ptr<T, Deleter, RefCount>   share_type;
	typedef typename share_type::pointer_type pointer_type;
	template<typename, typename, typename > friend class share_ptr_base;
private:
	pointer_type      ptr;
	RefCount*         ref_count;

	void inc_ref_count(){ if (ref_count) ref_count->inc_ref_count(); }
public:

	/* default and nullptr constructor */
	CCDK_FORCEINLINE weak_ptr() noexcept : ptr{ nullptr }, ref_count{ nullptr } {}
	CCDK_FORCEINLINE weak_ptr(ptr::nullptr_t) noexcept : ptr{ nullptr }, ref_count{ nullptr } {}

	/* copy from share_ptr/weak_ptr <T2,D2,R2>, inc ref count */
	template<typename T2, typename D2, typename R2, typename = check_t< is_convertible< share_ptr<T2, D2, R2>, share_type > > >
	CCDK_FORCEINLINE weak_ptr(const share_ptr<T2, D2, R2>& sp) noexcept : ptr{ sp.content }, ref_count{ sp.ref_count } { inc_ref_count(); }
	CCDK_FORCEINLINE weak_ptr(const weak_ptr& other) noexcept : ptr{ other.ptr }, ref_count{ other.ref_count } { inc_ref_count(); }
	template< typename T2, typename D2, typename R2, typename = check_t< is_convertible< share_ptr<T2, D2, R2>, share_type > > >
	CCDK_FORCEINLINE weak_ptr(const weak_ptr<T2, D2, R2>& other) noexcept :ptr{ other.ptr }, ref_count{ other.ref_count } { inc_ref_count(); }
	
	/* move from weak_ptr<T2,D2,R2> */
	CCDK_FORCEINLINE weak_ptr(weak_ptr&& other) noexcept : ptr{ other.ptr }, ref_count{ other.ref_count } { other.ptr = nullptr; other.ref_count = nullptr; }
	template< typename T2, typename D2, typename R2, typename = check_t< is_convertible< share_ptr<T2, D2, R2>, share_type > > >
	CCDK_FORCEINLINE weak_ptr(weak_ptr<T2, D2, R2>&& other) noexcept :ptr{ other.ptr }, ref_count{ other.ref_count } { other.ptr = nullptr; other.ref_count = nullptr; }

	/* swap with weak_ptr<T2,D2,R2> */
	template< typename T2, typename D2, typename R2, typename = check_t< is_compatible< share_ptr<T2, D2, R2>, share_type > > >
	CCDK_FORCEINLINE void swap(weak_ptr<T2, D2, R2>& other) noexcept { util::swap(ptr, other.ptr); util::swap(ref_count, other.ref_count); }

	/* copy assign, avoid self assign, assign from share_ptr/weak_ptr <T2,D2,R2> */
	template< typename T2, typename D2, typename R2, typename = check_t< is_convertible< share_ptr<T2, D2, R2>, share_type > > >
	CCDK_FORCEINLINE weak_ptr& operator=(const share_ptr<T2, D2, R2>& sp)  noexcept { weak_ptr{ sp }.swap(*this); return *this; }
	CCDK_FORCEINLINE weak_ptr& operator=(const weak_ptr& other) noexcept { ccdk_if_not_this(other) { weak_ptr{ other }.swap(*this); } return *this; }
	template< typename T2, typename D2, typename R2, typename = check_t< is_convertible< share_ptr<T2, D2, R2>, share_type > > >
	CCDK_FORCEINLINE weak_ptr& operator=(const weak_ptr<T2, D2, R2 >& other) noexcept { weak_ptr{ other }.swap(*this); return *this; }

	/* move assign, avoid self assign */
	CCDK_FORCEINLINE weak_ptr& operator=(weak_ptr&& other) { ccdk_if_not_this(other) { weak_ptr{ util::move(other) }.swap(other); } return *this; }
	template< typename T2, typename D2, typename R2, typename = check_t< is_convertible< share_ptr<T2, D2,  R2>, share_type > > >
	CCDK_FORCEINLINE weak_ptr& operator=(weak_ptr<T2,D2, R2 >&& other) { weak_ptr{util::move(other)}.swap(other); return *this; }

	/* share count */
	CCDK_FORCEINLINE uint32 share_count() const noexcept  { return ref_count ? ref_count->share_count : 0; }

	/* weak count */
	CCDK_FORCEINLINE uint32 weak_count() const noexcept { return ref_count ? ref_count->ref_count-1 : 0; }

	/* expired */
	CCDK_FORCEINLINE bool expired() const noexcept { return share_count() == 0; }
	  
	/* lock  */
	CCDK_FORCEINLINE share_type lock() const noexcept { if (ptr) { return share_type{ *this }; } ccdk_throw(bad_weak_ptr{}); }

	/* decrease ref count */
	CCDK_FORCEINLINE ~weak_ptr() { if (ref_count) ref_count->dec_ref_count(); }
};

ccdk_namespace_mpl_sp_end