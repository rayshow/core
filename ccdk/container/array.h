#pragma once

#include<ccdk/container/container_mudule.h>
#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/mcontainer/make_indice.h>
#include<ccdk/mpl/util/move.h>
#include<ccdk/container/filter_view.h>

ccdk_namespace_ct_start

using namespace ccdk::mpl;

template<typename T, uint32 N>
class array
{
public:
	typedef T      value_type;
	typedef array  this_type;
	typedef filter_view_t<this_type> view_type;
	typedef filter_t<T>              filter_type;
private:
	T content[N];

	/* copy compile time c-array to content */
	template<uint32... indice>
	CCDK_FORCEINLINE constexpr array(T const* arr, mpl::val_pack<uint32, indice...> ) : content{ arr[indice]... } {}

public:

	/* compile-time copy, to delegate constructor  */
	template<uint32 M, typename = mpl::check_lequal<M, N>>
	CCDK_FORCEINLINE constexpr array(const T(& arr)[M]) : array(arr, mpl::make_indice<M>{}) {}

	CCDK_FORCEINLINE  view_type operator[](filter_type&& filter)
	{
		view_type ret{ *this, util::move(filter) };
		return ret;
	}

	CCDK_FORCEINLINE T& operator[](uint32 index)
	{
		return content[index];
	}
	
	T const* data() const { return content; }
};

ccdk_namespace_ct_end