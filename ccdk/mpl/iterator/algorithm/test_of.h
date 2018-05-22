#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/type_traits/impl/has_member_decl.h>
#include<ccdk/mpl/iterator/algorithm/foreach.h>

ccdk_namespace_mpl_it_start

// some container like bit_set  defined more efficient any/all/none of 
CCDK_TT_HAS_MEMBER_WITH_RET_DECL(any_of, any, bool);
CCDK_TT_HAS_MEMBER_WITH_RET_DECL(all_of, all, bool);
CCDK_TT_HAS_MEMBER_WITH_RET_DECL(none_of, all, bool);

//////////////////////////////////////////////////////////////////////////////
//// test weather no one element of Container/range is false

template<
	typename Container, 
	typename = check_t< has_member_all_of<Container>> >
CCDK_FORCEINLINE bool all_of(Container const& ct) noexcept { return ct.all(); }

template<
	typename Container,
	typename = check_t<not_<has_member_all_of<Container>> >,
	typename = check_t<has_nest_iterator<Container>> >
CCDK_FORCEINLINE bool all_of(Container const& ct) noexcept {
	return all_of(ct.begin(), ct.end());
}

template<
	typename InputIt,
	typename = check_t< is_iterator< InputIt >>>
CCDK_FORCEINLINE bool any_of(InputIt begin, InputIt end) noexcept {
	foreach([](auto const& v) { if (!v) return false; });
	return true;
}

//////////////////////////////////////////////////////////////////////////////
//// test weather at least one element of Container/range is true
template<
	typename Container,
	typename = check_t< has_member_any_of<Container>> >
	CCDK_FORCEINLINE bool any_of(Container const& ct) noexcept { return ct.any(); }

template<
	typename Container,
	typename = check_t<not_<has_member_any_of<Container>> >,
	typename = check_t<has_nest_iterator<Container>> >
CCDK_FORCEINLINE bool any_of(Container const& ct) noexcept {
	return any_of(ct.begin(), ct.end());
}

template<
	typename InputIt,
	typename = check_t< is_iterator< InputIt >>>
CCDK_FORCEINLINE bool any_of(InputIt begin, InputIt end) noexcept {
	foreach([](auto const& v) { if (v) return true; });
	return false;
}

//////////////////////////////////////////////////////////////////////////////
//// test weather no one element of Container/range is true

template<
	typename Container,
	typename = check_t< has_member_any_of<Container>> >
CCDK_FORCEINLINE bool none_of(Container const& ct) noexcept { return !any_of(ct); }

template<
	typename InputIt,
	typename = check_t< is_iterator< InputIt >>>
CCDK_FORCEINLINE bool none_of(InputIt begin, InputIt end) noexcept { return !any_of(begin, end);}

ccdk_namespace_mpl_it_end