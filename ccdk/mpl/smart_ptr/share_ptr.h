#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/enable_if.h>
#include<ccdk/mpl/base/not_.h>
#include<ccdk/mpl/base/and_.h>
#include<ccdk/mpl/base/or_.h>
#include<ccdk/mpl/type_traits/is_void.h>
#include<ccdk/mpl/type_traits/is_class.h>
#include<ccdk/mpl/type_traits/is_union.h>
#include<ccdk/mpl/type_traits/is_array.h>
#include<ccdk/mpl/type_traits/is_pod.h>
#include<ccdk/mpl/type_traits/is_same.h>
#include<ccdk/mpl/type_traits/is_convertible.h>
#include<ccdk/mpl/util/noncopyable.h>
#include<ccdk/mpl/util/swap.h>
#include<ccdk/mpl/smart_ptr/normal_deleter.h>

ccdk_namespace_mpl_sp_start


//non-thread safe ref_count, max 65535 share count
template<typename RefType>
class default_ref_count
{
private:
	//resource memory only shared between share_ptr, last share_ptr release will delete resource memory
	RefType shared_count;
	//this will be shared between share_ptr and weak_ptr, unless last share_ptr or weak_ptr is release, this will not release
	RefType ref_count;
public:

	CCDK_FORCEINLINE
	default_ref_count() noexcept
		:shared_count{ 0 }, ref_count{ 0 }
	{}

	CCDK_FORCEINLINE
	default_ref_count(RefType inShareCount, RefType inRefCount) noexcept
		:shared_count{inShareCount},
			ref_count{inRefCount}
	{}

	template<typename Deleter, typename Class>
	CCDK_FORCEINLINE
	void dec_share_count(const Deleter& del, Class*& ptr)
	{
		if (--shared_count == 0)
		{
			del(ptr);
		}
	}

	CCDK_FORCEINLINE
	void inc_share_count()
	{
		++shared_count;
	}

	CCDK_FORCEINLINE
	void dec_ref_count()
	{
		if (--ref_count == 0)
		{
			//destroy RefCount
			delete this; 
		}
	}

	CCDK_FORCEINLINE
	void inc_ref_count()
	{
		++ref_count;
	}
};

template<
	typename Class,
	typename Deleter, 
	typename RefCount
>
class share_ptr;

// trait T is share_ptr type
template<typename T> struct is_share_ptr :false_ {};
template<typename Class,typename Deleter, typename RefCount> 
struct is_share_ptr< share_ptr<Class,Deleter,RefCount>> :true_ {};

// share_ptr S1 convertible to S2 when
//   0. both is share_ptr
//   1. S1::value_type convertible to S2::value_type
//   2. same delete_type
//   3. same ref_count_type
template<typename S1, typename S2>
struct is_share_ptr_convertible:
	and_< is_share_ptr<S1>, is_share_ptr<S2>,
		  is_convertible< typename S1::value_type, typename S2::value_type>,
		  is_same< typename S1::delete_type, typename S2::delete_type>,
		  is_same< typename S1::ref_count_type, typename S2::ref_count_type>
	>{};


template<
	typename T,
	typename Deleter = normal_deleter<T>,      //normal deleter for different type
	typename RefCount = default_ref_count<uint16>  // multi-thread need atomic ref_count, need nothrow constructor
>
class share_ptr
{
	typedef T*		  value_type;
	typedef Deleter   delete_type;
	typedef RefCount  ref_count_type;
	typedef share_ptr this_type;
private:
	ref_count_type* ref_count;
	value_type      content;
public:

	//default constructor
	CCDK_FORCEINLINE
		share_ptr() noexcept
		:ref_count{ nullptr }, content{ nullptr }
	{}

	//nullptr constructor
	CCDK_FORCEINLINE
		share_ptr(ptr::nullptr_t) noexcept
		: ref_count{ nullptr }, 
			content{ nullptr }
	{}

	//ptr constructor
	//RefCount need a noexcept constructor
	template<
		typename P,
		typename = check_t< 
			or_< is_convertible<P*, T*>,            //P* can convert to T* 
			and_< is_void<T>, not_< is_pod<T> >>>>  // if T is void, P only valid for pod type			 
	>
	CCDK_FORCEINLINE explicit
		share_ptr(P* ptr) noexcept
		: ref_count{ new RefCount{1,1} },
			content{ ptr }
	{}

