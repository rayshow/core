#pragma once

#include<ccdk/container/container_mudule.h>
#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/iterator/iterator_traits.h>
#include<ccdk/mpl/util/copy.h>
#include<ccdk/algorithm/distance.h>
#include<ccdk/memory/simple_new_allocator.h>
#include<ccdk/memory/allocator_traits.h>


ccdk_namespace_ct_start

using namespace ccdk::mpl;

template<typename T>
struct bit_access {
	T&     val;
	uint32 pos;

	CCDK_FORCEINLINE void operator=(bool bit) noexcept {
		val = (val & ~(1 << pos)) | (bit << pos);
	}

	CCDK_FORCEINLINE operator bool() { return val & 1 << pos; }

	CCDK_FORCEINLINE T* addressof() noexcept { return &val; }

	CCDK_FORCEINLINE T const * addressof() const noexcept { return &val; }
};


struct bitset_iterator {

	typedef bitset_iterator			 this_type;
	typedef bool					 value_type;
	typedef random_category			 category;

	uint8*  pointer;
	uint32  pos;

	CCDK_FORCEINLINE this_type& operator++() noexcept {
		++pos;
		return *this;
	}

	/* it++ */
	CCDK_FORCEINLINE constexpr this_type operator++(int) const noexcept {
		return { pointer, pos +1 };
	}

	/* it+step */
	CCDK_FORCEINLINE constexpr this_type operator+(ptr::size_t step) const noexcept {
		return { pointer, pos + step };
	}

	/* it+=step */
	CCDK_FORCEINLINE this_type& operator+=(ptr::size_t step) noexcept {
		pos += step;
		return *this;
	}

	/* const */
	CCDK_FORCEINLINE bool operator*() const noexcept {
		return pointer[pos / 8] & 1 << (pos % 8);
	}

	/* index */
	CCDK_FORCEINLINE bool operator[](ptr::size_t index) const noexcept {
		ptr::size_t id = index + pos;
		return pointer[id / 8] & 1 << (id % 8);
	}

	/* cmp */
	CCDK_FORCEINLINE bool operator==(this_type const& other) const noexcept {
		return pointer == other.pointer && pos == other.pos;
	}

	/* cmp */
	CCDK_FORCEINLINE bool operator!=(this_type const& other) const noexcept {
		return pointer != other.pointer || pos != other.pos;
	}
};

struct reverse_bitset_iterator {

	typedef reverse_bitset_iterator	 this_type;
	typedef bool					 value_type;
	typedef random_category			 category;

	uint8* pointer;
	int32  pos;

	CCDK_FORCEINLINE this_type& operator++() noexcept {
		--pos;
		return *this;
	}

	/* it++ */
	CCDK_FORCEINLINE constexpr this_type operator++(int) const noexcept {
		return { pointer, pos - 1 };
	}

	/* it+step */
	CCDK_FORCEINLINE constexpr this_type operator+(ptr::size_t step) const noexcept {
		return { pointer, pos - step };
	}

	/* it+=step */
	CCDK_FORCEINLINE this_type& operator+=(ptr::size_t step) noexcept {
		pos -= step;
		return *this;
	}

	/* const */
	CCDK_FORCEINLINE bool operator*() const noexcept {
		return pointer[pos / 8] & 1 << (pos % 8);
	}

	/* index */
	CCDK_FORCEINLINE bool operator[](ptr::size_t index) const noexcept {
		ptr::size_t id = index - pos;

		return pointer[id / 8] & 1 << (id % 8);
	}

	/* cmp */
	CCDK_FORCEINLINE bool operator==(this_type const& other) const noexcept {
		return pointer == other.pointer && pos == other.pos;
	}

	/* cmp */
	CCDK_FORCEINLINE bool operator!=(this_type const& other) const noexcept {
		return pointer != other.pointer || pos != other.pos;
	}
};

template<
	typename T = uint8, 
	typename Alloc = mem::simple_new_allocator<T>>
