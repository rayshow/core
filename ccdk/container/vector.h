#pragma once

#include<ccdk/container/container_mudule.h>
#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/mcontainer/val_pack.h>
#include<ccdk/mpl/util/fill.h>
#include<ccdk/mpl/util/move.h>
#include<ccdk/mpl/iterator/ptr_iterator.h>
#include<ccdk/mpl/iterator/uninitialized_fill.h>
#include<ccdk/mpl/iterator/uninitialized_copy.h>
#include<ccdk/container/filter/filter_view.h>
#include<ccdk/memory/simple_new_allocator.h>
#include<ccdk/mpl/units/ratio.h>

ccdk_namespace_ct_start

using namespace ccdk::mpl;

template<
	typename T,
	typename Size = uint32,                              
	typename Alloc = mem::simple_new_allocator<T>,        
	typename InceaseRatio = units::ratio<2,1>             /* 2.0 incease ratio*/
>
class vector : protected Alloc 
{
public:
	typedef vector		 this_type;
	typedef T			 value_type;
	typedef Size		 size_type;
	typedef Alloc		 allocator_type;
	typedef ptr::diff_t  different_type;
	typedef InceaseRatio incease_ratio;
	
	template<typename T, typename Size2, typename Alloc2, typename Ratio2>
	friend class vector;

private:
	T *content;
	size_type len;
	size_type cap;

	/* clear but not destruct memory */
	CCDK_FORCEINLINE void rvalue_reset() noexcept { content = nullptr; len = 0;  cap = 0; }

	/* we know it is right-value, no need cast */
	CCDK_FORCEINLINE void rvalue_assign(T* inContent,
		size_type inLen, size_type inCap) noexcept {
		content = inContent;
		len = inLen;
		cap = inCap;
		rvalue_reset();
	}

	/* */
	CCDK_FORCEINLINE void alloc_cap(size_type n) {
		T* memory = allocate(n);
		if (!memory) throw std::bad_alloc{};
		content = memory;
		cap = actual_size;
	}

	/* alloc n*increase_ratio elements */
	CCDK_FORCEINLINE void alloc_len(size_type n) {
		size_type actual_size = incease_ratio::multiply(n);
		if (n == size_type(-1) || actual_size < n) throw std::bad_alloc{};
		alloc_cap(actual_size);
		len = n;
	}

	/* realloc n, first deallocate content, then allocate n  */
	CCDK_FORCEINLINE void realloc_len(size_type n) {
		if (content != nullptr) { deallocate(content); content = nullptr }
		alloc_len(n);
	}

	CCDK_FORCEINLINE void realloc_copy(size_type n) {
		size_type actual_size = incease_ratio::multiply(n);
		if (n == size_type(-1) || actual_size < n) throw std::bad_alloc{};
		T* memory = allocate(actual_size);
		if (!memory) throw std::bad_alloc{};
		util::move(memory, content, len);
		cap = actual_size;
		content = memory;
	}

public:
	/* default and nullptr constructor */
	CCDK_FORCEINLINE constexpr vector()
		:content{ nullptr }, len{ 0 }, cap{ 0 } {}
	CCDK_FORCEINLINE constexpr explicit vector(ptr::nullptr_t)
		:content{ nullptr }, len{ 0 }, cap{ 0 } {}

	/* fill*/
	CCDK_FORCEINLINE explicit vector(size_type n, T const& t = T()){
		alloc_len(n);
		uninitialized_fill_n(content, n, t);
	}

	/* copy*/
	CCDK_FORCEINLINE vector(vector const& other) {
		alloc_len(other.cap);
		uninitialized_copy_n(other.content, other.len, content);
	}

	template<typename Size2, typename Alloc2, typename Ratio2>
	CCDK_FORCEINLINE vector(vector<T, Size2, Alloc2, Ratio2> const& other) {
		alloc_cap(other.cap);
		uninitialized_copy_n(other.content, other.len, content);
	}

	/* move */
	CCDK_FORCEINLINE vector(vector && other) noexcept
		:content{ other.content }, len{ other.len }, cap{ other.cap } {
		other.rvalue_reset();
	}

	template<typename Size2, typename Alloc2, typename Ratio2>
	CCDK_FORCEINLINE vector(vector<T, Size2, Alloc2, Ratio2> && other) noexcept
		: content{ other.content }, len{ other.len }, cap{ other.cap } {
		other.rvalue_reset();
	}

	/* swap */
	CCDK_FORCEINLINE swap(vector& other) noexcept {
		util::swap(content, other.content);
		util::swap(len, other.len);
		util::swap(cap, other.cap);
	}

	/* assign  */
	CCDK_FORCEINLINE operator=(ptr::nullptr_t) noexcept { len = 0; }

	CCDK_FORCEINLINE operator=(vector const& other) {
		ccdk_if_not_this(other) { vector{ other }.swap(*this); }
	}

	template<typename Size2, typename Alloc2, typename Ratio2>
	CCDK_FORCEINLINE operator=(vector<T, Size2, Alloc2, Ratio2> const& other) {
		vector{ other }.swap(*this);
	}

	CCDK_FORCEINLINE operator=(vector && other) {
		ccdk_if_not_this(other) { rvalue_assign(other.content, other.len, other.cap) }
	}

	template<typename Size2, typename Alloc2, typename Ratio2>
	CCDK_FORCEINLINE operator=(vector<T, Size2, Alloc2, Ratio2> && other) {
		rvalue_assign(other.content, other.len, other.cap);
	}

	CCDK_FORCEINLINE this_type& assign(ptr::nullptr_t) noexcept { 
		util::destruct_n<T>(content, n);  len = 0; 
	}
	CCDK_FORCEINLINE this_type& assign(size_type n, T const& t) {
		util::destruct_n<T>(content)
		if (n <= cap) { uninitialized_fill_n(content, n, t); len = n; return; }
		realloc_len(n);
	}

	/* attribute*/
	CCDK_FORCEINLINE size_type size() const noexcept { return len; }
	CCDK_FORCEINLINE size_type capacity() const noexcept { return cap; }
	CCDK_FORCEINLINE bool empty() const noexcept { return len == 0; }

	/* access */
	CCDK_FORCEINLINE T& operator[](size_type index) noexcept { return content[index]; }
	CCDK_FORCEINLINE T const& operator[](size_type index) const noexcept {  return content[index];  }
	CCDK_FORCEINLINE T& at(size_type index) noexcept { return content[index]; }
	CCDK_FORCEINLINE T const& at(size_type index) const noexcept { return content[index]; }
	CCDK_FORCEINLINE T& front() noexcept { return content[0]; }
	CCDK_FORCEINLINE T const& front() const noexcept { return content[0]; }
	CCDK_FORCEINLINE T& back() noexcept { return content[len-1]; }
	CCDK_FORCEINLINE T const& back() const noexcept { return content[len-1]; }
	CCDK_FORCEINLINE T* data() noexcept { return content; }
	CCDK_FORCEINLINE T const* data() const noexcept { return content; }

	/*  container operation  */
	CCDK_FORCEINLINE this_type& pop_back() noexcept { 
		if (len > 0) { util::destruct<T>(content + len); --len; } 
	}

	CCDK_FORCEINLINE this_type& push_back(T const& t) noexcept {
		if (len < cap) { util::construct<T>(content + len, t); ++len; }
		else {
			realloc_copy(cap);
		}
	}
};


ccdk_namespace_ct_end