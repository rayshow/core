#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/type_traits/is_convertible.h>
#include<ccdk/mpl/type_traits/is_pod.h>
#include<ccdk/mpl/type_traits/is_compatible.h>
#include<ccdk/mpl/base/enable_if.h>
#include<ccdk/mpl/util/noncopyable.h>
#include<ccdk/mpl/util/move.h>
#include<ccdk/mpl/util/swap.h>
#include<ccdk/mpl/util/addressof.h>
#include<ccdk/mpl/smart_ptr/default_deleter.h>

ccdk_namespace_mpl_sp_start


//static resource smart pointer
template<typename Type, typename Deleter = default_deleter >
struct unique_ptr :public util::noncopyable
{
	typedef unique_ptr type;
	typedef Type*      value_type;
	typedef Deleter    deleter_type;
private:
	value_type content;

public:
	CCDK_FORCEINLINE unique_ptr() noexcept : content{ nullptr } {}

	CCDK_FORCEINLINE unique_ptr(ptr::nullptr_t) noexcept : content{ nullptr } {}

	//from value pointer
	CCDK_FORCEINLINE unique_ptr(value_type ptr) noexcept : content(ptr) {}

	//move from compatible unique_ptr<Type2>
	template<typename Type2, typename = check_t< is_convertible<Type2*,Type*> > >
	CCDK_FORCEINLINE unique_ptr(unique_ptr<Type2>&& other) : content{ other.content } { other.content = nullptr; }


	template<typename Type2, typename = check_t< is_compatible<Type2*, Type*> > >
	CCDK_FORCEINLINE void swap(unique_ptr<Type2>& other)
	{
		util::swap(content, other.content);
	}

	//move assign
	CCDK_FORCEINLINE unique_ptr& operator=(value_type&& ptr) noexcept { unique_ptr{ ptr }.swap(*this); }

	//prevent self move
	CCDK_FORCEINLINE unique_ptr& operator=(unique_ptr&& other) { if (util::addressof(other) != this) { other.swap(*this); other.content = nullptr; } }

	//move from compatible unique_ptr<T2>
	template<typename T2, typename = check_t< is_convertible<T2*, T*> > >
	CCDK_FORCEINLINE unique_ptr& operator=(unique_ptr<T2>&& other)  { other.swap(*this); other.content = nullptr; }

	//non-copy
	template<typename P> unique_ptr(const unique_ptr<P>&) = delete;
	template<typename P> unique_ptr& operator=(const unique_ptr<P>&) = delete;

	//refer member
	CCDK_FORCEINLINE value_type operator->() noexcept { return content; }
	CCDK_FORCEINLINE const value_type operator->() const noexcept { return content; }

	//dereference, for efficient reason, assert not nullptr
	CCDK_FORCEINLINE add_lref_t<Type> operator*() & noexcept { ccdk_assert(content != nullptr);  return *content; }
	CCDK_FORCEINLINE add_const_lref_t<Type> operator*() const & noexcept { ccdk_assert(content != nullptr);  return *content; }
	CCDK_FORCEINLINE Type operator*() && noexcept{ ccdk_assert(content != nullptr);  return util::move(*content); }

	//pointer*
	CCDK_FORCEINLINE value_type pointer() noexcept { return content; }
	CCDK_FORCEINLINE const value_type pointer() const noexcept { return content; }

	//release
	CCDK_FORCEINLINE void release() noexcept { ptr::safe_delete(content); }

	//delete
	CCDK_FORCEINLINE ~unique_ptr() { ptr::safe_delete(content); }
};


//for array
// have index
template<typename Type, typename Deleter>
struct unique_ptr<Type[], Deleter> : public util::noncopyable
{
	typedef unique_ptr type;
	typedef Type*      value_type;
	typedef Deleter    deleter_type;
private:
	value_type content;
public:

