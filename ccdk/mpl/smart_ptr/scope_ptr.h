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


//resource smart pointer
// non-copy, only move
// construct specific resource recycle method
// destruct  recycle resource
// 
template<typename Type>
class scope_ptr: public util::noncopyable
{
public:
	typedef scope_ptr type;
private:

	resource_base * content;

	// when new may throw, safe release and rethrow
	CCDK_FORCEINLINE void set_pointer(Type* ptr) { ccdk_safe_release_if_exception(content, new default_resource_base<Type>{ ptr }); }

	template<typename Deleter>
	CCDK_FORCEINLINE void set_pointer(Type* ptr, Deleter dl) { ccdk_safe_release_if_exception(content, new deleter_resource_base<Type, Deleter>{ ptr, dl }); }

public:

	//default constructor
	CCDK_FORCEINLINE scope_ptr() noexcept : content{ nullptr } {}

	//nullptr constructor
	CCDK_FORCEINLINE scope_ptr(ptr::nullptr_t) noexcept : content{ nullptr } {}

	//pointer constructor,  safe process exception
	CCDK_FORCEINLINE scope_ptr(Type* ptr) { set_pointer(ptr); }

	//pointer and deleter constructor, safe process exception
	template<typename Deleter>
	CCDK_FORCEINLINE scope_ptr(Type* ptr, const Deleter& dl) { set_pointer(ptr, dl); }

	//move constructor, need check Type2* is convertible to Type*
	template<typename Type2, typename = check_t< is_convertible< Type2*, Type*> >  >
	CCDK_FORCEINLINE scope_ptr(scope_ptr<Type2>&& other) : content{ other.content } { other.content = nullptr; }

	//no copy
	template<typename Type2> scope_ptr(const scope_ptr<Type2>&) = delete;

	//no assign copy 
	template<typename Type2> scope_ptr& operator=(const scope_ptr<Type2>&) = delete;

	//swap 
	template<typename Type2, typename = check_t< is_convertible< Type2*, Type*> >  >
	CCDK_FORCEINLINE void swap(scope_ptr<Type2>& other) noexcept { util::swap(content, other.content); }

	//same type move assign, need check is same 
	CCDK_FORCEINLINE scope_ptr& operator=(scope_ptr&& other) { if (util::addressof(other) != this) { other.swap(*this); scope_ptr{}.swap(other); } }

	//diff type move assign, need check is convertible 
	template<typename Type2, typename = check_t< is_convertible< Type2*, Type*> >  >
	CCDK_FORCEINLINE scope_ptr& operator=(scope_ptr&& other) { other.swap(*this); scope_ptr{}.swap(other); }

	//destructor
	CCDK_FORCEINLINE ~scope_ptr() { ptr::safe_delete(content); }

	//get pointer
	CCDK_FORCEINLINE Type* pointer() noexcept  { (Type*)content->pointer(); }
	CCDK_FORCEINLINE const Type* pointer() const noexcept  { (Type*)content->pointer(); }

	//release resource
	CCDK_FORCEINLINE void release() { ptr::safe_delete(content); }

	//exists
	CCDK_FORCEINLINE explicit operator bool() noexcept { nullptr == content->pointer(); }

	//dereference
	CCDK_FORCEINLINE add_lref_t<Type>       operator*() noexcept { return *pointer(); }
	CCDK_FORCEINLINE add_const_lref_t<Type> operator*() const noexcept { return *pointer(); }

	//index
	CCDK_FORCEINLINE add_lref_t<Type>         operator[](uint32 index) noexcept { return pointer()[index]; }
	CCDK_FORCEINLINE add_const_lref_t<Type>   operator[](uint32 index) const noexcept { return pointer()[index]; }
};

ccdk_namespace_mpl_sp_end