#pragma once

#include<ccdk/container/container_mudule.h>
#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/mcontainer/val_pack.h>
#include<ccdk/mpl/function/operator.h>
#include<ccdk/mpl/iterator/ptr_iterator.h>
#include<ccdk/mpl/util/fill.h>
#include<ccdk/mpl/util/move.h>
#include<ccdk/mpl/util/copy.h>
#include<ccdk/mpl/util/construct.h>
#include<ccdk/mpl/units/ratio.h>
#include<ccdk/container/filter/filter_view.h>
#include<ccdk/memory/simple_new_allocator.h>
#include<ccdk/algorithm/distance.h>

ccdk_namespace_ct_start

using namespace ccdk::mpl;

template<
	typename T,
	typename Size = uint32,                              
	typename Alloc = mem::simple_new_allocator<T>,        
	typename InceaseRatio = units::ratio<2,1>             /* 2.0X incease ratio*/
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

	/* iterator */
	typedef T*                         iterator_type;
	typedef T const*                   const_iterator_type;
	typedef reverse_iterator<T*>       reverse_iterator_type;
	typedef reverse_iterator<const T*> const_reverse_iterator_type;
	
	template<typename T, typename Size2, typename Alloc2, typename Ratio2>
	friend class vector;

private:
	T *content;
	size_type len;
	size_type cap;

