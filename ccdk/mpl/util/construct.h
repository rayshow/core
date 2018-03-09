#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/base/logic_.h>
#include<ccdk/mpl/type_traits/is_pointer.h>
#include<ccdk/mpl/type_traits/has_deref.h>
#include<ccdk/mpl/util/forward.h>

ccdk_namespace_mpl_util_start

/* is proxy class(like iterator), try dereference It and get real address */
template<
	typename T,
	typename It,
	typename = check_t< not_< is_pointer<It>>>,
	typename = check_t< has_deref<It>>,
	typename... Args>
	CCDK_FORCEINLINE static void construct(It it, Args&& ... args)
{
	new( util::addressof( *it ) ) T(util::forward<Args>(args)...);
}

/* truely pointer, directly construct on it */
template<typename T, typename... Args>
CCDK_FORCEINLINE static void construct(void* memory, Args&& ... args)
{
	new(memory) T(util::forward<Args>(args)...);
}



ccdk_namespace_mpl_util_end