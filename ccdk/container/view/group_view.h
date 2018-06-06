#pragma once
#include<ccdk/container/container_mudule.h>
#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/fusion/pair.h>
#include<ccdk/mpl/iterator/reverse_iterator.h>
#include<ccdk/memory/allocator/simple_new_allocator.h>

ccdk_namespace_ct_start

using namespace mpl;

template<typename ValueType>
struct group_node {
	using this_type = group_node<ValueType>;

};

//a batch of view
template<
	typename View,
	typename Alloc = mem::simple_new_allocator<View,uint32> >
struct group_view : protected Alloc
{
public:
	using size_type       = uint32;
	using difference_type = int32;

private:
	View * content;

	// size, 

public:

	//
	group_view() noexcept : content{ nullptr } {}
	group_view(ptr::nullptr_t) noexcept : content{ nullptr } {}

	group_view(uint32 reserve) :content{ nullptr } {
		allocate(reserve);
	}


private:

	CCDK_FORCEINLINE void allocate(size_type n) {

	}

};

ccdk_namespace_ct_end