#pragma once

#include<ccdk/memory/memory_module.h>
#include<ccdk/mpl/base/derive_if.h>
#include<ccdk/mpl/base/identity.h>
#include<ccdk/mpl/type_traits/has_inner_type.h>
#include<ccdk/mpl/fusion/local_obj.h>

ccdk_namespace_memory_start

using namespace mpl;

template<typename T, uint32 N>
class local_cache
{
private:
	fs::local_obj<T> content[N];
public:

	//default ctor
	CCDK_FORCEINLINE local_cache() noexcept :content{} {}

	// fill [0, n)
	template<typename... Args>
	local_cache(ptr::size_t n, Args&& ... args) noexcept : content{} {
		util::construct_n<T>(content, n, util::forward<Args>(args)...);
	}



};


ccdk_namespace_memory_end