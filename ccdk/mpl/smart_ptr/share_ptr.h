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
#include<ccdk/mpl/type_traits/identity.h>
#include<ccdk/mpl/type_traits/is_convertible.h>
#include<ccdk/mpl/util/noncopyable.h>
#include<ccdk/mpl/util/swap.h>
#include<ccdk/mpl/util/value.h>
#include<ccdk/mpl/util/hash.h>
#include<ccdk/mpl/util/compatible_cmp.h>
#include<ccdk/mpl/smart_ptr/smart_ptr_fwd.h>
#include<ccdk/mpl/smart_ptr/default_deleter.h>
#include<ccdk/mpl/smart_ptr/default_ref_count.h>


ccdk_namespace_mpl_sp_start

//static small and simple share_ptr implements, no virtual, Deleter / RefCount / Allocator ensure at compile time

//for non-array, non-void, non-thread-safe implements
// have member operation
// have dereference operation
// no   index operation
// support convertible type share 
template<
	typename T,
	typename Deleter =  default_deleter<T>,          //normal deleter for different type
	typename RefCount = default_ref_count<uint32>  // multi-thread need atomic ref_count, need nothrow constructor
>
class share_ptr
{
public:
	typedef T*		                   value_type;
	typedef Deleter                    delete_type;
	typedef RefCount                   ref_count_type;
	typedef share_ptr				   type;
	typedef weak_ptr<T,RefCount>       weak_type;
	template<typename, typename, typename> friend class share_ptr;
	template<typename, typename> friend class weak_ptr;
private:
	ref_count_type* ref_count;
	value_type      content;
public:

	//default constructor
	CCDK_FORCEINLINE share_ptr() noexcept :ref_count{ nullptr }, content{ nullptr }{}
	//nullptr constructor
	CCDK_FORCEINLINE share_ptr(ptr::nullptr_t) noexcept : ref_count{ nullptr },  content{ nullptr } {}
	//construct from T pointer
	CCDK_FORCEINLINE explicit share_ptr(value_type ptr) noexcept : ref_count{ new RefCount{1,1} }, content{ ptr } {}
	//construct from weak_ptr
	CCDK_FORCEINLINE explicit share_ptr(const weak_type& wp) noexcept : ref_count{ wp.ref_count }, content{ wp.ptr } { if (ref_count) ref_count->inc_share_count(); }

	//copy constructor
	template<
		typename T2, typename D2, typename R2,
		typename = check_t< is_convertible< share_ptr<T2,D2,R2>, type >>
	>
	CCDK_FORCEINLINE
	share_ptr(const share_ptr<T2,D2,R2> & other) noexcept
		: ref_count{ other.ref_count },
			content{ other.content }
	{
		if(ref_count) ref_count->inc_share_count();
	}

	//move constructor
	template<
		typename T2, typename D2, typename R2,
		typename = check_t< is_convertible< share_ptr<T2, D2, R2>, type >>
	>
	CCDK_FORCEINLINE
	share_ptr(share_ptr<T2, D2, R2>&& other) noexcept
	{
		other.swap(*this);          
		share_ptr{}.swap(other);    
	}

	//assign
	CCDK_FORCEINLINE share_ptr& operator=(value_type ptr) { share_ptr{ ptr }.swap(*this); return *this; }


	//copy assign
	template<
		typename T2, typename D2, typename R2,
		typename = check_t< is_convertible< share_ptr<T2, D2, R2>, type >>
	>
	CCDK_FORCEINLINE
	share_ptr& operator=(const share_ptr<T2, D2, R2>& other) noexcept
	{
		share_ptr{ other }.swap(*this);
		return *this;
	}

	//move assign
	template<
		typename T2, typename D2, typename R2,
		typename = check_t< is_convertible< share_ptr<T2, D2, R2>, type >>
	>
	CCDK_FORCEINLINE
	share_ptr& operator=(share_ptr<T2, D2, R2>&& other) noexcept
	{
		other.swap(*this);
		share_ptr{}.swap(other);
		return *this;
	}

	//swap operation
	template<
		typename T2, typename D2, typename R2,
		typename = check_t< is_convertible< share_ptr<T2, D2, R2>, type >>
	>
	CCDK_FORCEINLINE
	void swap(share_ptr<T2, D2, R2>& other) noexcept
	{
		util::swap(ref_count, other.ref_count);
		util::swap(content, other.content);
	}

	//refer-member, valid for class and union
	CCDK_FORCEINLINE 
	value_type operator->() noexcept 
	{ 
		static_assert(or_v<is_class<T>, is_union<T>>, "only class and union have member operator");
		return content; 
	}

	//refer-member, valid for class and union
	CCDK_FORCEINLINE 
	const value_type operator->() const noexcept 
	{ 
		static_assert(or_v<is_class<T>, is_union<T>>, "only class and union have member operator");
		return content; 
	}