	//default
	CCDK_FORCEINLINE unique_ptr() noexcept : content{ nullptr } {}
	CCDK_FORCEINLINE unique_ptr(ptr::nullptr_t) noexcept : content{ nullptr } {}
	//value 
	CCDK_FORCEINLINE unique_ptr(value_type ptr) noexcept : content(ptr) {}
	//move
	CCDK_FORCEINLINE unique_ptr(unique_ptr&& other) noexcept : content{ other.content } { other.content = nullptr; }

	CCDK_FORCEINLINE void swap(unique_ptr& other) noexcept { util::swap(content, other.content); }

	//assign move
	CCDK_FORCEINLINE unique_ptr& operator=(value_type ptr) { unique_ptr{ ptr }.swap(*this); }
	CCDK_FORCEINLINE unique_ptr& operator=(unique_ptr&& other) noexcept
	{
		if (util::addressof(other) != this) { content = other.content; other.content = nullptr; }
	}

	//no compatible move assign and copy / copy assign
	template<typename Type2, typename Deleter2> unique_ptr(const unique_ptr<Type2, Deleter2>&) = delete;
	template<typename Type2, typename Deleter2> unique_ptr& operator=(unique_ptr<Type2, Deleter2>&&) = delete;
	template<typename Type2, typename Deleter2> unique_ptr& operator=(const unique_ptr<Type2, Deleter2>&) = delete;
	

	//index
	CCDK_FORCEINLINE add_lref_t<type> operator[](int index)& { return content[index]; }
	CCDK_FORCEINLINE add_const_lref_t<type> operator[](int index) const & { return content[index]; }
	CCDK_FORCEINLINE Type operator[](int index) && { return util::move(content[index]); }

	//pointer*
	CCDK_FORCEINLINE value_type pointer() noexcept { return content; }
	CCDK_FORCEINLINE const value_type pointer() const noexcept { return content; }

	//release
	CCDK_FORCEINLINE void release() noexcept { ptr::safe_delete_array(content); }
	CCDK_FORCEINLINE ~unique_ptr() { ptr::safe_delete_array(content); }
};

//forbidden void content
template<> struct unique_ptr<void> : public util::noncopyable { public: unique_ptr() = delete; };

//halp fn
template<typename Type1, typename Type2, typename Deleter1, typename Deleter2> 
void swap(unique_ptr<Type1,Deleter1>& lh, unique_ptr<Type2, Deleter2>& rh) { lh.swap(rh); }
template<typename Type, typename Deleter> decltype(auto) value(const unique_ptr<Type, Deleter>& sp) { return sp.pointer(); }

//equal
template<typename Type, typename Deleter> CCDK_FORCEINLINE bool operator==(const unique_ptr<Type, Deleter>& sp, ptr::nullptr_t) { return sp.pointer() == nullptr; }
template<typename Type, typename Deleter> CCDK_FORCEINLINE bool operator==(ptr::nullptr_t, const unique_ptr<Type, Deleter>& sp) { return sp.pointer() == nullptr; }
template<typename Type1, typename Type2, typename Deleter1, typename Deleter2>
CCDK_FORCEINLINE  bool operator==(const unique_ptr<Type1, Deleter1>& lh, const unique_ptr<Type2, Deleter2>& rh) { return lh.pointer() == rh.pointer(); }

//less
template<typename Type, typename Deleter> CCDK_FORCEINLINE  bool operator<(ptr::nullptr_t, const unique_ptr<Type, Deleter>& sp) { return nullptr < sp.pointer(); }
template<typename Type, typename Deleter> CCDK_FORCEINLINE  bool operator<(const unique_ptr<Type, Deleter>& sp, ptr::nullptr_t) { return sp.pointer() < nullptr; }
template<typename Type1, typename Type2, typename Deleter1, typename Deleter2>
CCDK_FORCEINLINE  bool operator<(const unique_ptr<Type1, Deleter1>& lh, const unique_ptr<Type2, Deleter2>& rh) { return lh.pointer() < rh.pointer(); }

ccdk_namespace_mpl_sp_end