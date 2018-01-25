#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/type_traits/declval.h>
ccdk_namespace_mpl_util_start


#if defined( CCDK_COMPILER_MSVC ) 

//no suitable implements found, for msvc-17+ to  to get detail line and file error place
template< typename T, typename = check_t<false_> >
CCDK_FORCEINLINE decltype(auto) at(const T&, uint32) {}


#elif defined( CCDK_COMPILER_GCC )

//no suitable implements found, for gcc  to get detail line and file error place 
template< typename T>
CCDK_FORCEINLINE decltype(auto) at(const T&, uint32);

#else //clang or some compiler not found suitable method


template< typename T>
CCDK_FORCEINLINE decltype(auto) at(const T&, uint32)
{
	static_assert(false_::value, "no suitable size function found");
}

#endif

template<typename T>
struct has_member_at
{
	template<typename P, typename = decltype(declval<P>().at())>
	bool sfinae(int) { return true; }
	template<typename P>
	bool sfinae(...) { return false; }
	static constexpr bool value = sfinae<T>(0);
};


/* default call size(), need check has function T::size() */
template<typename Container, typename = check_t< has_member_at<Container>  >
	CCDK_FORCEINLINE decltype(auto) at(Container const& ct, uint32 index) noexcept { return ct.at(index); }

	ccdk_namespace_mpl_util_end