	//deref, type can't be void, assert not nullptr 
	CCDK_FORCEINLINE
	T& operator*() noexcept
	{
		static_assert(not_< is_void<T>>::value, "void* can't dereference ");
		ccdk_assert(content != nullptr);
		return *content;
	}

	//deref, type can't be void, assert not nullptr
	CCDK_FORCEINLINE
	const T& operator*() const noexcept
	{
		static_assert(not_< is_void<T>>::value, "void* can't dereference ");
		ccdk_assert(content != nullptr);
		return *content;
	}

	//get pointer
	CCDK_FORCEINLINE value_type pointer() noexcept { return content; }
	CCDK_FORCEINLINE const value_type pointer() const noexcept { return content; }
	CCDK_FORCEINLINE const RefCount* ref_count_addr() noexcept { return ref_count; }

	//share count
	CCDK_FORCEINLINE uint32 share_count() noexcept { return ref_count->share_count; }

	//destructor
	CCDK_FORCEINLINE ~share_ptr() { if(ref_count) ref_count->dec_share_count(Deleter{}, content); /*not store Deleter*/ }
};


//for array implements
// no member operation
// no dereference operation
// has index operation
// only support same style share_ptr copy / move
template<
	typename T,
	typename Deleter,  
	typename RefCount 
>
class share_ptr<T[], Deleter, RefCount>
{
public:
	typedef share_ptr                  type;
	typedef T*                         value_type;
	typedef Deleter                    delete_type;
	typedef RefCount                   ref_count_type;
	typedef weak_ptr<T,RefCount>       weak_type;
	template<typename,typename > friend class  weak_ptr;
private:
	ref_count_type* ref_count;
	value_type      content;
public:

	//default constructor
	CCDK_FORCEINLINE share_ptr() noexcept : ref_count{ nullptr }, content{ nullptr } {}

	//nullptr constructor
	CCDK_FORCEINLINE share_ptr(ptr::nullptr_t) noexcept : ref_count{ nullptr }, content{ nullptr } {}

	//construct from weak_ptr
	CCDK_FORCEINLINE explicit share_ptr(const weak_type& wp) noexcept : ref_count{ wp.ref_count }, content{ wp.ptr } { if (ref_count) ref_count->inc_share_count(); }

	//construct from T pointer, use T[] pointer to differenet P[] is not a good idea(even T is base of P), see more effective c++ item 3
	template<typename P, typename = check_t< is_same<T,P>>  >
	CCDK_FORCEINLINE explicit share_ptr(P* ptr) noexcept : ref_count{ new RefCount{ 1,1 } }, content{ ptr } {}

	//only support copy from same style share_ptr, reason see explicit pointer constructor
	CCDK_FORCEINLINE share_ptr(const share_ptr& other) noexcept : ref_count{ other.ref_count }, content{ other.content } { if(ref_count) ref_count->inc_share_count(); }

	//only support move from same style share_ptr, reason see explicit pointer constructor
	CCDK_FORCEINLINE share_ptr(share_ptr&& other) noexcept : ref_count{ other.ref_count }, content{ other.content } { other.content = nullptr; other.ref_count = nullptr; }

	//swap same array share_ptr
	CCDK_FORCEINLINE void swap(share_ptr& other) noexcept { util::swap(content, other.content);  util::swap(ref_count, other.ref_count); }

	//copy assign
	CCDK_FORCEINLINE share_ptr& operator=(const share_ptr& other) noexcept { share_ptr{ other }.swap(*this); }

	//move assign 
	CCDK_FORCEINLINE share_ptr& operator=(share_ptr&& other) noexcept { share_ptr{ other }.swap(*this); share_ptr{}.swap(*this); }


	//index, valid only for array
	CCDK_FORCEINLINE T& operator[](uint32 index) & noexcept { return content[index]; }
	CCDK_FORCEINLINE const T& operator[](uint32 index) const& noexcept { return content[index]; }
	CCDK_FORCEINLINE T operator[](uint32 index) && noexcept { return util::move(content[index]); }

	//get pointer
	CCDK_FORCEINLINE value_type pointer() noexcept { return content; }
	CCDK_FORCEINLINE const value_type pointer() const noexcept { return content; }
	CCDK_FORCEINLINE const RefCount* ref_count_addr() noexcept { return ref_count; }

	//share count
	CCDK_FORCEINLINE uint32 share_count() noexcept { return ref_count->share_count; }

	CCDK_FORCEINLINE ~share_ptr() { if(ref_count) ref_count->dec_share_count(Deleter{}, content);  /*not store Deleter */ }
};



//for void implements, void only support pointer to pod type
// no member operation
// no dereference operation
// no index operation
// only support pod share_ptr copy / move
template<
	typename Deleter,
	typename RefCount
