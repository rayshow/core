#pragma once

#include<ccdk/container/container_mudule.h>
#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/mcontainer/val_pack.h>
#include<ccdk/mpl/iterator/ptr_iterator.h>
#include<ccdk/mpl/iterator/uninitialized_fill.h>
#include<ccdk/mpl/iterator/uninitialized_copy.h>
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
	typedef Alloc    allocator_type;
	typedef ptr::diff_t different_type;
	constexpr static float increase_ratio = 2.0f;

	template<typename T, typename Size2, typename Alloc2>
	friend class vector;

private:
	T * content;
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
	void alloc_cap(size_type n) {
		T* memory = allocator_type::allocate(actual_size);
		if (!memory) throw std::bad_alloc{};
		content = memory;
		len = n;
		cap = actual_size;
	}

	/* alloc n*increase_ratio elements */
	void alloc_len(size_type n) {
		size_type actual_size = (float)n * increase_ratio;
		if (n == size_type(-1) || actual_size < n)
			throw std::bad_alloc{};
		alloc_cap(actual_size);
	}

public:
	/* default and nullptr constructor */
	CCDK_FORCEINLINE constexpr vector()
		:content{ nullptr }, len{ 0 }, cap{ 0 } {}
	CCDK_FORCEINLINE constexpr explicit vector(ptr::nullptr_t)
		:content{ nullptr }, len{ 0 }, cap{ 0 } {}

	/* fill*/
	CCDK_FORCEINLINE explicit vector(size_type n, T const& t = T())
	{
		alloc_len(n);
		uninitialized_fill_n(content, n, t);
	}

	/* copy*/
	CCDK_FORCEINLINE vector(vector const& other) {
		alloc_cap(other.cap);
		uninitialized_copy_n(other.content, other.len, content);
	}

	template<typename Size2, typename Alloc2>
	CCDK_FORCEINLINE vector(vector<T, Size2, Alloc2> const& other) {
		alloc_cap(other.cap);
		uninitialized_copy_n(other.content, other.len, content);
	}

	/* move */
	CCDK_FORCEINLINE vector(vector && other) noexcept
		:content{ other.content }, len{ other.len }, cap{ other.cap } {
		other.rvalue_reset();
	}

	template<typename Size2, typename Alloc2>
	CCDK_FORCEINLINE vector(vector<T, Size2, Alloc2> && other) noexcept
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

	template<typename Size2, typename Alloc2>
	CCDK_FORCEINLINE operator=(vector<T, Size2, Alloc2> const& other) {
		vector{ other }.swap(*this);
	}

	CCDK_FORCEINLINE operator=(vector && other) {
		ccdk_if_not_this(other) { rvalue_assign(other.content, other.len, other.cap) }
	}

	template<typename Size2, typename Alloc2>
	CCDK_FORCEINLINE operator=(vector<T, Size2, Alloc2> && other) {
		rvalue_assign(other.content, other.len, other.cap);
	}

	CCDK_FORCEINLINE this_type& assign(ptr::nullptr_t) noexcept { len = 0; }
	CCDK_FORCEINLINE this_type& assign(size_type n, T const& t) {
		if(n<=len) fill()
	}


	/* attribute*/
	CCDK_FORCEINLINE size_type size() const noexcept { return len; }
	CCDK_FORCEINLINE size_type capacity() const noexcept { return cap; }
	CCDK_FORCEINLINE bool empty() const noexcept { return len == 0; }

	/* access */
	CCDK_FORCEINLINE T& operator[](size_type index) noexcept { return content[index]; }
	CCDK_FORCEINLINE T const& operator[](size_type index)  const noexcept {  return content[index];  }
	CCDK_FORCEINLINE T& at(size_type index) noexcept { return content[index]; }
	CCDK_FORCEINLINE T const& at(size_type index) const noexcept { return content[index]; }
	CCDK_FORCEINLINE T& front() noexcept { return content[0]; }
	CCDK_FORCEINLINE T const& front() const noexcept { return content[0]; }
	CCDK_FORCEINLINE T& back() noexcept { return content[len-1]; }
	CCDK_FORCEINLINE T const& back() const noexcept { return content[len-1]; }
	CCDK_FORCEINLINE T* data() noexcept { return content; }
	CCDK_FORCEINLINE T const* data() const noexcept { return content; }
};


ccdk_namespace_ct_end