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


//shared   resource smart pointer
//  same as std::share_ptr
template<typename Type, typename RefCount = default_ref_count<> >
class poly_share_ptr
{
public:
	typedef poly_share_ptr type;
	typedef RefCount       ref_count_type;
	typedef Type*          value_type;
	template<typename, typename> friend class poly_share_ptr;
	template<typename, typename> friend class poly_weak_ptr;
private:
	RefCount*       ref_count;
	resource_base*  content;

	//clean resource after new exception
	CCDK_FORCEINLINE void clean_up() {  ptr::safe_delete(ref_count); ptr::safe_delete(content); }

	// when new may throw, safe clean up and rethrow
	template<typename Type2, typename = check_t< is_convertible<Type2*, Type*> > >
	void set_pointer(Type2* ptr)  {  ccdk_safe_cleanup_if_exception( (ref_count = new RefCount{ 1,1 }, content = new default_resource_base<Type2>{ ptr }), clean_up() ); }

	template< typename Type2, typename Deleter>
	void set_pointer(Type2* ptr, Deleter dl)  {  ccdk_safe_cleanup_if_exception( (ref_count = new RefCount{ 1,1 }, content = new deleter_resource_base<Type2, Deleter>{ ptr, dl }), clean_up() ); }

public:
	//default
	CCDK_FORCEINLINE poly_share_ptr() noexcept : ref_count { nullptr }, content{ nullptr }  {}
	//nullptr
	CCDK_FORCEINLINE explicit poly_share_ptr(ptr::nullptr_t) noexcept : ref_count{nullptr}, content{ nullptr } {}

	//value constructor may throw
	template<typename Type2, typename = check_t< is_convertible<Type2*,Type*>>>
	explicit poly_share_ptr(Type2* ptr) { set_pointer(ptr); }

	template<typename Type2, typename Deleter, typename = check_t< is_convertible<Type2*, Type*>>>
	poly_share_ptr(Type2* ptr, const Deleter& dl) { set_pointer(ptr,dl); }

	//copy constructor
	CCDK_FORCEINLINE poly_share_ptr(const poly_share_ptr& other) noexcept : ref_count{ other.ref_count }, content{ other.content } { if (ccdk_likely(ref_count)) ref_count->inc_share_count(); }
	template< typename Type2, typename RefCount2, typename = check_t< is_convertible<poly_share_ptr<Type2, RefCount2>, type>>   >
	CCDK_FORCEINLINE poly_share_ptr(const poly_share_ptr<Type2, RefCount2>& other) noexcept : ref_count{other.ref_count}, content{ other.content } { if ( ccdk_likely(ref_count)) ref_count->inc_share_count(); }
	template< typename Type2, typename RefCount2, typename = check_t< is_convertible<poly_share_ptr<Type2, RefCount2>, type>>   >
	CCDK_FORCEINLINE poly_share_ptr(const poly_weak_ptr<Type2, RefCount2>& other) noexcept : ref_count{ other.ref_count }, content{ other.content } { if (ccdk_likely(ref_count)) ref_count->inc_share_count(); }

	//move constructor
	template< typename Type2, typename RefCount2, typename = check_t< is_convertible<poly_share_ptr<Type2, RefCount2>, type>> >
	CCDK_FORCEINLINE poly_share_ptr(poly_share_ptr<Type2, RefCount2>&& other) noexcept  : ref_count{ other.ref_count }, content{ other.content } { other.content = nullptr; other.ref_count = nullptr; }

	//swap when 2 sp is compatible
	template< typename Type2, typename RefCount2, typename = check_t< is_compatible<poly_share_ptr<Type2, RefCount2>, type>> >
	CCDK_FORCEINLINE void swap(poly_share_ptr<Type2, RefCount2>& other) noexcept {  util::swap(ref_count, other.ref_count); util::swap(content, other.content); }

	//nullptr assign
	CCDK_FORCEINLINE poly_share_ptr& operator=(ptr::nullptr_t) noexcept { poly_share_ptr{}.swap(*this); return *this; }
	//pointer assign, may throw
	CCDK_FORCEINLINE poly_share_ptr& operator=(value_type ptr)  { poly_share_ptr{ ptr }.swap(*this); return *this; }
	//copy self assign, forbidden self assign
	CCDK_FORCEINLINE poly_share_ptr& operator=(const poly_share_ptr& other) noexcept { if ( ccdk_likely( util::addressof(other) != this)) { poly_share_ptr{ other }.swap(*this); } return *this; }
	//move self assign, forbindden self assign
	CCDK_FORCEINLINE poly_share_ptr& operator=(poly_share_ptr&& other) noexcept { if (ccdk_likely( util::addressof(other) != this)) { poly_share_ptr{ util::move(other) }.swap(*this); } return *this; }