>
class share_ptr<void, Deleter, RefCount>
{
public:
	typedef void*	                   value_type;
	typedef Deleter                    delete_type;
	typedef RefCount                   ref_count_type;
	typedef weak_ptr<void,RefCount>       weak_type;
	template<typename, typename> friend class  weak_ptr;
	template<typename, typename, typename> friend class share_ptr;
private:
	ref_count_type* ref_count;
	value_type      content;
public:

	//default constructor
	CCDK_FORCEINLINE share_ptr() noexcept : ref_count{ nullptr }, content{ nullptr } {}

	//nullptr constructor
	CCDK_FORCEINLINE share_ptr(ptr::nullptr_t) noexcept : ref_count{ nullptr }, content{ nullptr } {}

	//construct from weak_ptr
	CCDK_FORCEINLINE explicit share_ptr(const weak_type& wp) noexcept : ref_count{ wp.ref_count }, content{ wp.ptr } { if (ref_count) ref_count->inc_share_count(); }

	//construct from P pointer, P need be pod
	template<typename P, typename = check_t< is_pod<P>> >
	CCDK_FORCEINLINE explicit share_ptr(P* ptr) noexcept : ref_count{ new RefCount{ 1,1 } }, content{ ptr } {}

	//only support copy from pod type share_ptr
	template<typename P, typename = check_t< is_pod<P>> >
	CCDK_FORCEINLINE share_ptr(const share_ptr<P>& other) noexcept : ref_count{ other.ref_count }, content{ other.content } { if(ref_count) ref_count->inc_share_count(); }

	//only support move from pod type share_ptr
	template<typename P, typename = check_t< is_pod<P>> >
	CCDK_FORCEINLINE share_ptr(share_ptr<P>&& other) noexcept : ref_count{ other.ref_count }, content{ other.content } { other.content = nullptr; other.ref_count = nullptr; }

	//only support swap from pod type share_ptr
	template<typename P, typename = check_t< is_pod<P>> >
	CCDK_FORCEINLINE void swap(share_ptr<P>& other) noexcept { util::swap(content, other.content);  util::swap(ref_count, other.ref_count); }

	//copy assign
	template<typename P, typename = check_t< is_pod<P>> >
	CCDK_FORCEINLINE share_ptr& operator=(const share_ptr<P>& other) noexcept { share_ptr{ other }.swap(*this); }

	//move assign 
	template<typename P, typename = check_t< is_pod<P>> >
	CCDK_FORCEINLINE share_ptr& operator=(share_ptr<P>&& other) noexcept { share_ptr{ other }.swap(*this); share_ptr{}.swap(*this); }

	//get pointer
	CCDK_FORCEINLINE value_type pointer() noexcept { return content; }
	CCDK_FORCEINLINE const value_type pointer() const noexcept { return content; }

	CCDK_FORCEINLINE const RefCount* ref_count_addr() noexcept { return ref_count; }

	//share count
	CCDK_FORCEINLINE uint32 share_count() noexcept { if (ref_count) return ref_count->share_count; return 0; }

