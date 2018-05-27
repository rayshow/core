#pragma once
#include<ccdk/container/container_mudule.h>
#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/mcontainer/val_pack.h>
#include<ccdk/mpl/function/function.h>
#include<ccdk/mpl/fusion/maybe.h>
#include<ccdk/container/range.h>

ccdk_namespace_ct_start

using namespace mpl;

//readonly view, store range, only for random container
template<typename Container>
struct const_view
{
public:
	using value_type      = typename Container::value_type;
	using size_type       = typename Container::size_type;
	using difference_type = typename Container::difference_type;
	using pointer         = typename Container::pointer;
	using const_pointer   = typename Container::const_pointer;
	using reference       = typename Container::reference;
	using const_reference = typename Container::const_reference;
	using range_type = range<const_pointer>;

	range_type  content;

	//empty range
	CCDK_FORCEINLINE const_view() : content{} {}
	CCDK_FORCEINLINE const_view(ptr::nullptr_t) : content{} {}

	//range-n ctor
	CCDK_FORCEINLINE const_view(const_pointer begin, size_type len)
		noexcept : content{ begin, content + len } {}
	CCDK_FORCEINLINE const_view(const_pointer begin, const_pointer end)
		noexcept : content{ begin, end } {}
	CCDK_FORCEINLINE const_view(const const_view& other)
		noexcept : content{ other.content } {}




};

ccdk_namespace_ct_end