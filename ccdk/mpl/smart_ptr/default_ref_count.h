#pragma once
#include<ccdk/mpl/mpl_module.h>

ccdk_namespace_mpl_sp_start

//non-thread safe ref_count
// thread-safe need atomic int
template<typename CountType = uint32 >
struct default_ref_count
{
	//resource memory only shared between share_ptr, last share_ptr release will delete resource memory
	CountType share_count;
	//this will be shared between share_ptr and weak_ptr, unless last share_ptr or weak_ptr is release, this will not release
	CountType ref_count;

	//default and value ref count
	CCDK_FORCEINLINE default_ref_count() noexcept :share_count{}, ref_count{} {}
	CCDK_FORCEINLINE default_ref_count(CountType inShareCount, CountType inRefCount) noexcept : share_count{ inShareCount }, ref_count{ inRefCount } {}

	//share ref count
	template<typename Deleter, typename Class>
	CCDK_FORCEINLINE void dec_share_count(const Deleter& del, Class*& ptr) { if (--share_count == 0) { del(ptr); dec_ref_count(); } }
	CCDK_FORCEINLINE void inc_share_count() { ++share_count; }

	//this ref count
	CCDK_FORCEINLINE void dec_ref_count() { if (--ref_count == 0) { /*destroy RefCount*/ delete this; } }
	CCDK_FORCEINLINE void inc_ref_count() { ++ref_count; }

	CCDK_FORCEINLINE  ~default_ref_count()
	{
		DebugValue("ref count destroy"); 
	}
};

ccdk_namespace_mpl_sp_end