	CCDK_FORCEINLINE ~share_ptr() { if(ref_count) ref_count->dec_share_count(Deleter{}, content);  /*not store Deleter */ }
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// share_ptr == / != / < / <= / > / >=

// share_ptr == share_ptr
template<typename T1, typename D1,  typename R1,
	typename T2, typename D2, typename R2>
CCDK_FORCEINLINE bool operator==(const share_ptr<T1, D1, R1>& lh, const share_ptr<T2, D2, R2>& rh)
{
	return lh.pointer() == rh.pointer();
}

// share_ptr != share_ptr
template<typename T1, typename D1, typename R1,
	typename T2, typename D2, typename R2>
	CCDK_FORCEINLINE bool operator!=(const share_ptr<T1, D1, R1>& lh, const share_ptr<T2, D2, R2>& rh)
{
	return !(lh == rh);
}


// share_ptr < share_ptr
template<typename T1, typename D1, typename R1,
	typename T2, typename D2, typename R2>
	CCDK_FORCEINLINE bool operator<(const share_ptr<T1, D1, R1>& lh, const share_ptr<T2, D2, R2>& rh)
{
	return util::compatible_less<
		typename share_ptr<T1, D1, R1>::value_type,
		typename share_ptr<T2, D2, R2>::value_type >
	{}(lh.pointer(), rh.pointer());
}

// share_ptr > share_ptr
template<typename T1, typename D1, typename R1,
typename T2, typename D2, typename R2>
CCDK_FORCEINLINE bool operator>(const share_ptr<T1, D1, R1>& lh, const share_ptr<T2, D2, R2>& rh)
{
	return rh < lh;
}

// share_ptr >= share_ptr
template<typename T1, typename D1, typename R1,
	typename T2, typename D2, typename R2>
	CCDK_FORCEINLINE bool operator>=(const share_ptr<T1, D1, R1>& lh, const share_ptr<T2, D2, R2>& rh)
{
	return !(lh < rh);
}

// share_ptr <= share_ptr
template<typename T1, typename D1, typename R1,
	typename T2, typename D2, typename R2>
	CCDK_FORCEINLINE bool operator<=(const share_ptr<T1, D1, R1>& lh, const share_ptr<T2, D2, R2>& rh)
{
	return !(rh < lh);
}

// share_ptr == nullptr
template<typename T1, typename D1, typename R1>
	CCDK_FORCEINLINE bool operator==(const share_ptr<T1, D1, R1>& lh, ptr::nullptr_t )
{
	return lh.pointer() == nullptr;
}

// nullptr == share_ptr
template<typename T1, typename D1, typename R1>
CCDK_FORCEINLINE bool operator==(ptr::nullptr_t, const share_ptr<T1, D1, R1>& rh )
{
	return rh.pointer() == nullptr;
}

// share_ptr != nullptr
template<typename T1, typename D1, typename R1>
CCDK_FORCEINLINE bool operator!=(const share_ptr<T1, D1, R1>& lh, ptr::nullptr_t)
{
	return lh.pointer() != nullptr;
}

// nullptr != share_ptr
template<typename T1, typename D1, typename R1>
CCDK_FORCEINLINE bool operator!=(ptr::nullptr_t, const share_ptr<T1, D1, R1>& rh)
{
	return rh.pointer() != nullptr;
}

// share_ptr < nullptr_t
template<typename T1, typename D1, typename R1>
CCDK_FORCEINLINE bool operator<(ptr::nullptr_t, const share_ptr<T1, D1, R1>& rh)
{
	return util::compatible_less<
		ptr::nullptr_t,
		typename share_ptr<T1, D1, R1>::value_type>
	{}(nullptr, rh.pointer());
}

// share_ptr < nullptr_t
template<typename T1, typename D1, typename R1>
CCDK_FORCEINLINE bool operator<(const share_ptr<T1, D1, R1>& lh, ptr::nullptr_t)
{
	return util::compatible_less<
		ptr::nullptr_t,
		typename share_ptr<T1, D1, R1>::value_type>
	{}( lh.pointer(), nullptr );
}


// nullptr_t > share_ptr
template<typename T1, typename D1, typename R1>
CCDK_FORCEINLINE bool operator>(ptr::nullptr_t, const share_ptr<T1, D1, R1>& rh)
{
	return rh.pointer() < nullptr;
}

// share_ptr > nullptr_t
template<typename T1, typename D1, typename R1>
CCDK_FORCEINLINE bool operator>(const share_ptr<T1, D1, R1>& rh, ptr::nullptr_t)
{
	return nullptr < rh.pointer();
}

// nullptr_t >= share_ptr
template<typename T1, typename D1, typename R1>
CCDK_FORCEINLINE bool operator>=(ptr::nullptr_t, const share_ptr<T1, D1, R1>& rh)
{
	return !(nullptr < rh.pointer());
}

// share_ptr >= nullptr_t
template<typename T1, typename D1, typename R1>
CCDK_FORCEINLINE bool operator>=(const share_ptr<T1, D1, R1>& lh , ptr::nullptr_t)
{
	return !(lh.pointer() < nullptr);
}


// nullptr_t <= share_ptr
template<typename T1, typename D1, typename R1>
CCDK_FORCEINLINE bool operator<=(ptr::nullptr_t, const share_ptr<T1, D1, R1>& rh)
{
	return !(rh.pointer() < nullptr);
}

// share_ptr <= nullptr_t
template<typename T1, typename D1, typename R1>
CCDK_FORCEINLINE bool operator<=(const share_ptr<T1, D1, R1>& lh, ptr::nullptr_t)
{
	return !(nullptr < lh.pointer());
}

//// end 
//////////////////////////////////////////////////////////////////////////////////////////////////////////

ccdk_namespace_mpl_sp_end


//partial specialization
ccdk_namespace_mpl_util_start

//general implements
template<typename T, typename D, typename R>
struct get_value_t< ccdk::mpl::sp::share_ptr<T,D,R> >
{
	decltype(auto) operator()(const ccdk::mpl::sp::share_ptr<T,D,R>& t) const
	{
		return t.pointer();
	}
};


//partial implments of hash
template<typename T, typename D, typename R>
struct hash_t< ccdk::mpl::sp::share_ptr<T, D, R> >
{
	ptr::size_t operator()(const ccdk::mpl::sp::share_ptr<T, D, R>& t) const
	{
		return hash< typename ccdk::mpl::sp::share_ptr<T, D, R>::value_type >(t.pointer());
	}
};


ccdk_namespace_mpl_util_end