#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/smart_ptr/smart_ptr_fwd.h>

#include<ccdk/mpl/base/enable_if.h>

ccdk_namespace_mpl_sp_start

#if defined( CCDK_COMPILER_MSVC ) 

//no suitable implements found, for msvc-17+ to  to get detail line and file error place
template< typename To, typename From, typename = check_t<false_> >
CCDK_FORCEINLINE void static_pointer_cast(From& t) {}

#elif defined( CCDK_COMPILER_GCC )

//no suitable implements found, for gcc  to get detail line and file error place 
template< typename To, typename From>
CCDK_FORCEINLINE void static_pointer_cast(From& f);

#else //clang or some compiler not found suitable method

template< typename To, typename From>
CCDK_FORCEINLINE void static_pointer_cast(From& f)
{
	static_assert(false_::value, "no suitable static_pointer_cast found");
}

#endif

template< typename To, typename From>
CCDK_FORCEINLINE scope_ptr<To> static_pointer_cast(scope_ptr<From>&& t)
{
	return util::move( scope_ptr{ static_cast<To>(t.release()) } );
}

ccdk_namespace_mpl_sp_end