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

	// destruct holded objects and release allocated memory if any.
	~local_vector() noexcept {
		destruct_content();
		deallocate();
		content = nullptr;
		cap = InitSize;
		len = 0;
	}

	/* default ctor */
	CCDK_FORCEINLINE local_vector() = default;
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
		else if (!heap_part && !other.heap_part) {
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

	// destroy holded objects, but not release memory
	CCDK_FORCEINLINE this_type& operator=(ptr::nullptr_t) noexcept {
		this->~local_vector();
	}

	CCDK_FORCEINLINE this_type& operator=(local_vector const& other) {
		assign_copy(other.begin(), other.size());
		return *this;
	}

	template<size_type N2, typename Ratio2, typename Alloc2>
	CCDK_FORCEINLINE this_type& operator=(local_vector<N2, T, Ratio2, Alloc2> const& other) {
		assign_copy(other.begin(), other.size());
		return *this;
	}

	CCDK_FORCEINLINE this_type& operator=(local_vector && other) {
		destruct_content();
		deallocate();
		if (other.heap_part) {
			heap_part = other.heap_part;
			content = heap_part;
		}
		else {
			local_part = other.local_part;
			content = local_part.address();
		}
		len = other.len;
		cap = other.cap;
		other.rvalue_reset();
		return *this;
	}

	template<size_type N2, typename Ratio2>
	CCDK_FORCEINLINE this_type& operator=(local_vector<N2, T, Ratio2, Alloc> && other) {
		destruct_content();
		deallocate();
		if (other.heap_part) {
			heap_part = other.heap_part;
			content = heap_part;
		}
		else {
			local_part = other.local_part;
			content = local_part.address();
		}
		len = other.len;
		cap = other.cap;
		other.rvalue_reset();
		return *this;
	}

	template<typename... Args>
	CCDK_FORCEINLINE this_type& assign(size_type n, Args && ... args) {
		return assign_fill(n, util::forward<Args>(args)...);
	}

	template<InputIt>
	CCDK_FORCEINLINE this_type& assign(InputIt beginIt, size_type n) {
		return assign_copy(beginIt, n);
	}

	template<InputIt>
	CCDK_FORCEINLINE this_type& assign(InputIt beginIt, InputIt endIt) {
		return assign_copy(beginIt, alg::distance(beginIt, endIt));
	}

	//attr
	CCDK_FORCEINLINE size_type size() const noexcept { return len; }
	CCDK_FORCEINLINE size_type capcity() const noexcept { return cap; }
	CCDK_FORCEINLINE bool  empty() const noexcept { return len == 0; }
	CCDK_FORCEINLINE constexpr size_type max_size() const noexcept { return kMaxSize; }

	//access
	CCDK_FORCEINLINE pointer data() noexcept { return content; }
	CCDK_FORCEINLINE const_pointer data() const noexcept { return content; }
	CCDK_FORCEINLINE reference operator[](size_type idx) noexcept { return content[idx]; }
	CCDK_FORCEINLINE const_reference operator[](size_type idx) const noexcept { return content[idx]; }
	CCDK_FORCEINLINE reference at(size_type idx) noexcept { return content[idx]; }
	CCDK_FORCEINLINE const_reference at(size_type idx) const noexcept { return content[idx]; }
	CCDK_FORCEINLINE reference front()() noexcept { return content[0]; }
	CCDK_FORCEINLINE reference front() const noexcept { return content[0]; }

	//iterator
	CCDK_FORCEINLINE iterator begin() noexcept { return content; }
	CCDK_FORCEINLINE iterator end() noexcept { return content + len; }
	CCDK_FORCEINLINE const_iterator cbegin() const noexcept { return content; }
	CCDK_FORCEINLINE const_iterator cend() const noexcept { return content + len; }
	CCDK_FORCEINLINE reverse_iterator rbegin() noexcept { return { content +len-1 }; }
	CCDK_FORCEINLINE reverse_iterator rend() noexcept { return {content-1}; }
	CCDK_FORCEINLINE const_reverse_iterator crbegin() const noexcept { return { content + len - 1 }; }
	CCDK_FORCEINLINE const_reverse_iterator crend() const noexcept { return { content - 1 }; }


	//  pop back element, destruct it 
	CCDK_FORCEINLINE this_type& pop_back() noexcept {
		if (len > 0) { util::destruct<T>(content + --len); }
		return *this;
	}

	/* inplace construct with P, Args... */
	template<
		typename ... Args,
		typename = check_t< has_constructor< T, Args...>>
	>
	CCDK_FORCEINLINE this_type& emplace_back(Args&& ... args) {
		if (len == cap) { local_reallocate(); }
		util::construct<T>(content + len++, util::forward<Args>(args)...);
		return *this;
	}

	CCDK_FORCEINLINE this_type& push_back(T const& t) {
		return emplace_back(t);
	}

	CCDK_FORCEINLINE this_type& push_back(T && t) {
		return emplace_back(util::move(t));
	}

	/* inplace-construct at pos( [0,len] ) */
	template<
		typename... Args,
		typename = check_t< has_constructor<T, Args...>>
	>
	CCDK_FORCEINLINE this_type& emplace(size_type pos, Args&&... args) {
		ccdk_assert(pos <= len);
		if (pos <= len) { 
			emplace_impl(pos, pos + 1, util::forward<Args>(args)...); 
		}
		return *this;
	}


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
	CCDK_FORCEINLINE void local_allocate_n(size_type n) {
		if (n == 0) return;
		ccdk_increase_allocate3(n, heap_part, cap, len);
		content = heap_part;
	}

	template<typename InputIt>
	CCDK_FORCEINLINE void allocate_copy(InputIt beginIt, size_type n) {
		local_allocate_n(n);
		util::construct_copy_n(content, beginIt, n);
	}

	CCDK_FORCEINLINE void reallocate_if_need(size_type n) {
		if (n > cap) {
			deallocate();
			local_allocate_n(n);
		}
	}

	CCDK_FORCEINLINE void local_reallocate() {
		pointer memory;
		size_type new_cap, new_len;
		ccdk_increase_allocate3(len, memory, new_cap, new_len);
		util::construct_move_n(memory, content, len);
		util::destruct_n(content, len, n);
		heap_part = memory;
		len = new_len;
		cap = new_cap;
		content = heap_part;
	}

	template<typename InputIt>
	CCDK_FORCEINLINE this_type& assign_copy(InputIt beginIt, size_type n) {
		if (n > 0) {
			destruct_content();
			reallocate_if_need(n);
			util::construct_copy_n(content, beginIt, n);
		}
		return *this;
	}

	template<typename ...Args>
	CCDK_FORCEINLINE this_type& assign_fill(size_type n, Args&&... args) {
		if (n > 0) {
			destruct_content();
			reallocate_if_need(n);
			util::construct_n(content, n, util::forward<Args>(args)...);
		}
		return *this;
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

	/*
		split copy [content, content+pos) to [content,content+pos),
		[content+pos, content+len) to [memory+pos+n, memory+len+n)
	*/
	CCDK_FORCEINLINE void split_move(T* memory, size_type start, size_type end) noexcept {
		util::construct_move_n(memory, content, pos);
		util::construct_move_n(memory + end, content + start, len - start);
	}

	template<typename... Args>
	CCDK_FORCEINLINE void emplace_impl(size_type start, size_type end, Args&& ... args) {
		size_type n = end - start;
		size_type new_len = len + n;
		if (new_len > cap) {
			ccdk_increase_allocate2(new_len, pointer memory, size_type new_cap);
			ccdk_safe_cleanup_if_exception(
				util::construct_n(memory + start, n, util::forward<Args>(args)...), /* may throw */
				allocate_type::deallocate(*this, memory, new_cap)
			);
			split_move(memory, start, end);
			util::destruct_n(content, len, n);
			heap_part = memory;
			content = memory;
			cap = new_cap;
		}
		else {
			size_type max_end = fn::max(end, len);
			util::construct_move_n(content + max_end, content + max_end - n, n);
			if (end<len) util::move_n(content + end, content + start, len - end);
			util::destruct_n(content + start, fn::min(end, len) - start);
			util::construct_copy_n(content + start, begin, n);
		}
		len = new_len;
		return *this;
	}

};

ccdk_namespace_ct_end
