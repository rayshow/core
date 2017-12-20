#pragma once

#include<exception>

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/type_traits/is_base_of.h>
#include<ccdk/mpl/base/enable_if.h>
#include<ccdk/mpl/util/noncopyable.h>
#include<ccdk/mpl/smart_ptr/deref_nullptr_exception.h>

ccdk_namespace_mpl_sp_start

	template<typename T>
	struct scope_ptr 
		:public util::noncopyable
	{
		typedef scope_ptr type;
		typedef T* value_type;
	private:
		value_type ptr; 

	public:
		//P* is convertible to T
		template<
			typename P,
			typename = check_t< is_convertible<P*, T*>>
		>
		CCDK_FORCEINLINE  explicit scope_ptr( P* inPtr) noexcept : ptr(inPtr) {}

		//non-copy
		template<typename P> scope_ptr(const scope_ptr<P>&) = delete;
		template<typename P> scope_ptr& operator=(const scope_ptr<P>&) = delete;

		//refer member
		CCDK_FORCEINLINE value_type operator->() noexcept { return ptr; }
		CCDK_FORCEINLINE const value_type operator->() const noexcept { return ptr; }

		//dereference: may raise deref_nullptr_exception
		CCDK_FORCEINLINE T& operator*() & { return const_cast<T&>(__deref_impl()); }
		CCDK_FORCEINLINE const T& operator*() const & { return __deref_impl(); }
		CCDK_FORCEINLINE T&& operator*() &&  { return const_cast<T&&>(__deref_impl()); }

		//delete
		CCDK_FORCEINLINE ~scope_ptr()  { ptr::safe_delete(ptr); }

	private:
		CCDK_FORCEINLINE  
		const T& __deref_impl() const&
		{
			if (ptr) return ptr;
			throw deref_nullptr_exception{ " deref nullptr at ccdk::mpl::scope_ptr::operator*()" };
		}
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

		//index: may cause index out of range exception
		CCDK_FORCEINLINE T& operator[](uint32 index)& noexcept { return ptr[index]; }
		CCDK_FORCEINLINE const T& operator[](uint32 index) const & noexcept  { return ptr[index]; }
		CCDK_FORCEINLINE T&& operator[](uint32 index) && noexcept { return ptr[index]; }

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