#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/type_traits/is_convertible.h>
#include<ccdk/mpl/base/enable_if.h>
#include<ccdk/mpl/util/noncopyable.h>
#include<ccdk/mpl/util/swap.h>
#include<ccdk/mpl/smart_ptr/normal_deleter.h>

ccdk_namespace_mpl_sp_start

//non-thread safe ref_count
class default_ref_count
{
private:
	//resource memory only shared between share_ptr, last share_ptr release will delete resource memory
	uint32 shared_count;
	//this will be shared between share_ptr and weak_ptr, unless last share_ptr or weak_ptr is release, this will not release
	uint32 ref_count;
public:

	CCDK_FORCEINLINE
	default_ref_count() noexcept
		:shared_count{ 0 }, ref_count{ 0 }
	{}

	CCDK_FORCEINLINE
	default_ref_count(uint32 inShareCount, uint32 inRefCount) noexcept
		:shared_count{inShareCount},
			ref_count{inRefCount}
	{}

	template<typename Deleter, typename Class>
	CCDK_FORCEINLINE
	void decrease_share_count(const Deleter& del, Class*& ptr)
	{
		if (--shared_count == 0)
		{
			del(ptr);
		}
	}

	CCDK_FORCEINLINE
	void increase_share_count()
	{
		++shared_count;
	}

	CCDK_FORCEINLINE
	void decrease_ref_count()
	{
		if (--ref_count == 0)
		{
			//destroy RefCount
			delete this; 
		}
	}

	CCDK_FORCEINLINE
		void increase_ref_count()
	{
		++ref_count;
	}
};


template<
	typename Class,
	typename Deleter = normal_deleter,     //array need normal_array_deleter
	typename RefCount = default_ref_count  //multi-thread need atomic ref_count, need nothrow constructor
>
class share_ptr
{
private:
	RefCount* ref_count;
	Class*    content;
public:
	CCDK_FORCEINLINE
		share_ptr() noexcept
		:ref_count{ nullptr }, content{ nullptr }
	{}

	CCDK_FORCEINLINE
		share_ptr(ptr::nullptr_t) noexcept
		: ref_count{ nullptr }, content{ nullptr }
	{}

	//derived class can also transfered use ptr
	//RefCount need a noexcept constructor
	CCDK_FORCEINLINE explicit
		share_ptr( Class* ptr) noexcept
		: ref_count{ new RefCount{1,1} }, content{ ptr }
	{}

	//Class1 must be base of Class2
	template<
		typename Class2,
		typename = check_t< is_convertible<Class*,Class2*> >
	>
	CCDK_FORCEINLINE
		share_ptr(const share_ptr<Class2> & other) noexcept
		: ref_count{ other.ref_count },
		content{ other.content }
	{}

	template<typename Class2>
	CCDK_FORCEINLINE
	void swap(share_ptr<Class2>& other) noexcept
	{
		util::swap(ref_count, other.ref_count);
		util::swap(content, other.content);
	}

	CCDK_FORCEINLINE
		~share_ptr()
	{
		ref_count->decrease_share_count(Deleter{}, content);
	}
};

ccdk_namespace_mpl_sp_end