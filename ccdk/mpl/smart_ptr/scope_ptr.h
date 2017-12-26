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
	typedef scope_ptr type;
	typedef Type*     value_type;
	template<typename> friend class scope_ptr;
private:

	resource_base * content;

	// when new may throw, safe release and rethrow
	template<typename Type2, typename = check_t< is_convertible<Type2*, Type*> > >
	CCDK_FORCEINLINE void set_pointer(Type2* ptr) { ccdk_safe_delete_if_exception(content, new default_resource_base<Type2>{ ptr }); }

	template< typename Type2, typename Deleter>
	CCDK_FORCEINLINE void set_pointer(Type2* ptr, Deleter dl) { ccdk_safe_delete_if_exception(content, (new deleter_resource_base<Type2, Deleter>{ ptr, dl })); }

public:

	//default constructor
	CCDK_FORCEINLINE scope_ptr() noexcept : content{ nullptr } {}

	//nullptr constructor
	CCDK_FORCEINLINE scope_ptr(ptr::nullptr_t) noexcept : content{ nullptr } {}

	//pointer constructor,  safe process exception
	template<typename Type2, typename = check_t< is_convertible<Type2*,Type*> > >
	CCDK_FORCEINLINE scope_ptr(Type2* ptr) { set_pointer(ptr); }

	//pointer and deleter constructor, safe process exception
	template<typename Deleter, typename Type2, typename = check_t< is_convertible<Type2*, Type*> >>
	CCDK_FORCEINLINE scope_ptr(Type2* ptr, const Deleter& dl) { set_pointer(ptr, dl); }

	//move constructor, need check Type2* is convertible to Type*
	template<typename Type2, typename = check_t< is_convertible< Type2*, Type*> >  >
	CCDK_FORCEINLINE scope_ptr(scope_ptr<Type2>&& other) : content{ other.content } { other.content = nullptr; }

	//no copy
	template<typename Type2> scope_ptr(const scope_ptr<Type2>&) = delete;

	//no assign copy 
	template<typename Type2> scope_ptr& operator=(const scope_ptr<Type2>&) = delete;

	//no move assign
	template<typename Type2, typename = check_t< is_convertible< Type2*, Type*> >  >
	CCDK_FORCEINLINE scope_ptr& operator=(scope_ptr<Type2>&&) = delete;

	//swap 
	template<typename Type2>
	CCDK_FORCEINLINE void swap(scope_ptr<Type2>& other) noexcept { util::swap(content, other.content); }

	//destructor
	CCDK_FORCEINLINE ~scope_ptr() { ptr::safe_delete(content); }
	//release resource
	CCDK_FORCEINLINE void release() { ptr::safe_delete(content); }

	//get pointer
	CCDK_FORCEINLINE value_type pointer() const noexcept { ccdk_assert(content != nullptr); return (Type*)content->pointer(); }
	
	//dereference and member
	CCDK_FORCEINLINE add_lref_t<Type> operator*() const noexcept { ccdk_assert(pointer() != nullptr);  return *pointer(); }

	//member op
	CCDK_FORCEINLINE value_type operator->() const noexcept { return pointer(); }

	//exists
	CCDK_FORCEINLINE explicit operator bool() noexcept { return (nullptr != content) && (nullptr != content->pointer()); }


	//index
	CCDK_FORCEINLINE add_lref_t<Type>         operator[](uint32 index) noexcept { ccdk_assert(pointer() != nullptr);  return pointer()[index]; }
	CCDK_FORCEINLINE add_const_lref_t<Type>   operator[](uint32 index) const noexcept { ccdk_assert(pointer() != nullptr);  return pointer()[index]; }
};

	//general fn
	template<typename Type1, typename Type2> void swap(scope_ptr<Type1>& lh, scope_ptr<Type2>& rh) { lh.swap(rh); }
	template<typename Type> decltype(auto) value(const scope_ptr<Type>& sp) { return sp.pointer(); }
	

	//equal
	template<typename Type> CCDK_FORCEINLINE bool operator==(const scope_ptr<Type>& sp, ptr::nullptr_t) { return sp.pointer() == nullptr; }
	template<typename Type> CCDK_FORCEINLINE  bool operator==(ptr::nullptr_t, const scope_ptr<Type>& sp) { return sp.pointer() == nullptr; }
	template<typename Type1, typename Type2> CCDK_FORCEINLINE  bool operator==(const scope_ptr<Type1>& lh, const scope_ptr<Type2>& rh) { return lh.pointer() == rh.pointer(); }

	//less
	template<typename Type> CCDK_FORCEINLINE  bool operator<(ptr::nullptr_t, const scope_ptr<Type>& sp) { return nullptr < sp.pointer(); }
	template<typename Type> CCDK_FORCEINLINE  bool operator<(const scope_ptr<Type>& sp, ptr::nullptr_t) { return sp.pointer() < nullptr ; }
	template<typename Type1, typename Type2> CCDK_FORCEINLINE  bool operator<(const scope_ptr<Type1>& lh, const scope_ptr<Type2>& rh) { return lh.pointer() < rh.pointer(); }

ccdk_namespace_mpl_sp_end