#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/logic_.h>
#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/type_traits/is_same.h>
#include<ccdk/mpl/type_traits/has_equal.h>
#include<ccdk/mpl/util/addressof.h>

ccdk_namespace_mpl_util_start

// test weather member ptr::size_t to_hash() exists
namespace ut_impl
{
	template<typename T1, typename T2>
	struct has_equals_impl
	{
		template<
			typename P1,
			typename P2,
			typename Ret = decltype(makeval<P1>().equals(declval<P2>())), //check to_hash() exists
			typename = check_t< is_same<Ret, bool>>   //check return is bool
		>
			constexpr static bool sfinae(int) { return true; }
		template<typename P1, typename = P2>
		constexpr static bool sfinae(...) { return false; }
		constexpr static bool value = sfinae<T1, T2>(0);
	};
}
template<typename T1, typename T2>
struct has_member_equals :bool_< ut_impl::has_equals_impl<T1,T2>::value > {};
template<typename T1, typename T2>
static constexpr bool  has_member_equals_v = ut_impl::has_equals_impl<T1, T2>::value;


#if defined( CCDK_COMPILER_MSVC ) 

//no suitable implements found, for msvc-17+ to  to get detail line and file error place
template< typename T1, typename T2, typename = check_t<false_> >
CCDK_FORCEINLINE bool equals(T1 const&, typename T2) {}

template< typename T1, typename T2, typename = check_t<false_> >
CCDK_FORCEINLINE bool not_equals(T1 const&, typename T2) {}

#elif defined( CCDK_COMPILER_GCC )

//no suitable implements found, for gcc  to get detail line and file error place 
template< typename T1, typename T2>
CCDK_FORCEINLINE bool equals(T1 const&, typename T2);

template< typename T1, typename T2>
CCDK_FORCEINLINE bool not_equals(T1 const&, typename T2);

#else //clang or some compiler not found suitable method
template< typename T1, typename T2>
CCDK_FORCEINLINE bool equals(T1 const&, typename T2){
	static_assert(false_::value, "no equal function found hash found");
}
template< typename T1, typename T2>
CCDK_FORCEINLINE bool not_equals(T1 const&, typename T2) {
	static_assert(false_::value, "no equal function found hash found");
}
#endif

//default call T1 == T2
template<
	typename T1,
	typename T2,
	typename = check_t< has_equal<T1,T2,bool>>
>
CCDK_FORCEINLINE bool equals(T1 const& t1, T2 const& t2) noexcept {
	return t1 == t2;
}

// if == not define and equals defined
template<
	typename T1,
	typename T2,
	typename = check_t< has_member_equals<T1,T2> >
>
CCDK_FORCEINLINE bool equals(T1 const& t1, T2 const& t2) noexcept {
	return t1.equals(t2);
}

ccdk_namespace_mpl_util_end