	//copy constructor
	//Class1 must be base of Class2
	template<
		typename T2,
		typename = check_t< is_share_ptr_convertible< share_ptr<T2>, this_type >>
	>
	CCDK_FORCEINLINE
	share_ptr(const share_ptr<T2> & other) noexcept
		: ref_count{ other.ref_count },
			content{ other.content }
	{
		ref_count->inc_share_count();
	}

	//move constructor
	template<
		typename T2,
		typename = check_t< is_share_ptr_convertible< share_ptr<T2>, this_type >>
	>
	CCDK_FORCEINLINE
	share_ptr(share_ptr<T2>&& other) noexcept
	{
		other.swap(*this);          //move to this
		share_ptr{}.swap(other);    //release other
	}

	//assign
	template<
		typename P,
		typename = check_t< is_convertible<P*, T*>>,                 //P* can convert to T* 
		typename = check_t< and_< is_void<T>, not_< is_pod<T> > >>   //if T is void, P only valid for pod type
	>
	CCDK_FORCEINLINE
	share_ptr& operator=(P* ptr)
	{
		share_ptr{ ptr }.swap(*this);
		return *this;
	}

	//copy assign
	template<
		typename T2,
		typename = check_t< is_share_ptr_convertible< share_ptr<T2>, this_type >>
	>
	CCDK_FORCEINLINE
	share_ptr& operator=(const share_ptr<T2>& other)
	{
		share_ptr{ other }.swap(*this);
		return *this;
	}

	//copy assign
	template<
		typename T2,
		typename = check_t< is_share_ptr_convertible< share_ptr<T2>, this_type >>
	>
	CCDK_FORCEINLINE
	share_ptr& operator=(share_ptr<T2>&& other)
	{
		other.swap(*this);
		share_ptr{}.swap(other);
		return *this;
	}

	//swap operation
	template<
		typename T2,
		typename = check_t< is_share_ptr_convertible< share_ptr<T2>, this_type >>
	>
	CCDK_FORCEINLINE
	void swap(share_ptr<T2>& other) noexcept
	{
		util::swap(ref_count, other.ref_count);
		util::swap(content, other.content);
	}

	//refer-member, valid for class and union
	template<typename = check_t< or_<is_class<T>, is_union<T>> > >
	CCDK_FORCEINLINE 
	value_type operator->() noexcept 
	{ 
		return content; 
	}

	//refer-member, valid for class and union
	template<typename = check_t< or_<is_class<T>, is_union<T>> > >
	CCDK_FORCEINLINE 
	const value_type operator->() const noexcept 
	{ 
		return content; 
	}

	//index, valid only for array
	template<typename = check_t< is_array<T> > >
	CCDK_FORCEINLINE
	T& operator[](uint32 index) & noexcept
	{
		return content[index];
	}

	//index, valid only for array
	template<typename = check_t< is_array<T> > >
	CCDK_FORCEINLINE
	const T& operator[](uint32 index) const& noexcept 
	{
		return content[index];
	}

	//index, valid only for array
	template<typename = check_t< is_array<T> > >
	CCDK_FORCEINLINE
	T operator[](uint32 index) && noexcept
	{
		return util::move( content[index] );
	}

	//deref, type can't be void, assert not nullptr 
	template<typename = check_t< not_< is_void<T>>>>
	CCDK_FORCEINLINE
	T& operator*() noexcept
	{
		ccdk_assert(content != nullptr);
		return *content;
	}

	//deref, type can't be void, assert not nullptr 
	template<typename = check_t< not_< is_void<T>>>>
	CCDK_FORCEINLINE
	const T& operator*() const noexcept
	{
		ccdk_assert(content != nullptr);
		return *content;
	}

	//get pointer
	CCDK_FORCEINLINE value_type value() { return content; }
	CCDK_FORCEINLINE const value_type value() const { return content; }


	CCDK_FORCEINLINE
		~share_ptr()
	{
		//not store Deleter
		ref_count->dec_share_count(Deleter{}, content);
	}
};

ccdk_namespace_mpl_sp_end