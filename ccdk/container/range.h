#pragma once

#include<ccdk/container/container_mudule.h>
#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/fusion/pair.h>
#include<ccdk/mpl/function/operator.h>
#include<ccdk/mpl/util/compare.h>

ccdk_namespace_ct_start

using namespace mpl;

template<typename T>
struct range_iterator {
	using this_type = range_iterator;

	T value;

	CCDK_FORCEINLINE this_type& operator++() { ++value; }
	CCDK_FORCEINLINE this_type& operator--() { --value; }
	CCDK_FORCEINLINE this_type  operator++(int) { { value + 1}; }
	CCDK_FORCEINLINE this_type  operator--(int) { --value; }

};

template<typename T>
struct range : public fs::pair<T,T> {

	CCDK_FORCEINLINE range merge(fs::pair<T, T> const& other) {
		return { fn::min(_1, other._1), fn::min(_2, other._2) };
	}

	CCDK_FORCEINLINE range intersect(fs::pair<T, T> const& other) {
		T& min = _1 < other._1 ? other._1 : _1;
		T& max = _2 < other._2 ? _2 : other._2;

		// max greater or equal with min
		if (min>=max) {
			return range{};
		}
		return { min,max };
	}

	CCDK_FORCEINLINE bool is_valid() {
		return max > min;
	}
};




ccdk_namespace_ct_end