class bitset : protected Alloc
{
public:
	typedef bool                          value_type;
	typedef T*                            pointer_type;
	typedef T const*                      const_reference;
	typedef bit_access<T>                 reference_type;
	typedef const bit_access<T>           const_reference_type;
	typedef uint32                        size_type;
	typedef int32                         difference_type;
	typedef mem::allocator_traits<Alloc>  allocate_type;

	typedef bitset_iterator               iterator_type;
	typedef const bitset_iterator         const_iterator_type;
	typedef reverse_bitset_iterator       reverse_iterator_type;
	typedef const reverse_bitset_iterator const_reverse_iterator_type;
private:
	pointer_type  content;
	size_type     len;

public:

	CCDK_FORCEINLINE constexpr bitset() :content{ nullptr }, len{ 0 } {}

	/* fill byte*/
	CCDK_FORCEINLINE bitset(size_type n, bool value = false) {
		allocate_fill(n, value);
	}
	
	/* range-n copy */
	template<typename InputIt, 
		typename = check< is_iterator<InputIt>> >
	CCDK_FORCEINLINE bitset(InputIt beginIt, size_type n) {
		allocate_copy(n, beginIt);
	}

	/* range copy */
	template<typename InputIt,
		typename = check< is_iterator<InputIt>> >
	CCDK_FORCEINLINE bitset(InputIt beginIt, InputIt endIt) {
		ptr::size_t n = alg::distance(beginIt, endIt);
		allocate_copy(n, beginIt);
	}

	/* copy */
	CCDK_FORCEINLINE bitset(bitset const& other):
		content{ allocate_type::allocate(*this, other.len) }, len{other.len} {
		memcpy(content, other.content, other.len);
	}

	/* move */
	CCDK_FORCEINLINE bitset(bitset && other)  noexcept
		: content{ other.content }, len{ other.len } {
		other.rvalue_reset();
	}

	CCDK_FORCEINLINE this_type& operator=(bitset const& other) {

	}

	/* iterator */
	CCDK_FORCEINLINE iterator_type begin() 
		noexcept { return { content, 0 }; }
	CCDK_FORCEINLINE iterator_type end() 
		noexcept { return { content, len }; }
	CCDK_FORCEINLINE const_iterator_type cbegin() 
		const noexcept { return { content, 0 }; }
	CCDK_FORCEINLINE const_iterator_type cend() 
		const noexcept { return { content, len }; }
	CCDK_FORCEINLINE reverse_iterator_type rbegin() 
		noexcept { return { content, len-1 };}
	CCDK_FORCEINLINE reverse_iterator_type rend() 
		noexcept { return { content, -1 }; }
	CCDK_FORCEINLINE const_reverse_iterator_type crbegin() 
		const noexcept { return { content, len-1 }; }
	CCDK_FORCEINLINE const_reverse_iterator_type crend() 
		const noexcept { return { content, -1 }; }

	/* attribute */
	CCDK_FORCEINLINE size_type length() { return len; }

private:

	CCDK_FORCEINLINE void rvalue_reset() {
		len = 0;
		content = nullptr;
	}

	CCDK_FORCEINLINE void allocate_fill(size_type n, bool val) {
		if (n == 0) return;
		ptr::size_t count = (n + sizeof(T)-1 ) / sizeof(T);
		T* memory = allocate_type::allocate(*this, count);
		uint8 mask = val ? 0b11111111 : 0b00000000;
		memset(memory, mask, sizeof(T)*count);
		len = count;
		content = memory;
	}

	template<typename InputIt>
	CCDK_FORCEINLINE void allocate_copy(size_type n, InputIt beginIt) {
		if (n == 0) return;
		ptr::size_t count = (n + sizeof(T) - 1) / sizeof(T);
		T* memory = allocate_type::allocate(*this, count);
		util::copy_n(begin(), beginIt, n);
		len = count;
		content = memory;
	}

};

ccdk_namespace_ct_end