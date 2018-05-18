#pragma once

#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/iterator/algorithm/foreach.h>
#include<ccdk/mpl/type_traits/impl/has_member_decl.h>

ccdk_namespace_mpl_it_start

// some container like bit_set  defined more efficient any/all of 
CCDK_TT_HAS_MEMBER_DECL(any_of, any_of, bool);
CCDK_TT_HAS_MEMBER_DECL(all_of, all_of, bool);

template<
	typename Container, 
	typename = check_t< has_member_all_of<Container,null_>>
>
CCDK_FORCEINLINE bool all_of(Container const& ct) {
	foreach([](auto const& elem) { if (!elem) return false; });
	return true;
}

template<typename Container>
CCDK_FORCEINLINE bool any_of(Container const)





ccdk_namespace_mpl_it_end