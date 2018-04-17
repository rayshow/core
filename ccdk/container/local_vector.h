#pragma once

#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/mcontainer/val_pack.h>
#include<ccdk/mpl/function/operator.h>
#include<ccdk/mpl/iterator/ptr_iterator.h>
#include<ccdk/mpl/iterator/reverse_iterator.h>
#include<ccdk/mpl/fusion/local_arr.h>
#include<ccdk/mpl/util/fill.h>
#include<ccdk/mpl/util/move.h>
#include<ccdk/mpl/util/copy.h>
#include<ccdk/mpl/util/construct.h>
#include<ccdk/mpl/units/ratio.h>
#include<ccdk/memory/simple_new_allocator.h>
#include<ccdk/memory/allocator_traits.h>
#include<ccdk/container/filter/range_filter.h>
#include<ccdk/container/algorithm/distance.h>
#include<ccdk/container/container_mudule.h>

ccdk_namespace_ct_start

using namespace ccdk::mpl;

// semi-stack-memory vector,
// if content size not over InitSize, all object store in stack
// else allocate from heap, all object transform to new heap memory
template<
	uint32 InitSize,
	typename T,
	typename InceaseRatio = units::ratio<2, 1>, /* 2X incease ratio*/
	typename Alloc = mem::simple_new_allocator<T>
>
class local_vector : protected Alloc
{
public:
	using this_type = local_vector;
	using increase_ratio = InceaseRatio;

	using value_type = T;
	using pointer = T * ;
	using const_pointer = T const*;
	using reference = T & ;
	using const_reference = T const&;
	using size_type = uint32;
	using difference_type = int32;
	using allocator_type = mem::allocator_traits< Alloc >;

	using iterator = T * ;
	using const_iterator = T const *;
	using reverse_iterator = it::reverse_iterator<T*>;
	using const_reverse_iterator = it::reverse_iterator<T const*>;

	constexpr static uint32 kMaxSize = size_type(-1) / sizeof(T);

private:
	fs::local_arr<T, InitSize> local_part; // if size below InitSize, all content store in local part
	pointer                    heap_part;  // once allocated, stack_part is useless, all object store in heap part
	pointer                    content;    // actually pointer to memory operating
	size_type                  cap;        // total elements size
	size_type                  len;        // current used elements size

public:

	/* default ctor */
	local_vector() = default;
	CCDK_FORCEINLINE local_vector(ptr::nullptr_t) noexcept
		: local_part{}, heap_part{ nullptr }, content{ nullptr }, 
		cap{ 0 }, len{ 0 } {}

	// fill n with args
	template<typename... Args>
	CCDK_FORCEINLINE local_vector(size_type n, Args&& ... args)
		: local_part{ n, util::forward<Args>(args)... },
		heap_part{ nullptr }, content{ local_part.address() },
		len{ n }, cap{ InitSize }
	{
		// InitSize is set too small if assert
		ccdk_assert(n <= InitSize);
	}

	// range-n copy ctor
	template<typename InputIt, typename = check_t< is_iterator<InputIt>>>
	CCDK_FORCEINLINE local_vector(InputIt beginIt, size_type n) {
		ccdk_assert(n <= InitSize);
		initialize_copy(beginIt, n)
	}

	// range copy ctor
	template<typename InputIt, typename = check_t< is_iterator<InputIt>>>
	CCDK_FORCEINLINE local_vector(InputIt beginIt, InputIt endIt) {
		ccdk_assert(n <= InitSize);
		initialize_copy(beginIt, alg::distance(beginIt, endIt))
	}

	// copy ctor
	CCDK_FORCEINLINE local_vector(local_vector const& other) {
		ccdk_assert(other.size() < InitSize);
		initialize_copy(other.data(), other.size())
	}

	// move ctor
	CCDK_FORCEINLINE local_vector(local_vector && other) noexcept :
		local_part{}, content{ other.data() }, heap_part{ other.data()},
		len {n}, cap{ other.size() }
	{
		//store in stack 
		if (!heap_part) {
			content = local_part.address();
			util::construct_move_n(content, other.begin(), other.size());
		}
		other.rvalue_reset();
	}

	//template copy
	template<size_type N2, typename Ratio2, typename Alloc2>
	CCDK_FORCEINLINE local_vector(local_vector<N2, T, Ratio2, Alloc2> const& other) {
		ccdk_assert(other.size() < InitSize);
		initialize_copy(other.data(), other.size())
	}

