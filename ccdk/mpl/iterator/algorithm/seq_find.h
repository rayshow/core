#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/type_traits/impl/has_member_decl.h>
#include<ccdk/mpl/iterator/iterator_traits.h>
#include<ccdk/mpl/iterator/reverse_iterator.h>

ccdk_namespace_mpl_it_start

namespace alg_impl {

	template<typename RandomIt>
	uint32* build_next(RandomIt begin, uint32 n) {
		uint32 *next = new uint32[n];
		uint32 fore = 0;
		next[0] = 0;
		for (uint32 i = 1; i < n; ++i) {
			if (begin[fore] == begin[i]) { next[i] = ++fore; }
			else { fore = 0; next[i] = 0; }
		}
		return next;
	}

	template<typename RandomIt, typename RandomIt2>
	ptr::size_t kmp(uint32* next,
		RandomIt source, ptr::size_t n1,
		RandomIt2 search, uint32 n2) noexcept {

		/* search */
		ptr::size_t offset = 0;
		ptr::size_t i = 0;
		while (i < n1 && offset< n2) {
			// same char, simultaneous move to next char 
			if (search[offset] == source[i]) { ++offset; ++i;}
			// fore have same part 
			else if (offset > 0)  {
				if (next[offset] == 0 && next[offset - 1] > 0) {
					offset = next[offset - 1]; /* move to next*/ 
				}
				else { offset = 0; ++i; /* next[offset]!=0 || next[offset-1] == 0 */ }
			}
			else { ++i; /* not equal && offset == 0, source move to next char */ }
		}
		if (offset == n2) return i - n2;
		return n1;
	}

	//N > 0, forward search N-th match
	template< 
		int32 N, 
		typename RandomIt,
		typename RandomIt2>
	CCDK_FORCEINLINE ptr::size_t seq_find_impl(mpl::false_,
		RandomIt source, ptr::size_t n1,
		RandomIt2 search, uint32 n2)
	{
		uint32* next = build_next(search, n2);
		ptr::size_t next_start = 0;
		RandomIt end = source + n1;
		for (uint32 i = 0; i < N && next_start < n1; ++i) {
			next_start = kmp(next, source+next_start,
				n1 - next_start, search, n2);
		}
		delete[] next;
		return next_start;
	}

	// N < 0, backward search N-th match
	template< 
		int32 N,
		typename RandomIt,
		typename RandomIt2,
		typename iterator = reverse_iterator<RandomIt>,
		typename iterator2 = reverse_iterator<RandomIt2>>
	CCDK_FORCEINLINE ptr::size_t seq_find_impl(mpl::true_,
		RandomIt source, ptr::size_t n1,
		RandomIt2 search, uint32 n2)
	{
		uint32* next = build_next(iterator2{ search+n2-1 }, n2);
		iterator begin = iterator{ source + n1 - 1 };
		iterator end = iterator{ source - 1 };
		ptr::size_t next_start = 0;
		for (uint32 i = 0; i < -N && next_start < n1; ++i) {
			next_start = kmp(next, begin + next_start,
				n1 - next_start, iterator2{ search }, n2);
		}
		delete[] next;
		return next_start;
	}

}


template< int32 N,
	typename RandomIt,
	typename RandomIt2,
	typename = check< N!=0>,
	typename = check_t< is_random_iterator<RandomIt>>,
	typename = check_t< is_random_iterator<RandomIt2>> >
CCDK_FORCEINLINE ptr::size_t seq_find(
	RandomIt source, ptr::size_t n1,
	RandomIt2 search, uint32 n2)
{
	return alg_impl::seq_find_impl<N>(mpl::bool_c<(N < 0)>, source, n1, search, n2);
}




ccdk_namespace_mpl_it_end