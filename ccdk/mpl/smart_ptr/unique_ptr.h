#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/type_traits/is_convertible.h>
#include<ccdk/mpl/type_traits/is_pod.h>
#include<ccdk/mpl/base/enable_if.h>
#include<ccdk/mpl/util/noncopyable.h>
#include<ccdk/mpl/util/move.h>
#include<ccdk/mpl/util/swap.h>
#include<ccdk/mpl/util/addressof.h>

ccdk_namespace_mpl_sp_start

template<typename T>
struct unique_ptr :public util::noncopyable
{
	typedef unique_ptr type;
	typedef T* value_type;
private:
	value_type content; 

public:
	//from value pointer
	CCDK_FORCEINLINE unique_ptr(value_type ptr) noexcept : content(ptr) {}

	//move from compatible unique_ptr<T2>
	template<typename T2, typename = check_t< is_convertible<T2*,T*> > >
	CCDK_FORCEINLINE unique_ptr(unique_ptr<T2>&& other) : content{ other.content } { other.content = nullptr; }

	template<typename T2, typename = check_t< is_convertible<T2*, T*> > >
	CCDK_FORCEINLINE void swap(unique_ptr<T2>& other)
	{
		util::swap(content, other);
	}

	//prevent self move
	CCDK_FORCEINLINE unique_ptr& operator=(unique_ptr&& other) { if (util::addressof(other) != this) { other.swap(*this); other.content = nullptr; } }

	//move from compatible unique_ptr<T2>
	template<typename T2, typename = check_t< is_convertible<T2*, T*> > >
	CCDK_FORCEINLINE unique_ptr& operator=(unique_ptr<T2>&& other) 
	{
		other.swap(*this);
		other.content = nullptr;
	}

	//non-copy
	template<typename P> unique_ptr(const unique_ptr<P>&) = delete;
	template<typename P> unique_ptr& operator=(const unique_ptr<P>&) = delete;

	//refer member
	T* operator->() noexcept { return content; }
	const T* operator->() const noexcept { return content; }

	//dereference, for efficient reason, assert not nullptr
	T& operator*() & noexcept { ccdk_assert(content != nullptr);  return *content; }
	const T& operator*() const & noexcept { ccdk_assert(content != nullptr);  return *content; }
	T&& operator*() && noexcept{ ccdk_assert(content != nullptr);  return util::move(*content); }

	//pointer*
	CCDK_FORCEINLINE T* pointer() noexcept { return content; }
	CCDK_FORCEINLINE const T* pointer() const noexcept { return content; }

	//release
	CCDK_FORCEINLINE T* release() noexcept { T* ret = content; content = nullptr; return ret; }

	//delete
	CCDK_FORCEINLINE ~unique_ptr() { ptr::safe_delete(content); }
};


//for array
// have index
template<typename T>
struct unique_ptr<T[]> : public util::noncopyable
{
	typedef unique_ptr type;
	typedef T*         value_type;
private:
	value_type content;
public:
	CCDK_FORCEINLINE unique_ptr(value_type ptr) noexcept : content(ptr) {}

	CCDK_FORCEINLINE unique_ptr(unique_ptr&& other) noexcept : content{ other.content } { other.content = nullptr; }

	CCDK_FORCEINLINE void swap(unique_ptr& other) noexcept { util::swap(content, other.content); }

	//assign move
	CCDK_FORCEINLINE unique_ptr& operator=(unique_ptr&& other) noexcept
	{
		if (util::addressof(other) != this) { content = other.content; other.content = nullptr; }
	}

	//non-copy
	template<typename P> unique_ptr(const unique_ptr<P>&) = delete;
	template<typename P> unique_ptr& operator=(const unique_ptr<P>&) = delete;

	//index
	CCDK_FORCEINLINE T& operator[](int index)& { return content[index]; }
	CCDK_FORCEINLINE const T& operator[](int index) const & { return content[index]; }
	CCDK_FORCEINLINE T operator[](int index) && { return util::move(content[index]); }

	//pointer*
	CCDK_FORCEINLINE T* pointer() noexcept { return content; }
	CCDK_FORCEINLINE const T* pointer() const noexcept { return content; }

	//release
	CCDK_FORCEINLINE T* release() noexcept { T* ret = content; content = nullptr; return ret; }

	//deletes
	CCDK_FORCEINLINE ~unique_ptr() { ptr::safe_delete_array(content); }
};


//for void
// no dereference
// only access pod type
template<>
struct unique_ptr<void> : public util::noncopyable
{
	typedef unique_ptr	  type;
	typedef void*         value_type;
private:
	value_type content;

	template<typename T, typename = check_t< is_pod<T> > >
	CCDK_FORCEINLINE unique_ptr(T* ptr) noexcept : content(ptr) {}

	template<typename T, typename = check_t< is_pod<T> > >
	CCDK_FORCEINLINE unique_ptr(unique_ptr<T>&& other) noexcept : content{ other.content } { other.content = nullptr; }

	CCDK_FORCEINLINE void swap(unique_ptr& other) noexcept { util::swap(content, other.content); }

	//assign move, provent self move
	CCDK_FORCEINLINE unique_ptr& operator=(unique_ptr&& other) noexcept
	{
		if (util::addressof(other) != this) { content = other.content; other.content = nullptr; }
	}

	//assign move from other unique
	template<typename T, typename = check_t< is_pod<T> > >
	CCDK_FORCEINLINE unique_ptr& operator=(unique_ptr<T>&& other) noexcept
	{
		content = other.content;
		other.content = nullptr;
	}

	//non-copy
	template<typename P> unique_ptr(const unique_ptr<P>&) = delete;
	template<typename P> unique_ptr& operator=(const unique_ptr<P>&) = delete;

	//pointer*
	CCDK_FORCEINLINE void* pointer() noexcept { return content; }
	CCDK_FORCEINLINE const void* pointer() const noexcept { return content; }

	//release
	CCDK_FORCEINLINE void* release() noexcept { void* ret = content; content = nullptr; return ret; }

	//deletes
	CCDK_FORCEINLINE ~unique_ptr() { ptr::imcomp_delete(content); }
};


ccdk_namespace_mpl_sp_end