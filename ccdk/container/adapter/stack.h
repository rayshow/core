#pragma once

#include<ccdk/container/container_mudule.h>
#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/container/vector.h>

ccdk_namespace_ct_start

using namespace ccdk::mpl;

template<
	typename T,
	typename Size = uint32 ,
	typename Alloc = mem::simple_new_allocator<T,Size>,
	typename Base = vector<T, units::ratio<2,1>, 20, Size, Alloc>
>
class stack
{
public:
	using base_type       = Base;
	using this_type       = stack;
	using value_type      = T;
	using pointer         = T * ;
	using const_pointer   = T const*;
	using reference       = T & ;
	using const_reference = T const&;
	using size_type       = Size;
	using difference_type = to_signed_t<size_type>;
private:
	Base  content;
public:

	//empty ctor
	CCDK_FORCEINLINE stack() :content{} {}

	//copy ctor
	CCDK_FORCEINLINE stack(this_type const& other) : content{ other.content } {}

	//move ctor
	CCDK_FORCEINLINE stack(this_type&& other) : content{ util::move(other.content) } {}

	//base container copy ctor
	CCDK_FORCEINLINE stack(Base const& base) : content{ base } {}

	//base container move ctor
	CCDK_FORCEINLINE stack(Base && base) : content{ util::move(base) } {}

	//array ctor
	template<uint32 N>
	CCDK_FORCEINLINE stack(T const(&arr)[N]) : content{ arr } {}

	//swap
	CCDK_FORCEINLINE void swap(this_type& other) {
		util::swap(content, other.content);
	}

	//assign copy
	CCDK_FORCEINLINE this_type& operator=(this_type const& other) {
		content = other.content;
		return *this;
	}

	//assign move
	CCDK_FORCEINLINE this_type& operator=(this_type && other) {
		content = util::move(other.content);
		return *this;
	}

	//push / pop
	CCDK_FORCEINLINE void pop() noexcept { content.pop_back(); }
	CCDK_FORCEINLINE void push(T const& t) { content.push_back(t); }
	CCDK_FORCEINLINE void push(T && t) { content.push_back(util::move(t)); }
	template<typename... Args>
	CCDK_FORCEINLINE void emplace(Args &&... args) { content.emplace_back(util::forward<Args>(args)...); }

	//top
	CCDK_FORCEINLINE reference top() noexcept { return content.back(); }
	CCDK_FORCEINLINE const_reference top() const noexcept { return content.back(); }


	//size / empty
	CCDK_FORCEINLINE size_type size() const noexcept { return content.size(); }
	CCDK_FORCEINLINE size_type empty() const noexcept { return content.empty(); }
};


// local stack
template<
	typename T,
	uint32 KLeastElements = 128,
	typename Size = uint32 ,
	typename Alloc = mem::simple_new_allocator<T, Size> >
using local_stack = stack<T, Size, Alloc,
	local_vector<T, KLeastElements, units::ratio<2, 1>, Size, Alloc> >;



ccdk_namespace_ct_end