	//template move
	template<size_type N2, typename Ratio2>
	CCDK_FORCEINLINE local_vector(local_vector<N2, T, Ratio2, Alloc> && other) noexcept:
		local_part{}, content{ other.data() }, heap_part{ other.data() },
		len{ n }, cap{ other.size() }
	{
		//store in stack 
		if (!heap_part) {
			content = local_part.address();
			util::construct_move_n(content, other.begin(), other.size());
		}
		other.rvalue_reset();
	}

	//swap, there are 4 kinds of situation
	CCDK_FORCEINLINE void swap(local_vector& other) noexcept {
		if (heap_part && other.heap_part) {
			util::swap(heap_part, other.heap_part);
			content = heap_part;
		}
		else if(!heap_part && !other.heap_part){
			util::swap(local_part, other.local_part);
			content = local_part.address();
		}
		else if (!heap_part && other.heap_part) {
			heap_part = other.heap_part;
			other.heap_part = nullptr;
			other.local_part = local_part;
			local_part.rvalue_reset();
			content = heap_part;
		}
		else {
			other.heap_part = heap_part;
			heap_part = nullptr;
			local_part = other.local_part;
			other.local_part.rvalue_reset();
			content = local_part.address();
		}
		util::swap(len, other.len);
		util::swap(cap, other.cap);
	}

	CCDK_FORCEINLINE operator=(local_vector const& other) {
		destruct_content();

	}

	CCDK_FORCEINLINE operator=(local_vector && other) {

	}

	//attr
	CCDK_FORCEINLINE size_type size() const noexcept { return len; }
	CCDK_FORCEINLINE size_type capcity() const noexcept { return cap; }
	CCDK_FORCEINLINE constexpr size_type max_size() const noexcept { return kMaxSize; }

	//access
	CCDK_FORCEINLINE pointer data() noexcept { return content; }
	CCDK_FORCEINLINE const_pointer data() const noexcept { return content; }
	CCDK_FORCEINLINE reference operator[](size_type idx) noexcept { return content[idx]; }
	CCDK_FORCEINLINE const_reference operator[](size_type idx) const noexcept { return content[idx]; }
	CCDK_FORCEINLINE reference at(size_type idx) noexcept { return content[idx]; }
	CCDK_FORCEINLINE const_reference at(size_type idx) const noexcept { return content[idx]; }

	//iterator
	CCDK_FORCEINLINE iterator begin() noexcept { return content; }
	CCDK_FORCEINLINE iterator end() noexcept { return content + len; }
	CCDK_FORCEINLINE const_iterator cbegin() const noexcept { return content; }
	CCDK_FORCEINLINE const_iterator cend() const noexcept { return content + len; }
	CCDK_FORCEINLINE reverse_iterator rbegin() noexcept { return { content +len-1 }; }
	CCDK_FORCEINLINE reverse_iterator rend() noexcept { return {content-1}; }
	CCDK_FORCEINLINE const_reverse_iterator crbegin() const noexcept { return { content + len - 1 }; }
	CCDK_FORCEINLINE const_reverse_iterator crend() const noexcept { return { content - 1 }; }

private:

	CCDK_FORCEINLINE void destruct_content() noexcept {
		if (len > 0) util::destruct_n(content, len);
	}

	CCDK_FORCEINLINE void deallocate() noexcept {
		if (heap_part) allocator_type::deallocate(*this, heap_part, len);
	}

	CCDK_FORCEINLINE void rvalue_reset() noexcept {
		len = 0;
		cap = 0;
		//use stack memory
		if (!heap_part) local_part.rvalue_reset();
		heap_part = nullptr;
		content = nullptr;
	}

	template<typename InputIt>
	CCDK_FORCEINLINE void allocate_copy(InputIt beginIt, size_type n) {
		if (n == 0) return;
		ccdk_increase_allocate3(n, content, cap, len);
		util::construct_copy_n( heap_part, beginIt, n);
		content = heap_part;
	}

	template<typename InputIt>
	CCDK_FORCEINLINE void assign_copy(InputIt beginIt, size_type n) {
		destruct_content();
		if (n <= cap) {
			util::construct_copy_n(content, beginIt, n);
		}
		else {

		}
	}

	template<typename InputIt>
	CCDK_FORCEINLINE void initialize_copy(InputIt beginIt, size_type n) {
		if (n == 0) return;
		else if (n > InitSize) allocate_copy(beginIt, n);
		else {
			content = local_part.address();
			util::construct_copy_n(content, beginIt, n);
		}
	}
};

ccdk_namespace_ct_end