public:
	/* default */
	CCDK_FORCEINLINE constexpr vector()
		:content{ nullptr }, len{ 0 }, cap{ 0 } {}

	/* fill*/
	CCDK_FORCEINLINE explicit vector(size_type n, T const& t = T())
	{
		ccdk_assert(n > 0);
		if(n>0) allocate_fill(n, t);
	}

	/* copy range */
	template<typename InputIt>
	CCDK_FORCEINLINE explicit vector(InputIt begin, InputIt end)
	{
		ptr::size_t n = alg::distance(begin, end);
		ccdk_assert(n > 0);
		allocate_copy(n, begin);
	}

	/* copy */
	CCDK_FORCEINLINE vector(vector const& other) {
		if (other.len > 0) allocate_copy(other.len, other.content);
	}

	/* template copy */
	template<typename Size2, typename Alloc2, typename Ratio2>
	CCDK_FORCEINLINE vector(vector<T, Size2, Alloc2, Ratio2> const& other) {
		if (other.len > 0) allocate_copy(other.len, other.content);
	}

	/* move */
	CCDK_FORCEINLINE vector(vector && other) noexcept
		:content{ other.content }, len{ other.len }, cap{ other.cap } {
		other.rvalue_reset();
	}

	/* template move */
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

	/* assign  nullptr, first destruct objects */
	CCDK_FORCEINLINE operator=(ptr::nullptr_t) noexcept { 
		util::destruct_n(content, len);
		len = 0; 
	}

	/* copy assign */
	CCDK_FORCEINLINE operator=(vector const& other) {
		ccdk_if_not_this(other) { 
			if ( other.len <= cap) range_copy(other.content, other.len);
			else  vector{ other }.swap(*this);
		}	
	}

	/* template copy assign */
	template<typename Size2, typename Alloc2, typename Ratio2>
	CCDK_FORCEINLINE operator=(vector<T, Size2, Alloc2, Ratio2> const& other) {
		if (other.len <= cap) range_copy(other.content, other.len);
		else  vector{ other }.swap(*this);
	}

	/* move assign */
	CCDK_FORCEINLINE operator=(vector && other) {
		ccdk_if_not_this(other) { vector{ util::move(other) }.swap(*this); }
	}

	/* template move assign */
	template<typename Size2, typename Alloc2, typename Ratio2>
	CCDK_FORCEINLINE operator=(vector<T, Size2, Alloc2, Ratio2> && other) {
		ccdk_if_not_this(other) { vector{ util::move(other) }.swap(*this); }
	}

	/* assign nullptr */
	CCDK_FORCEINLINE this_type& assign(ptr::nullptr_t) noexcept { 
		util::destruct_n(content, len);  len = 0; 
		return *this;
	}

	/* fill with nxt */
	CCDK_FORCEINLINE this_type& assign(size_type n, T const& t = T() ) {
		if (n <= cap) {
			util::destruct_n(content, len);
			util::construct_fill_n(content, t, n);
		}
		else { vector{ n,t }.swap(*this);  }
		return *this;
	}

	/* copy n*/
	CCDK_FORCEINLINE this_type& assign(InputIt begin, ptr::size_t n)
	{
		if (n <= cap) {
			util::destruct_n(content, len);
			util::construct_copy_n(content, begin, n);
		}
		else { vector{ begin, end }.swap(*this); }
		return *this;
	}

	/* copy range */
	CCDK_FORCEINLINE this_type& assign(InputIt begin, InputIt end)
	{
		return assign(begin, alg::distance(begin, end));
	}

	/* initialize_list */
	CCDK_FORCEINLINE this_type& assign(std::initializer_list<T> const& lst)
	{
		assign(lst.begin(), lst.size());
	}

	/* attribute*/
	CCDK_FORCEINLINE size_type size() const noexcept { return len; }
	CCDK_FORCEINLINE size_type max_size() const noexcept { return size_type(-1); }
	CCDK_FORCEINLINE size_type capacity() const noexcept { return cap; }
	CCDK_FORCEINLINE bool empty() const noexcept { return len == 0; }

	/* iterator */
	CCDK_FORCEINLINE iterator_type begin() noexcept{ 
		ccdk_assert(content);  return content; 
	}
	CCDK_FORCEINLINE const_iterator_type cbegin() const noexcept {
		ccdk_assert(content);  return content;
	}
	CCDK_FORCEINLINE reverse_iterator_type rbegin() noexcept {
		ccdk_assert(content);  
		return reverse_iterator_type{content+len};
	}
	CCDK_FORCEINLINE const_reverse_iterator_type crbegin() const noexcept {
		ccdk_assert(content);  
		return const_reverse_iterator_type{ content + len };
	}
	CCDK_FORCEINLINE iterator_type end() noexcept {
		ccdk_assert(content && len>0); 
		return content+len;
	}
	CCDK_FORCEINLINE const_iterator_type cend() const noexcept {
		ccdk_assert(content && len > 0); 
		return content + len;
	}
	CCDK_FORCEINLINE reverse_iterator_type rend() noexcept {
		ccdk_assert(content&& len > 0); 
		return reverse_iterator_type{ content -1 };
	}
	CCDK_FORCEINLINE const_reverse_iterator_type crend() const noexcept {
		ccdk_assert(content&& len > 0);  
		return const_reverse_iterator_type{ content-1 };
	}

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

	/*  pop back element, destruct it  */
	CCDK_FORCEINLINE this_type& pop_back() noexcept { 
		if (len > 0) { util::destruct(content + --len); }
		return *this;
	}

	/* inplace construct with P, Args... */
	template<typename P, typename ... Args>
	CCDK_FORCEINLINE this_type& emplace_back(P&& p, Args&& ... args)
	{
		if (len == cap) { realloc_move(len); }
		util::construct<T>(content + len++, util::forward<P>(p),
			util::forward<Args>(args)...);
		return *this;
	}

	/* copy-construct T into back */
	CCDK_FORCEINLINE this_type& push_back(T const& t)  { return emplace_back(t); }

	/* move-contruct T into back */
	CCDK_FORCEINLINE this_type& push_back(T&& t) { return emplace_back(util::move(t)); }

	/* copy-construct [begin, begin+n) to back */
	template<InputIt>
	CCDK_FORCEINLINE this_type& push_back(InputIt begin, ptr::size_t n) {
		return insert_impl(len, len + n, begin);
	}

	/* copy-construct [begin, begin+n) to back */
	template<InputIt>
	CCDK_FORCEINLINE this_type& push_back(InputIt begin,InputIt end) {
		return push_back(begin, alg::distance(begin, end));
	}

	/* copy initialize_list to back */
	CCDK_FORCEINLINE this_type& push_back(std::initializer_list<T> const& lst) {
		return push_back(lst.begin(), lst.size());
	}

	/* move-range-back */
	CCDK_FORCEINLINE this_type& move_back(InputIt begin, InputIt end) {
		return *this;
	}

	/* inplace-construct at pos( [0,len] ) */
	template< typename T, typename... Args>
	CCDK_FORCEINLINE this_type& emplace(size_type pos, T&& t, Args&&... args)
	{
		ccdk_assert(pos < len);
		if (pos <= len) { emplace_impl(pos, util::forward<Args>(args)...); }
	}

	/* insert */
	CCDK_FORCEINLINE this_type& insert(size_type pos, T const& t) { return emplace(pos, t); }

	CCDK_FORCEINLINE this_type& insert(size_type pos, T&& t) { return emplace(pos, util::move(t)); }

	template<
		typename InputIt,
		typename = check_t< has_constructor<T, iterator_value_t<InputIt>>> >
	CCDK_FORCEINLINE this_type& insert(size_type pos, InputIt begin, size_type n)
	{
		return insert_impl(pos, pos + n, begin);
	}

	template<
		typename InputIt, 
		typename = check_t< has_constructor<T, iterator_value_t<InputIt>>> >
	CCDK_FORCEINLINE this_type& insert(size_type pos, InputIt begin, InputIt end)
	{
		return insert(pos, begin, alg::distance(begin, end));
	}

	CCDK_FORCEINLINE this_type& insert(size_type pos, std::initializer_list<T> const& lst)
	{
		return insert(pos, lst.begin(), lst.size());
	}

	/* erase */
	CCDK_FORCEINLINE this_type& erase(size_type start, size_type end)
	{
		ccdk_assert(start < end && end <= len);
		if (start < end && end <= len) {
			util::move_n(content + start, content + end, end - start);
			util::destruct_n(content + end, len - end);
			len -= end - start;
		}
		return *this;
	}

	/* clear, destruct all objects */
	CCDK_FORCEINLINE this_type& clear()
	{
		util::destruct(content, len);
		len = 0;
	}

	/* erase [begin, end) */
	

