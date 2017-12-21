#pragma once

#include<exception>

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/type_traits/is_base_of.h>
#include<ccdk/mpl/base/enable_if.h>
#include<ccdk/mpl/util/noncopyable.h>
#include<ccdk/mpl/smart_ptr/normal_deleter.h>

ccdk_namespace_mpl_sp_start

//resource smart ptr

template<typename T, typename Deleter = normal_deleter<T> >
struct scope_ptr :public util::noncopyable
{
	typedef scope_ptr type;
	typedef T*        value_type;
	typedef Deleter   delete_type;
private:
	value_type ptr; 

public:
	//P* is convertible to T
	template<
		typename P,
		typename = check_t< or_< is_convertible<P*, T*>, //P* can convert to T* 
				and_< is_void<T>, not_< is_pod<T> >>>>  // if T is void, P only valid for pod type			 
	>
	CCDK_FORCEINLINE  explicit scope_ptr( P* inPtr) noexcept : ptr(inPtr) {}

	//non-copy
	template<typename P> scope_ptr(const scope_ptr<P>&) = delete;
	template<typename P> scope_ptr& operator=(const scope_ptr<P>&) = delete;

	//refer member, only for class and union, assert not nullptr
	template<typename = check_t< or_<is_class<T>, is_union<T>> > >
	CCDK_FORCEINLINE 
	value_type operator->() noexcept
	{ 
		ccdk_assert(ptr != nullptr);
		return ptr; 
	}

	//refer member, only for class and union, assert not nullptr
	template<typename = check_t< or_<is_class<T>, is_union<T>> > >
	CCDK_FORCEINLINE 
	const value_type operator->() const noexcept
	{
		ccdk_assert(ptr != nullptr);
		return ptr; 
	}

	//index, only for array, assert not nullptr 
	template<typename = check_t< is_array<T> > >
	CCDK_FORCEINLINE 
	T& operator[](uint32 index)& noexcept 
	{ 
		ccdk_assert(ptr != nullptr);
		return ptr[index]; 
	}

	//index, only for array, assert not nullptr 
	template<typename = check_t< is_array<T> > >
	CCDK_FORCEINLINE 
	const T& operator[](uint32 index) const & noexcept 
	{ 
		ccdk_assert(ptr != nullptr);
		return ptr[index];
	}

	//index, only for array, assert not nullptr 
	template<typename = check_t< is_array<T> > >
	CCDK_FORCEINLINE 
	T operator[](uint32 index) && noexcept 
	{ 
		ccdk_assert(ptr != nullptr);
		return ptr[index]; 
	}


	//dereference, not for void, assert not nullptr
	template<typename = check_t< not_< is_void<T>> > >
	CCDK_FORCEINLINE 
	T& operator*() & noexcept
	{ 
		ccdk_assert(ptr != nullptr);  
		return *ptr; 
	}

	//dereference, not for void, assert not nullptr
	template<typename = check_t< not_< is_void<T>> > >
	CCDK_FORCEINLINE const T& operator*() const & noexcept  
	{
		ccdk_assert(ptr != nullptr);  
		return *ptr; 
	}

	//dereference, not for void, assert not nullptr
	template<typename = check_t< not_< is_void<T>> > >
	CCDK_FORCEINLINE 
	T operator*() && noexcept 
	{ 
		ccdk_assert(ptr != nullptr);  
		return util::move(*ptr); 
	}

	//delete
	CCDK_FORCEINLINE ~scope_ptr() { Deleter{}(ptr); }
};


//for array
template<typename T> 
struct scope_ptr<T[]> : public util::noncopyable
{
	typedef scope_ptr<T[]> type;
	typedef T* value_type;
private:
	value_type ptr;
public:

	//P* is convertible to T*
	template<
		typename P,
		typename = check_t< is_convertible<P*, T*>>
	>
	CCDK_FORCEINLINE explicit scope_ptr(P* inPtr) noexcept : ptr(inPtr) {}

	//non-copy
	template<typename P> scope_ptr(const scope_ptr<P>&) = delete;
	template<typename P> scope_ptr& operator=(const scope_ptr<P>&) = delete;

		

	CCDK_FORCEINLINE value_type value() { return ptr; }

	//delete
	CCDK_FORCEINLINE ~scope_ptr()  { ptr::safe_delete_array(ptr); }
};

//void * is special, can't dereference / ref member / index
//because void* is incomplete, can't use safe_delete
template<>
struct scope_ptr<void>: public util::noncopyable
{
	typedef void* value_type;
private:
	value_type ptr;
public:
	//keep reference

	//T need be pod when destructed is safe
	template<
		typename T,
		typename = check_t< is_pod<T> >
	>
	CCDK_FORCEINLINE scope_ptr(T* inPtr) noexcept : ptr(inPtr) {}

	//non-copy
	template<typename P> scope_ptr(const scope_ptr<P>&) = delete;
	template<typename P> scope_ptr& operator=(const scope_ptr<P>&) = delete;

	CCDK_FORCEINLINE  value_type value() { return ptr; }

	CCDK_FORCEINLINE ~scope_ptr() { delete ptr; ptr = nullptr; }
};

ccdk_namespace_mpl_sp_end