#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/logic_.h>
#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/type_traits/is_same.h>
#include<ccdk/mpl/type_traits/has_less.h>
#include<ccdk/mpl/util/addressof.h>

ccdk_namespace_mpl_util_start

// test weather member bool T.less_than(P) exists
namespace ut_impl
{
	template<typename T1, typename T2>
	struct has_less_impl
	{
		template<
			typename P1,
			typename P2,
			//check exists
			typename Ret = decltype(makeval<P1>().less(declval<P2>())),
			//check return is bool
			typename = check_t< is_same<Ret, bool>>
		>
			constexpr static bool sfinae(int) { return true; }
		template<typename P1, typename = P2>
		constexpr static bool sfinae(...) { return false; }
		constexpr static bool value = sfinae<T1, T2>(0);
	};
}
template<typename T1, typename T2>
struct has_member_less :bool_< ut_impl::has_less_impl<T1, T2>::value > {};
template<typename T1, typename T2>
static constexpr bool  has_member_less_v = ut_impl::has_less_impl<T1, T2>::value;


namespace util_impl {

	template<
		typename T1,
		typename T2,
		bool = has_less_v<T1, T2, bool>,
		bool = has_member_less_v<T1, T2>,
		bool = has_member_less_v<T2, T1>
	>
		struct less_impl_t {
		// error
	};

	template<typename T1, typename T2>
	constexpr less_impl_t<T1, T2> less_impl{};


	// T1 == T2 declared
	template<typename T1, typename T2>
	struct less_impl_t<T1, T2, true, false, false> {
		CCDK_FORCEINLINE constexpr bool operator()(T1 const& t1, T2 const& t2) const noexcept {
			DebugValue("default less");
			return t1 < t2;
		}
	};

	// T1 == T2 not declared but T1.equals(T2) declare
	template<typename T1, typename T2>
	struct less_impl_t<T1, T2, false, true, false> {
		CCDK_FORCEINLINE constexpr bool operator()(T1 const& t1, T2 const& t2) const noexcept {
			DebugValue("member less exists");
			return t1.less(t2);
		}
	};

	// T1 == T2 not declared but T2.equals(T1) declare
	template<typename T1, typename T2>
	struct less_impl_t<T1, T2, false, false, true> {
		CCDK_FORCEINLINE constexpr bool operator()(T1 const& t1, T2 const& t2) const noexcept {
			DebugValue("reverse member less exists");
			return t2.less(t1);
		}
	};
}

//judge pair <t1,t2> is equals
template<
	typename T1,
	typename T2,
	typename = check_t< or_<
		has_less_v<T1, T2, bool>,
		has_member_less_v<T1, T2>,
		has_member_less_v<T2, T1> >>
	>
	CCDK_FORCEINLINE constexpr bool less(T1 const& t1, T2 const& t2) noexcept {
	return util_impl::less_impl<T1, T2>(t1, t2);
}

template< typename T1, typename T2, uint32 N1, uint32 N2 >
CCDK_FORCEINLINE constexpr bool less(T1 const (&arr1)[N1], T2 const (&arr2)[N2]) noexcept {
	DebugValue("size-diff array equals");
	return false;
}

template< typename T1, typename T2, uint32 N >
CCDK_FORCEINLINE constexpr bool less(T1 const (&arr1)[N], T2 const (&arr2)[N]) noexcept {
	DebugValue("size-same array equals");
	for (uint32 i = 0; i < N; ++i) {
		if (!less(arr1[i], arr2[i])) return false;
	}
	return true;
}

template< typename T1, typename T2>
CCDK_FORCEINLINE constexpr bool less_equal(T1 const& t1, T2 const& t2) noexcept {
	return !less(t2, t1);
}

template< typename T1, typename T2>
CCDK_FORCEINLINE constexpr bool greater(T1 const& t1, T2 const& t2) noexcept {
	return less(t2, t1);
}

template< typename T1, typename T2>
CCDK_FORCEINLINE constexpr bool greater_equal(T1 const& t1, T2 const& t2) noexcept {
	return !less(t1, t2);
}

//for template expression 
struct less_t {
	template<typename T1, typename T2>
	CCDK_FORCEINLINE bool operator()(T1 const& t1, T2 const& t2) const noexcept {
		return less<T1, T2>(t1, t2);
	}
};

// convert t > p to p < t
template<typename T, typename P>
CCDK_FORCEINLINE bool operator>(T const& t, P const& p) {
	//DebugValue("default greater");
	return p<t;
}

// convert t >= p to !(t<p)
template<typename T, typename P>
CCDK_FORCEINLINE bool operator>=(T const& t, P const& p) {
	//DebugValue("default greater");
	return !(t<p);
}

// convert t<=p to !(p<t)
template<typename T, typename P>
CCDK_FORCEINLINE bool operator<=(T const& t, P const& p) {
	//DebugValue("default greater");
	return !(p<t);
}

ccdk_namespace_mpl_util_end