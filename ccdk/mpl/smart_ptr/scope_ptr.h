#pragma once

#include<ccdk/mpl/mpl_module.h>

#include<ccdk/mpl/base/enable_if.h>
#include<ccdk/mpl/type_traits/is_convertible.h>
#include<ccdk/mpl/type_traits/add_lref.h>
#include<ccdk/mpl/type_traits/add_const_lref.h>
#include<ccdk/mpl/util/swap.h>
#include<ccdk/mpl/util/addressof.h>
#include<ccdk/mpl/smart_ptr/resource_base.h>

ccdk_namespace_mpl_sp_start

//simple resource smart pointer
// no copy ctor / copy assign / move assign , only value and move constructor
template<typename Type>
class scope_ptr: public util::noncopyable
{
public:
	typedef scope_ptr  this_type;
	typedef Type       value_type;
	typedef Type*      pointer_type;
	template<typename> friend class scope_ptr;
private:

	resource_base * content;

	/* internal call, when new may throw, safe release and rethrow */
	template<typename T2>
	CCDK_FORCEINLINE void set_pointer(T2* ptr) { ccdk_safe_delete_if_exception(content, new default_resource_base<T2>{ ptr }); }

	template< typename T2, typename D>
	CCDK_FORCEINLINE void set_pointer(T2* ptr, D dl) { ccdk_safe_delete_if_exception(content, (new deleter_resource_base<T2, D>{ ptr, dl })); }

public:

	/* default and nullptr constructor */
	CCDK_FORCEINLINE scope_ptr() noexcept : content{ nullptr } {}
	CCDK_FORCEINLINE scope_ptr(ptr::nullptr_t) noexcept : content{ nullptr } {}

	/* pointer constructor, may throw std::bad_alloc */
	template<typename T2, typename = check_t< is_convertible<T2*, pointer_type> > >
	CCDK_FORCEINLINE scope_ptr(T2* ptr) { set_pointer(ptr); }
	template<typename T2, typename D,  typename = check_t< is_convertible<T2*, pointer_type> >>
	CCDK_FORCEINLINE scope_ptr(T2* ptr, const D& dl) { set_pointer(ptr, dl); }

	/* no template copy and template assign copy */
	template<typename T2> scope_ptr(const scope_ptr<T2>&) = delete;
	template<typename T2> scope_ptr& operator=(const scope_ptr<T2>&) = delete;

	/* move constructor, check compatible  */
	CCDK_FORCEINLINE scope_ptr(scope_ptr&& other) noexcept: content{ other.content } { other.content = nullptr; }
	template<typename T2, typename = check_t< is_convertible< T2*, pointer_type> >  >
	CCDK_FORCEINLINE scope_ptr(scope_ptr<T2>&& other) noexcept : content{ other.content } { other.content = nullptr; }


	/* move assign, avoid self assign, destruct old pointer may throw */
	CCDK_FORCEINLINE scope_ptr& operator=(scope_ptr&& other) { ccdk_if_not_this(other){ scope_ptr{ util::move(other) }.swap(*this); } return *this; }
	template<typename T2, typename = check_t< is_convertible< T2*, pointer_type> >  >
	CCDK_FORCEINLINE scope_ptr& operator=(scope_ptr<T2>&& other) { scope_ptr{ util::move(other) }.swap(*this); }

	/* swap  */
	template<typename T2, typename = check_t< is_compatible< T2*, pointer_type> >>
	CCDK_FORCEINLINE void swap(scope_ptr<T2>& other) noexcept { util::swap(content, other.content); }

	/* reset, pointer version may throw std::bad_alloc */
	CCDK_FORCEINLINE void reset() noexcept  { ptr::safe_delete(content); }
	CCDK_FORCEINLINE void reset(ptr::nullptr_t) noexcept { reset(); }
	CCDK_FORCEINLINE void reset(pointer_type ptr) { scope_ptr{ ptr }.swap(*this); }

	/* release hold, amd return pointer */
	CCDK_FORCEINLINE pointer_type release() { if (ccdk_likely(content)) { pointer_type ret = (pointer_type) content->release(); reset(); return ret; } return nullptr; }

	/* get pointer */
	CCDK_FORCEINLINE pointer_type pointer() const noexcept { if (ccdk_likely(content)) { return (pointer_type)content->pointer(); } return nullptr; }
	
	/* dereference and member */
	CCDK_FORCEINLINE add_lref_t<Type> operator*() const noexcept { ccdk_assert(pointer() != nullptr);  return *pointer(); }
	CCDK_FORCEINLINE pointer_type     operator->() const noexcept { return pointer(); }

	/* exists */
	CCDK_FORCEINLINE explicit operator bool() noexcept { return pointer()!=nullptr; }

	/* index */
	CCDK_FORCEINLINE add_lref_t<Type>  operator[](uint32 index) const noexcept { return pointer()[index]; }

	/* destructor */
	CCDK_FORCEINLINE ~scope_ptr() { reset(); }
};

	/* general fn */
	template<typename T1, typename T2, typename = check_t< is_compatible< typename scope_ptr<T1>::pointer_type, typename scope_ptr<T2>::pointer_type> >>
	void swap(scope_ptr<T1>& lh, scope_ptr<T2>& rh) { lh.swap(rh); }
	template<typename T> decltype(auto) value(const scope_ptr<T>& sp) { return sp.pointer(); }
	
	/* equal */
	template<typename T> CCDK_FORCEINLINE bool operator==(const scope_ptr<T>& sp, ptr::nullptr_t) { return sp.pointer() == nullptr; }
	template<typename T> CCDK_FORCEINLINE  bool operator==(ptr::nullptr_t, const scope_ptr<T>& sp) { return sp.pointer() == nullptr; }
	template<typename T1, typename T2> CCDK_FORCEINLINE  bool operator==(const scope_ptr<T1>& lh, const scope_ptr<T2>& rh) { return lh.pointer() == rh.pointer(); }

	/* less */
	template<typename T> CCDK_FORCEINLINE  bool operator<(ptr::nullptr_t, const scope_ptr<T>& sp) { return nullptr < sp.pointer(); }
	template<typename T> CCDK_FORCEINLINE  bool operator<(const scope_ptr<T>& sp, ptr::nullptr_t) { return sp.pointer() < nullptr ; }
	template<typename T1, typename T2> CCDK_FORCEINLINE  bool operator<(const scope_ptr<T1>& lh, const scope_ptr<T2>& rh) { return lh.pointer() < rh.pointer(); }

ccdk_namespace_mpl_sp_end