private:
	/* reset but not destruct memory */
	CCDK_FORCEINLINE void rvalue_reset() noexcept {
		content = nullptr; len = 0;  cap = 0;
	}

	/* copy from [begin, begin+n) to content[0, n), destruct old objects  */
	template<typename InputIt>
	CCDK_FORCEINLINE void range_copy(InputIt begin, size_type n) {
		util::destruct_n(content, len);
		util::construct_copy_n(content, begin, n);
		len = n;
	}

	/* pre-compute allocate size */
	CCDK_FORCEINLINE size_type precompute_cap(size_type n)
	{
		size_type actual_size = incease_ratio::multiply(n);
		if (n == size_type(-1) || actual_size < n) throw std::bad_alloc{};
	}

	/* alloc n elements */
	CCDK_FORCEINLINE T* allocate_cap(size_type n) {
		T* memory = allocate(n);
		if (!memory) throw std::bad_cast{}
		return memory;
	}

	template<typename InputIt>
	CCDK_FORCEINLINE void allocate_len(size_type n)
	{
		size_type capcity = precompute_cap(n);
		content = allocate_cap(capcity);
		len = n;
		cap = capcity;
	}

	template<typename InputIt>
	CCDK_FORCEINLINE void allocate_fill(size_type n, T const& v)
	{
		allocate_len(n);
		construct_fill_n(content, v, n);
	}


	template<typename InputIt>
	CCDK_FORCEINLINE void allocate_copy(size_type n, InputIt begin)
	{
		allocate_len(n);
		construct_copy_n(content, begin, n);
	}


	/*  split copy content[0, pos) to memory[0,pos),
		content[pos, len) to memory[pos+n, len+n)
	*/
	CCDK_FORCEINLINE void split_copy(T* memory, size_type pos, size_type n) noexcept
	{
		util::construct_move_n(memory, content, pos);
		util::construct_move_n(memory + end, content + start, len - start);
	}

	void realloc_move(size_type n) {
		size_type new_cap = prealloc_size(len);
		T* memory = alloc_cap(new_cap);
		util::construct_move_n(memory, content, len);
		util::destruct_n(content, len);
		deallocate(content);
		content = memory;
		cap = new_cap;
	}

	/* if capacity is not enough, realloc big enough memory to hold insert data,
		memory[start, end) stay empty for external data.
		else move content[start, len) backward
	*/
	template<typename InputIt>
	void insert_impl(size_type start, size_type end, InputIt begin)
	{
		if (end <= start || start > len) return;
		size_type n = end - start;
		size_type new_len = len + n;
		if (new_len > cap)
		{
			size_type new_cap = prealloc_size(new_len);
			T* memory = allocate_cap(new_cap);
			ccdk_safe_cleanup_if_exception(
				util::construct_copy_n(memory + start, begin, n), /* may throw */
				deallocate(memory)
			);
			split_copy(memory, start, n);
			deallocate(content);
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
	}

	/* if capacity is not enough, realloc big enough memory to hold emplace construct data,
	memory[start, end) stay empty for external data to copy in.
	else move content[start, len) backward
	*/
	template<typename... Args>
	void emplace_impl(size_type start, size_type end, Args&&... args)
	{
		if (end <= start || start > len) return;
		size_type n = end - start;
		size_type new_len = len + n;
		if (new_len > cap)
		{
			size_type new_cap = prealloc_size(new_len);
			T* memory = allocate_cap(new_cap);
			ccdk_safe_cleanup_if_exception(
				util::construct_n<T>(memory + start, add_len, util::forward<Args>(args)...),
				deallocate(memory)
			);
			split_copy(memory, start, n);
			deallocate(content);
			content = memory;
			cap = new_cap;
		}
		else {
			size_type max_end = fn::max(end, len);
			util::construct_move_n(content + max_end, content + max_end - n, n);
			if (end<len) util::move_n(content + end, content + start, len - end);
			util::destruct_n(content + start, fn::min(end, len) - start);
			util::construct_n(content + start, n, util::forward<Args>(args)...);
		}
		len = new_len;
	}
};


ccdk_namespace_ct_end