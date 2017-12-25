#pragma once

#include<ccdk/mpl/mpl_module.h>

#include<ccdk/mpl/base/enable_if.h>
#include<ccdk/mpl/type_traits/is_convertible.h>
#include<ccdk/mpl/type_traits/add_lref.h>
#include<ccdk/mpl/type_traits/add_const_lref.h>
#include<ccdk/mpl/util/swap.h>
#include<ccdk/mpl/util/addressof.h>
#include<ccdk/mpl/smart_ptr/resource_base.h>
#include<ccdk/mpl/smart_ptr/default_ref_count.h>
#include<ccdk/mpl/smart_ptr/smart_ptr_fwd.h>

ccdk_namespace_mpl_sp_start

template<typename Type, typename RefCount = default_ref_count<> >
class poly_weak_ptr
{
public:
	typedef poly_weak_ptr                  type;
	typedef RefCount                       ref_count_type;
	typedef poly_share_ptr<Type, RefCount> share_type;
	template<typename, typename> friend class poly_share_ptr;
	template<typename, typename> friend class poly_weak_ptr;
private:
	RefCount*       ref_count;
	resource_base*  content;

public:
	//default and nullptr_t constructor
	CCDK_FORCEINLINE poly_weak_ptr() noexcept : ref_count{ nullptr }, content{ nullptr } {}
	CCDK_FORCEINLINE poly_weak_ptr(ptr::nullptr_t) noexcept : ref_count{ nullptr }, content{ nullptr } {}

	//from share_ptr
	template<typename Type2, typename RefCount2, typename = check_t< is_convertible<poly_share_ptr<Type2, RefCount2>, share_type>> >
	CCDK_FORCEINLINE poly_weak_ptr(const poly_share_ptr<Type2, RefCount2>& sp) noexcept : ref_count{ sp.ref_count }, content{ sp.content } { if (ccdk_likely(ref_count)) { DebugValue("inc"); ref_count->inc_ref_count(); }; }
	
	//copy 
	CCDK_FORCEINLINE poly_weak_ptr(const poly_weak_ptr& other)noexcept : ref_count{ other.ref_count }, content{ other.content } { if (ccdk_likely(ref_count)) { DebugValue("inc"); ref_count->inc_ref_count(); }; }
	template<typename Type2, typename RefCount2, typename = check_t< is_convertible<poly_share_ptr<Type2, RefCount2>, share_type>> >
	CCDK_FORCEINLINE poly_weak_ptr(const poly_weak_ptr<Type2, RefCount2>& other) noexcept : ref_count{ other.ref_count }, content{ other.content } { if (ccdk_likely(ref_count)) { DebugValue("inc"); ref_count->inc_ref_count(); }; }


	template<typename Type2, typename RefCount2, typename = check_t< is_compatible<poly_share_ptr<Type2, RefCount2>, share_type>> >
	CCDK_FORCEINLINE void swap(poly_weak_ptr<Type2, RefCount2>& other) noexcept {  util::swap(ref_count, other.ref_count); util::swap(content, other.content); }

	//assign nullptr
	CCDK_FORCEINLINE poly_weak_ptr& operator=(ptr::nullptr_t) noexcept { poly_weak_ptr{}.swap(*this); }

	CCDK_FORCEINLINE poly_weak_ptr& operator=(const poly_weak_ptr& other) {}

	//assign from share
	template<typename Type2, typename RefCount2, typename = check_t< is_compatible<poly_share_ptr<Type2, RefCount2>, share_type>> >
	CCDK_FORCEINLINE poly_weak_ptr& operator=(const poly_share_ptr<Type2, RefCount2>& sp) noexcept { poly_weak_ptr{ sp }.swap(*this); return *this; }

	//copy assign 
	template<typename Type2, typename RefCount2, typename = check_t< is_compatible<poly_share_ptr<Type2, RefCount2>, share_type>> >
	CCDK_FORCEINLINE poly_weak_ptr& operator=(const poly_weak_ptr<Type2, RefCount2>& other) noexcept { poly_weak_ptr{ other }.swap(*this); return *this; }

	//share memory is expired
	CCDK_FORCEINLINE bool expired() const { return !ref_count || ref_count->share_count == 0; }

	//lock
	CCDK_FORCEINLINE share_type lock() const { return share_type{ *this }; }

	//shared count
	CCDK_FORCEINLINE uint32 share_count() const { return ref_count ? ref_count->share_count : 0; }

	//ref count
	CCDK_FORCEINLINE uint32 weak_count() const { return ref_count ? ref_count->ref_count : 0; }

	//
	CCDK_FORCEINLINE ~poly_weak_ptr() { if (ccdk_likely(ref_count)) ref_count->dec_ref_count(); }
};


ccdk_namespace_mpl_sp_end