#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/logic_.h>
#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/type_traits/is_same.h>
#include<ccdk/mpl/type_traits/has_less.h>
#include<ccdk/mpl/type_traits/is_byte.h>
#include<ccdk/mpl/util/addressof.h>
#include<ccdk/mpl/type_traits/impl/has_member_decl.h>

ccdk_namespace_mpl_util_start

// has_member_less test weather bool T.less( P ) is exists
CCDK_TT_HAS_MEMBER_WITH_RET_DECL(has_member_less, less, bool);

// has_member_greater test weather bool T.greater( P ) is exists
CCDK_TT_HAS_MEMBER_WITH_RET_DECL(has_member_greater, greater, bool);

// has_member_equals test weather bool T.equals( P ) is exists

namespace util_impl {
	template<
		typename T1,
		typename T2,
		bool = has_less_v<T1, T2, bool>,
		bool = has_member_less_v<T1, T2>,
		bool = has_member_greater_v<T2, T1> >
		struct less_impl_t {};

	template<typename T1, typename T2>
	constexpr less_impl_t<T1, T2> less_impl{};

	// T1 == T2 declared
	template<typename T1, typename T2>
	struct less_impl_t<T1, T2, true, false, false> {
		CCDK_FORCEINLINE constexpr bool operator()(T1 const& t1, T2 const& t2) const noexcept {
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
			return t2.greater(t1);
		}
	};
}


//judge pair <t1,t2> is equals
template<
	typename T1,
	typename T2,
	typename = check_t< or_<
		has_less<T1, T2, bool>,
		has_member_less<T1, T2>,
		has_member_greater<T2, T1> >> >
	CCDK_FORCEINLINE constexpr bool less(T1 const& t1, T2 const& t2) noexcept {
	return util_impl::less_impl<T1, T2>(t1, t2);
}


// lex-order compare array
template< typename T1, typename T2, uint32 N, uint32 N2 >
CCDK_FORCEINLINE constexpr bool less(T1 const (&arr1)[N], T2 const (&arr2)[N2]) noexcept {
	DebugValue("size-same array less");
	for (uint32 i = 0; i < min_val<uint32, N, N2>; ++i) {
		if (util_impl::less_impl<T1, T2>(arr1[i], arr2[i])) return true;
		else if (util_impl::less_impl<T2, T1>(arr2[i], arr1[i])) return false;
	}
	return N<N2;
}

// byte type , just use memcmp
template< typename T, uint32 N, uint32 N2, typename = check_t< is_byte<T>> >
CCDK_FORCEINLINE constexpr bool less(T const (&arr1)[N], T const (&arr2)[N2]) noexcept {
	DebugValue("byte array less");
	if (memcmp(arr1, arr2, min_val<uint32,N,N2>) < 0) return true;
	return N < N2;
}

template<typename T, typename P>
CCDK_FORCEINLINE bool compare(T const& t, P const& p) { return util::less(t, p); }


ccdk_namespace_mpl_util_end