#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/logic_.h>
#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/type_traits/is_class.h>
#include<ccdk/mpl/type_traits/is_float.h>
#include<ccdk/mpl/util/addressof.h>

ccdk_namespace_mpl_util_start

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


//implements of hash 
namespace util_impl {

	// FNV-1a hash function for bytes in [bytes, bytes + n)
	inline ptr::size_t hash_bytes(const uint8 *bytes, ptr::size_t n) noexcept
	{
		ccdk_assert(bytes != nullptr);
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

	//default hash operation
	template<
		typename T,
		bool = is_float_v<T>,
		bool = has_member_to_hash_v<T>
	>
		struct hash_impl_t {
		CCDK_FORCEINLINE ptr::size_t operator()(T const& t) const noexcept {
			DebugValue("default hash");
			if (util::addressof(t) == nullptr) {
				const std::string* s = reinterpret_cast<const std::string*>(&t);
				DebugFunctionName();
			}
			return hash_bytes(reinterpret_cast<const uint8*>(
				util::addressof(t)), sizeof(t));
		}
	};





	// class with member to_hash exists
	template< typename T>
	struct hash_impl_t<T, false, true> {
		CCDK_FORCEINLINE ptr::size_t operator()(T const& t) const noexcept {
			DebugValue("to hash");
			return t.to_hash();
		}
	};


	// class with member to_hash exists
	template<typename T>
	struct hash_impl_t<T, true, false> {
		CCDK_FORCEINLINE ptr::size_t operator()(T const& t) const noexcept {
			DebugValue("float hash");
			T v = t == T(0) ? T(0) : t;
			return hash_bytes(reinterpret_cast<const uint8*>(
				util::addressof(v)), sizeof(v));
		}
	};

	template<typename T>
	constexpr hash_impl_t<T> hash_impl{};
}

//compute hash of any value
template<typename T>
CCDK_FORCEINLINE ptr::size_t hash(T const& t) noexcept {
	return util_impl::hash_impl<T>(t);
}

//use for expression template
struct hash_t {
	template<typename T>
	CCDK_FORCEINLINE ptr::size_t operator()(T const& t) const noexcept {
		return util_impl::hash_impl<T>(t);
	}
};


ccdk_namespace_mpl_util_end