	//copy assign comptible sp
	template< typename Type2, typename RefCount2, typename = check_t< is_convertible<poly_share_ptr<Type2, RefCount2>, type>> >
	CCDK_FORCEINLINE poly_share_ptr& operator=(const poly_share_ptr<Type2, RefCount2>& other) { poly_share_ptr{ other }.swap(*this); return *this; }
	//move assign comptible sp
	template< typename Type2, typename RefCount2, typename = check_t< is_convertible<poly_share_ptr<Type2, RefCount2>, type>> >
	CCDK_FORCEINLINE poly_share_ptr& operator=(poly_share_ptr<Type2, RefCount2>&& other) { poly_share_ptr{ util::move(other) }.swap(*this); return *this; }

	//reset, may throw
	CCDK_FORCEINLINE void reset() { poly_share_ptr{}.swap(*this); }

	CCDK_FORCEINLINE void reset(ptr::nullptr_t) { poly_share_ptr{}.swap(*this); }

	template<typename Type2, typename = check_t< is_convertible<Type2*, Type*>>>
	CCDK_FORCEINLINE void reset(Type2* ptr) { poly_share_ptr{ ptr }.swap(*this); }

	template<typename Type2, typename Deleter, typename = check_t< is_convertible<Type2*, Type*>>>
	CCDK_FORCEINLINE void reset(Type2* ptr, const Deleter& dl) { poly_share_ptr{ ptr, dl }.swap(*this); }


	//get pointer
	CCDK_FORCEINLINE value_type pointer() const noexcept  { return content ? (value_type)content->pointer() : nullptr; }

	//member access
	CCDK_FORCEINLINE value_type operator->() const noexcept { return pointer(); }

	//dereference 
	CCDK_FORCEINLINE add_lref_t<Type> operator*() const noexcept { ccdk_assert(content != nullptr); return *(value_type)content->pointer(); }

	//bool
	CCDK_FORCEINLINE explicit operator bool() const noexcept { return ref_count && ref_count->share_count > 0; }

	//share count
	CCDK_FORCEINLINE uint32 share_count() const noexcept { return ref_count ? ref_count->share_count : 0; }

	//destructor
	CCDK_FORCEINLINE ~poly_share_ptr() { if (ccdk_likely(ref_count)) { ref_count->dec_share_count(default_deleter<resource_base>{}, content); } }
};


	//general fn
	template<typename T1, typename T2, typename R1, typename R2> void swap(poly_share_ptr<T1,R1>& lh, poly_share_ptr<T2,R2>& rh) { lh.swap(rh); }
	template<typename T> decltype(auto) value(const poly_share_ptr<T>& sp) { return sp.pointer(); }

	//equal
	template<typename T, typename R> CCDK_FORCEINLINE  bool operator==(const poly_share_ptr<T,R>& sp, ptr::nullptr_t) { return sp.pointer() == nullptr; }
	template<typename T, typename R> CCDK_FORCEINLINE  bool operator==(ptr::nullptr_t, const poly_share_ptr<T,R>& sp) { return sp.pointer() == nullptr; }
	template<typename T1, typename T2, typename R1, typename R2> CCDK_FORCEINLINE  bool operator==(const poly_share_ptr<T1,R1>& lh, const poly_share_ptr<T2,R2>& rh) { return lh.pointer() == rh.pointer(); }

	//less
	template<typename T, typename R> CCDK_FORCEINLINE  bool operator<(ptr::nullptr_t, const poly_share_ptr<T,R>& sp) { return nullptr < sp.pointer(); }
	template<typename T, typename R> CCDK_FORCEINLINE  bool operator<(const poly_share_ptr<T,R>& sp, ptr::nullptr_t) { return sp.pointer() < nullptr; }
	template<typename T1, typename T2, typename R1, typename R2> CCDK_FORCEINLINE  bool operator<(const poly_share_ptr<T1,R1>& lh, const poly_share_ptr<T2,R2>& rh) { return lh.pointer() < rh.pointer(); }


ccdk_namespace_mpl_sp_end