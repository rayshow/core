#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/logic_.h>
#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/type_traits/is_const.h>
#include<ccdk/mpl/type_traits/is_enum.h>
#include<ccdk/mpl/type_traits/is_volatile.h>
#include<ccdk/mpl/type_traits/is_float.h>
#include<ccdk/mpl/util/addressof.h>

ccdk_namespace_mpl_util_start

// FNV-1a hash function for bytes in [bytes, bytes + n)
inline ptr::size_t _hash_bytes(const uint8 *bytes, ptr::size_t n) noexcept
{	
#if defined(_WIN64)
	static_assert(sizeof(ptr::size_t) == 8, "This code is for 64-bit size_t.");
	constexpr ptr::size_t kOffsetBasic = 14695981039346656037ULL;
	constexpr ptr::size_t kPrime = 1099511628211ULL;
#else 
	static_assert(sizeof(ptr::size_t) == 4, "This code is for 32-bit size_t.");
	constexpr ptr::size_t kOffsetBasic = 2166136261U;
	constexpr ptr::size_t kPrime = 16777619U;
#endif

	ptr::size_t ret = kOffsetBasic;
	for (ptr::size_t i = 0; i < n; ++i)
	{
		ret ^= (ptr::size_t)bytes[i];
		ret *= kPrime;
	}
	return (ret);
}

// test weather member ptr::size_t to_hash() exists
namespace ut_impl
{
	template<typename T>
	struct has_to_hash_impl
	{
		template<
			typename P,
			typename Ret = decltype(makeval<P>().to_hash()), //check to_hash() exists
			typename = check_t< is_same<Ret, ptr::size_t>>   //check return is ptr::size_t
		>
		constexpr static bool sfinae(int) { return true; }
		template<typename P>
		constexpr static bool sfinae(...) { return false; }
		constexpr static bool value = sfinae<T>(0);
	};
}
template<typename T>
struct has_member_to_hash :bool_< ut_impl::has_to_hash_impl<T>::value > {};
template<typename T>
static constexpr bool  has_member_to_hash_v = ut_impl::has_to_hash_impl<T>::value;



#if defined( CCDK_COMPILER_MSVC ) 

//no suitable implements found, for msvc-17+ to  to get detail line and file error place
template< typename T, typename = check_t<false_> >
CCDK_FORCEINLINE ptr::size_t hash(T const&) noexcept  {}

#elif defined( CCDK_COMPILER_GCC )

//no suitable implements found, for gcc  to get detail line and file error place 
template< typename T>
CCDK_FORCEINLINE ptr::size_t hash(T const&) noexcept;

#else //clang or some compiler not found suitable method

template< typename T, typename = check_t<false_> >
CCDK_FORCEINLINE ptr::size_t hash(T const&) noexcept  {
	static_assert(false_::value, "no suitable member hash found");
}
#endif

// mapping float type -0.0 to 0.0
template<
	typename T,
	typename = check_t< is_float<T> >
>
CCDK_FORCEINLINE ptr::size_t hash(T const& t) noexcept {
	T v = t == T(0) ? T(0) : t;
	return _hash_bytes(util::addressof(v), sizeof(v));
}

//use object's self defined to_hash()
template<
	typename T,
	typename = check_t< has_member_to_hash<T>>
>
CCDK_FORCEINLINE ptr::size_t hash(T const& t) noexcept {
	return t.to_hash();
}

// compute hash of object except float, class with to_hash 
template<
	typename T,
	typename = check_t< not_< has_member_to_hash<T>> >
>
CCDK_FORCEINLINE ptr::size_t hash(T const& t) noexcept {
	return _hash_bytes(util::addressof(t), sizeof(t));
}



ccdk_namespace_mpl_util_end