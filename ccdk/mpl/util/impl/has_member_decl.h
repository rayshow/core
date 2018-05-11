#pragma once

#define CCDK_MPL_HAS_MEMBER_DECL(name, fn, ret)                          \
namespace ut_impl                                                        \
{                                                                        \
	template<typename T1, typename T2>                                   \
		struct has_member_##name##_impl {                                \
		template< typename P1,                                           \
			typename P2,                                                 \
			/*check exists*/                                             \
			typename R = decltype(makeval<P1>().##fn##(declval<P2>())),  \
			/*check return is ret */                                     \
			typename = check_t< is_same<R, ret>> >                       \
		constexpr static bool sfinae(int) { return true; }               \
		template<typename P1, typename = P2>                             \
		constexpr static bool sfinae(...) { return false; }              \
		constexpr static bool value = sfinae<T1, T2>(0);                 \
	};                                                                   \
}                                                                        \
template<typename T1, typename T2>                                       \
struct has_member_##name:bool_< ut_impl::has_member_##name##_impl<T1, T2>::value > {}; \
template<typename T1, typename T2>                                       \
static constexpr bool  has_member_##name##_v = ut_impl::has_member_##name##_impl<T1, T2>::value;