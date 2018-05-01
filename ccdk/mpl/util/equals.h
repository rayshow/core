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


namespace util_impl {

	template<
		typename T1,
		typename T2,
		bool = has_equal_v<T1,T2>,
		bool = has_member_equals_v<T1,T2>
	>
		struct equals_impl_t {};

	// T1 == T2 declared
	template<typename T1, typename T2>
	struct equals_impl_t<T1, T2, true, false> {
		CCDK_FORCEINLINE bool operator()(T1 const& t1, T2 const& t2) const noexcept {
			DebugValue("default equals");
			return t1 == t2;
		}
	};

	// T1 == T2 not declared but T1.equals(T2) declare
	template<typename T1, typename T2>
	struct equals_impl_t<T1, T2, false, true> {
		CCDK_FORCEINLINE bool operator()(T1 const& t1, T2 const& t2) const noexcept {
			DebugValue("member equals");
			return t1.equals(t2);
		}
	};

	template<typename T1, typename T2>
	constexpr equals_impl_t<T1,T2> equals_impl{};
}

//judge pair <t1,t2> is equals
template<typename T1, typename T2>
CCDK_FORCEINLINE bool equals(T1 const& t1, T2 const& t2) noexcept {
	return util_impl::equals_impl<T1, T2>(t1, t2);
}

//for template expression 
struct equals_t {
	template<typename T1, typename T2>
	CCDK_FORCEINLINE bool operator()(T1 const& t1, T2 const& t2) const noexcept {
		return util_impl::equals_impl<T1, T2>(t1, t2);
	}
};


ccdk_namespace_mpl_util_end