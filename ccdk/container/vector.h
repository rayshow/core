#pragma once

#include<ccdk/container/container_mudule.h>
#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/mcontainer/val_pack.h>
#include<ccdk/mpl/iterator/ptr_iterator.h>
#include<ccdk/mpl/util/move.h>
#include<ccdk/mpl/util/fill.h>
#include<ccdk/container/filter/filter_view.h>
#include<ccdk/memory/simple_new_allocator.h>

ccdk_namespace_ct_start

using namespace ccdk::mpl;

template<
	typename T,
	typename Size = uint32,
	typename Alloc = mem::simple_new_allocator<T>
>
class vector : protected Alloc
{
public:
	typedef vector   this_type;
	typedef T        value_type;
	typedef Size     size_type;

private:
	T*        content;
	size_type pos;
	size_type cap;

	
	void rvalue_reset() noexcept { content = nullptr; pos = 0;  cap = 0;  }

	/* we know it is right-value, no need cast */
	void rvalue_assign(vector& other) noexcept { content = other.content; pos = other.pos;  cap = other.cap; rvalue_reset();  }

public:
	/* default and nullptr constructor */
	CCDK_FORCEINLINE constexpr vector() :content{ nullptr }, pos{ 0 }, cap{ 0 } {}
	CCDK_FORCEINLINE constexpr explicit vector(ptr::nullptr_t) :content{ nullptr }, pos{ 0 }, cap{ 0 } {}

	/* */
	CCDK_FORCEINLINE explicit vector(size_type n, T const& t = T()) {}

	/* copy */
	CCDK_FORCEINLINE vector(vector const& other) {}

	/* move */
	CCDK_FORCEINLINE vector(vector && other) 
		:content{ other.content },
		pos{ other.pos },
		cap{ other.cap } 
	{
		other.rvalue_reset(); 
	}

	/* swap */
	CCDK_FORCEINLINE swap(vector& other)
	{
		util::swap(content, other.content);
		util::swap(pos, other.pos);
		util::swap(cap, other.cap);
	}


	/* assign  */
	CCDK_FORCEINLINE operator=(ptr::nullptr_t) { pos = 0; } 
	CCDK_FORCEINLINE operator=(vector const& other) { ccdk_if_not_this(other) { vector{ other }.swap(*this); } }
	template<typename Size2, typename Alloc2>
	CCDK_FORCEINLINE operator=(vector<T, Size2,Alloc2> const& other) { vector{ other }.swap(*this); }
	CCDK_FORCEINLINE operator=(vector && other) { ccdk_if_not_this(other) { rvalue_assign(other); } }

};


ccdk_namespace_ct_end