#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/smart_ptr/share_ptr.h>

ccdk_namespace_mpl_sp_start

template<typename T, typename RefCount = default_ref_count>
class weak_ptr
{
public:
	typedef weak_ptr type;
	typedef RefCount ref_count_type;
	typedef share_ptr<T, normal_deleter<T>, ref_count_type> share_type;
	typedef typename share_type::value_type value_type;
private:
	value_type ptr;
	ref_count_type * ref_count;
public:

	//default constructor
	CCDK_FORCEINLINE weak_ptr() noexcept : ptr{ nullptr }, ref_count{ nullptr } {}

	//nullptr constructor
	CCDK_FORCEINLINE weak_ptr(ptr::nullptr_t) noexcept : ptr{ nullptr }, ref_count{ nullptr } {}

	//from share_ptr<T2,D,R>
	template<  
		typename T2, typename D, typename R,
		typename = check_t< is_convertible< share_ptr<T2,D,R>, type > >
	>
	CCDK_FORCEINLINE weak_ptr(const share_ptr<T2,D,R>& sp) noexcept
		: ptr{ sp.content }, ref_count{ sp.ref_count }
	{
		if (ref_count) ref_count->inc_ref_count();
	}

	//copy from other weak_ptr
	template<
		typename T2, typename R2,
		typename = check_t< is_convertible< weak_ptr<T2,R2>, type > >
	>
	CCDK_FORCEINLINE weak_ptr(const weak_ptr<T2, R2>& other) noexcept
		: ptr{ other.ptr }, ref_count{ other.ref_count }
	{
		if (ref_count) ref_count->inc_ref_count();
	}
	
	//move from other weak_ptr
	template<
		typename T2, typename R2,
		typename = check_t< is_convertible< weak_ptr<T2, R2>, type > >
	>
		CCDK_FORCEINLINE weak_ptr(weak_ptr<T2, R2>&& other) noexcept
		: ptr{ other.ptr }, ref_count{ other.ref_count }
	{
		other.ptr = nullptr;
		other.ref_count = nullptr;
	}

	//swap with other weak_ptr
	template<
		typename T2, typename R2,
		typename = check_t< is_convertible< weak_ptr<T2, R2>, type > >
	>
		CCDK_FORCEINLINE void swap(weak_ptr<T2, R2>& other) noexcept
	{
		util::swap(ptr, other.ptr);
		util::swap(ref_count, other.ref_count);
	}

	//assign from share_ptr
	template<
		typename T2, typename D, typename R,
		typename = check_t< is_convertible< share_ptr<T2, D, R>, type > >
	>
		CCDK_FORCEINLINE weak_ptr& operator=(const share_ptr<T2, D, R>& sp) 
	{
		weak_ptr{}.swap(*this);
		ptr = sp.content;
		ref_count = sp.ref_count;
		if (ref_count) ref_count->inc_ref_count();
		return *this;
	}

	//copy assign
	template<
		typename T2, typename R2,
		typename = check_t< is_convertible< weak_ptr<T2, R2>, type > >
	>
		CCDK_FORCEINLINE weak_ptr& operator=(const weak_ptr<T2, R2 >& other)
	{
		weak_ptr{ other }.swap(*this);
		return *this;
	}

	//move assign
	template<
		typename T2, typename R2,
		typename = check_t< is_convertible< weak_ptr<T2, R2>, type > >
	>
		CCDK_FORCEINLINE weak_ptr& operator=(weak_ptr<T2, R2 >&& other)
	{
		other.swap(*this);
		weak_ptr{}.swap(other);
		return *this;
	}

	// share count
	CCDK_FORCEINLINE uint32 share_count() const noexcept  { if (ref_count) return ref_count->share_count(); return 0; }

	// expired
	CCDK_FORCEINLINE bool expired() const noexcept { return share_count() == 0; }

	// lock 
	CCDK_FORCEINLINE share_type lock() const noexcept { return share_type{ *this }; }
};



ccdk_namespace_mpl_sp_end