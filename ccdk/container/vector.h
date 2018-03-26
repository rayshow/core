#pragma once


#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/mcontainer/val_pack.h>
#include<ccdk/mpl/function/operator.h>
#include<ccdk/mpl/iterator/ptr_iterator.h>
#include<ccdk/mpl/fusion/pair.h>
#include<ccdk/mpl/util/fill.h>
#include<ccdk/mpl/util/move.h>
#include<ccdk/mpl/util/copy.h>
#include<ccdk/mpl/util/construct.h>
#include<ccdk/mpl/units/ratio.h>
#include<ccdk/container/filter/filter_view.h>
#include<ccdk/memory/simple_new_allocator.h>
#include<ccdk/memory/allocator_traits.h>
#include<ccdk/algorithm/distance.h>
#include<ccdk/container/container_mudule.h>

ccdk_namespace_ct_start

using namespace ccdk::mpl;

template<
	typename T,
	typename InceaseRatio = units::ratio<15, 10>,             /* 1.5X incease ratio*/
	typename Size = uint32,
	typename Alloc = mem::simple_new_allocator<T>
>
class vector : protected Alloc
{
public:
	typedef vector		 this_type;
	typedef T			 value_type;
	typedef Size		 size_type;
	typedef ptr::diff_t  different_type;
	typedef InceaseRatio increase_ratio;
	typedef mem::allocator_traits<Alloc> allocate_type;

	/* iterator */
	typedef T*                         iterator_type;
	typedef T const*                   const_iterator_type;
	typedef reverse_iterator<T*>       reverse_iterator_type;
	typedef reverse_iterator<const T*> const_reverse_iterator_type;
	
	template<typename T2, typename Ratio2, typename Size2, typename Alloc2>
	friend class vector;

private:
	T *content;
	size_type len;
	size_type cap;

public:
	/* destruct */
	CCDK_FORCEINLINE ~vector() noexcept {
		if (content) {
			if (len > 0)  util::destruct_n(content, len);
			allocate_type::deallocate(*this, content, cap);
		}
	}

	/* default */
	CCDK_FORCEINLINE constexpr vector()
		:content{ nullptr }, len{ 0 }, cap{ 0 } {}

	/* fill*/
	CCDK_FORCEINLINE explicit vector(size_type n, T const& t = T()){
		ccdk_assert(n > 0);
		allocate_fill(n, t);
	}

	/* copy range-n */
	template<
		typename InputIt,
		typename = check_t< is_iterator<InputIt>>
	>
	CCDK_FORCEINLINE vector(InputIt begin, ptr::size_t n){
		ccdk_assert(n > 0);
		allocate_copy(n, begin);
	}

	/* copy range */
	template<
		typename InputIt,
		typename = check_t< is_iterator<InputIt>> 
	>
	CCDK_FORCEINLINE vector(InputIt begin, InputIt end){
		ptr::size_t n = alg::distance(begin, end);
		ccdk_assert(n > 0);
		allocate_copy(n, begin);
	}

	/* copy */
	CCDK_FORCEINLINE vector(vector const& other) {
		allocate_copy(other.len, other.content);
	}

	/* template copy */
	template<typename Size2, typename Alloc2, typename Ratio2 >
	CCDK_FORCEINLINE explicit vector(vector<T, Ratio2, Size2, Alloc2> const& other) {
		allocate_copy(other.len, other.content);
	}

	/* move */
	CCDK_FORCEINLINE vector(vector && other) noexcept
		:content{ other.content }, len{ other.len }, cap{ other.cap } {
		other.rvalue_reset();
	}

	/* template move */
	template<typename Size2, typename Alloc2, typename Ratio2>
	CCDK_FORCEINLINE vector(vector<T, Ratio2, Size2, Alloc2> && other) noexcept
		: content{ other.content }, len{ other.len }, cap{ other.cap } {
		other.rvalue_reset();
	}

	/* initializer list */
	CCDK_FORCEINLINE vector(std::initializer_list<T> const& lst) noexcept {
		allocate_copy(lst.size(), lst.begin());
	}

	/* swap */
	CCDK_FORCEINLINE void swap(vector& other) noexcept {
		util::swap(content, other.content);
		util::swap(len, other.len);
		util::swap(cap, other.cap);
	}

	/* assign  nullptr, first destruct objects */
	CCDK_FORCEINLINE this_type& operator=(ptr::nullptr_t) noexcept { 
		destruct_content();
		return *this;
	}

	/* copy assign */
	CCDK_FORCEINLINE this_type& operator=(vector const& other) {
		ccdk_if_not_this(other) { copy_or_allocate_copy(other.content, other.len); }	
		return *this;
	}

	/* template copy assign */
	template<typename Size2, typename Alloc2, typename Ratio2>
	CCDK_FORCEINLINE this_type& operator=(vector<T, Ratio2, Size2, Alloc2> const& other) {
		copy_or_allocate_copy(other.content, other.len);
		return *this;
	}

	/* move assign */
	CCDK_FORCEINLINE this_type& operator=(vector && other) {
		ccdk_if_not_this(other) { vector{ util::move(other) }.swap(*this); }
		return *this;
	}

	/* template move assign */
	template<typename Size2, typename Alloc2, typename Ratio2>
	CCDK_FORCEINLINE this_type& operator=(vector<T, Ratio2, Size2, Alloc2> && other) {
		vector{ util::move(other) }.swap(*this);
		return *this;
	}

	/* assign nullptr */
	CCDK_FORCEINLINE this_type& assign(ptr::nullptr_t) noexcept { 
		return (*this = nullptr);
	}

	/* fill with nxt */
	CCDK_FORCEINLINE this_type& assign(size_type n, T const& t = T() ) {
		fill_or_allocate_fill(t, n);
		return *this;
	}

	/* copy n*/
	template<typename InputIt,
		typename = check_t< is_iterator<InputIt>>
	>
	CCDK_FORCEINLINE this_type& assign(InputIt begin, ptr::size_t n){
		copy_or_allocate_copy(begin, n);
		return *this;
	}

	/* copy range */
	template<typename InputIt,
		typename = check_t< is_iterator<InputIt>>
	>
	CCDK_FORCEINLINE this_type& assign(InputIt begin, InputIt end){
		return assign(begin, alg::distance(begin, end));
	}

	/* initialize_list */
	CCDK_FORCEINLINE this_type& assign(std::initializer_list<T> const& lst){
		return assign(lst.begin(), lst.size());
	}

	/* attribute*/
	CCDK_FORCEINLINE size_type size() const noexcept { return len; }
	CCDK_FORCEINLINE size_type capacity() const noexcept { return cap; }
	CCDK_FORCEINLINE bool empty() const noexcept { return len == 0; }
	CCDK_FORCEINLINE size_type max_size() const noexcept { 
		return allocate_type::max_allocate_size();
	}
	

	/* iterator */
	CCDK_FORCEINLINE iterator_type begin() noexcept{ 
		return content;
	}
	CCDK_FORCEINLINE const_iterator_type cbegin() const noexcept {
		return content;
	}
	CCDK_FORCEINLINE reverse_iterator_type rbegin() noexcept {
		ccdk_assert(content);  
		return {content+len-1};
	}
	CCDK_FORCEINLINE const_reverse_iterator_type crbegin() const noexcept {
		ccdk_assert(content);  
		return { content + len-1 };
	}
	CCDK_FORCEINLINE iterator_type end() noexcept {
		return content + len ;
	}
	CCDK_FORCEINLINE const_iterator_type cend() const noexcept {
		return content + len;
	}

	CCDK_FORCEINLINE reverse_iterator_type rend() noexcept {
		ccdk_assert(content&& len > 0); 
		return { content -1 };
	}

	CCDK_FORCEINLINE const_reverse_iterator_type crend() const noexcept {
		ccdk_assert(content&& len > 0);  
		return { content-1 };
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
		if (len > 0) { util::destruct<T>(content + --len); }
		return *this;
	}

	/* inplace construct with P, Args... */
	template<
		typename P, typename ... Args,
		typename  = check_t< has_constructor< T, P, Args...>>
	>
	CCDK_FORCEINLINE this_type& emplace_back(P&& p, Args&& ... args){
		if (len == cap) { reallocate_move(); }
		util::construct<T>(content + len++, util::forward<P>(p),
			util::forward<Args>(args)...);
		return *this;
	}

	/* copy-construct T into back */
	CCDK_FORCEINLINE this_type& push_back(T const& t)  { return emplace_back(t); }

	/* move-contruct T into back */
	CCDK_FORCEINLINE this_type& push_back(T&& t) { return emplace_back(util::move(t)); }

	/* copy-construct [begin, begin+n) to back */
	template<typename InputIt>
	CCDK_FORCEINLINE this_type& push_back(InputIt begin, ptr::size_t n) {
		return insert_impl(len, len + n, begin);
	}

	/* copy-construct [begin, begin+n) to back */
	template<typename InputIt>
	CCDK_FORCEINLINE this_type& push_back(InputIt begin,InputIt end) {
		return push_back(begin, alg::distance(begin, end));
	}

	/* copy initialize_list to back */
	CCDK_FORCEINLINE this_type& push_back(std::initializer_list<T> const& lst) {
		return push_back(lst.begin(), lst.size());
	}

	/* move-range-back */
	template<typename InputIt>
	CCDK_FORCEINLINE this_type& move_back(InputIt begin, InputIt end) {
		return *this;
	}

	/* inplace-construct at pos( [0,len] ) */
	template< 
		typename P, typename... Args,
		typename = check_t< has_constructor<T, P, Args...>>
	>
	CCDK_FORCEINLINE this_type& emplace(size_type pos, P&& p, Args&&... args) {
		ccdk_assert(pos <= len);
		if (pos <= len) { emplace_impl(pos, pos + 1, util::forward<P>(p), util::forward<Args>(args)...); }
		return *this;
	}

	/* inplace-construct at pos( [0,len] ) */
	template<
		typename Iterator,
		typename P,  typename... Args,
		typename = check_t<is_pointer_iterator<T, Iterator> >, // exclude comflict with size_type
		typename = check_t< has_constructor<T, P, Args...>>
	>
	CCDK_FORCEINLINE this_type& emplace(Iterator it, P&& p, Args&&... args) {
		return emplace(it - content, util::forward<P>(p), util::forward<Args>(args)...);
	}

	/* insert, pos copy */
	CCDK_FORCEINLINE this_type& insert(size_type pos, T const& t) { return emplace(pos, t);  }

	/* insert, iterator copy*/
	template<
		typename Iterator,
		typename = check_t<is_pointer_iterator<T, Iterator> > // exclude comflict with size_type
	>
	CCDK_FORCEINLINE this_type& insert(Iterator it, T const& t){  return emplace(it - content, t); }

	/* insert, pos move */
	CCDK_FORCEINLINE this_type& insert(size_type pos, T&& t) { return emplace(pos, util::move(t)); }

	/* insert, iterator move*/
	template<
		typename Iterator, 
		typename = check_t<is_pointer_iterator<T,Iterator> > // exclude comflict with size_type
	>
	CCDK_FORCEINLINE this_type& insert(Iterator it, T&& t) { return emplace(it - content, util::move(t)); }

	/* insert, pos item-count*/
	template<
		typename InputIt,
		typename = check_t< has_constructor<T, iterator_value_t<InputIt>>> 
	>
	CCDK_FORCEINLINE this_type& insert(size_type pos, InputIt begin, size_type n){
		return insert_impl(pos, pos + n, begin);
	}

	/* insert, iterator item-count*/
	template<
		typename Iterator,
		typename InputIt,
		typename = check_t<is_pointer_iterator<T, Iterator> >, // exclude comflict with size_type
		typename = check_t< has_constructor<T, iterator_value_t<InputIt>>> 
	>
	CCDK_FORCEINLINE this_type& insert(Iterator it, InputIt begin, size_type n) {
		return insert_impl(it -content, it - content + n, begin);
	}

	/* insert, pos range */
	template<
		typename InputIt, 
		typename = check_t< has_constructor<T, iterator_value_t<InputIt>>> 
	>
	CCDK_FORCEINLINE this_type& insert(size_type pos, InputIt begin, InputIt end){
		return insert(pos, begin, alg::distance(begin, end));
	}

	/* insert, iterator range */
	template<
		typename Iterator,
		typename InputIt,
		typename = check_t<is_pointer_iterator<T, Iterator> >, // exclude comflict with size_type
		typename = check_t< has_constructor<T, iterator_value_t<InputIt>>>
	>
		CCDK_FORCEINLINE this_type& insert(Iterator it, InputIt begin, InputIt end) {
		return insert( it - content, begin, alg::distance(begin, end));
	}

	/* insert, pos initialize_list */
	CCDK_FORCEINLINE this_type& insert(size_type pos, std::initializer_list<T> const& lst)
	{
		return insert(pos, lst.begin(), lst.size());
	}

	/* insert, iterator initialize_list */
	template<
		typename Iterator,
		typename = check_t<is_pointer_iterator<T, Iterator> > // exclude comflict with size_type
	>
	CCDK_FORCEINLINE this_type& insert(Iterator it, std::initializer_list<T> const& lst){
		return insert(it - content, lst.begin(), lst.size());
	}

	/* erase */
	CCDK_FORCEINLINE this_type& erase(size_type start, size_type end) {
		ccdk_assert(start < end && end <= len);
		size_type n = end - start;
		if (n>0 && end <= len) {
			util::move_n(content + start, content + end, n);
			util::destruct_n(content + end, len - end);
			len -= n;
		}
		return *this;
	}

	/* erase [start, len)*/
	CCDK_FORCEINLINE this_type& erase(size_type start) { return erase(start, len); }

	/* erase iterator [begin, end())*/
	template<
		typename Iterator,
		typename = check_t<is_pointer_iterator<T, Iterator> > // exclude comflict with size_type
	>
	CCDK_FORCEINLINE this_type& erase(Iterator begin) { return erase(begin-content, len); }

	/* erase iterator [begin, end())*/
	template<
		typename Iterator,
		typename = check_t<is_pointer_iterator<T, Iterator> > // exclude comflict with size_type
	>
	CCDK_FORCEINLINE this_type& erase(Iterator begin, Iterator end) {
		return erase(begin - content, end-content); 
	}

	/* clear, destruct all objects */
	CCDK_FORCEINLINE this_type& clear(){
		util::destruct_n(content, len);
		len = 0;
		return *this;
	}

private:
	/* reset but not destruct memory */
	CCDK_FORCEINLINE void rvalue_reset() noexcept {
		content = nullptr; len = 0;  cap = 0;
	}

	/* if len > 0 , destruct objects */
	CCDK_FORCEINLINE void destruct_content() noexcept {
		if (len > 0) { util::destruct_n(content, len); len = 0; }
	}

	/* copy from [begin, begin+n) to content[0, n), destruct old objects  */
	template<typename InputIt>
	CCDK_FORCEINLINE void range_copy(InputIt begin, size_type n) {
		destruct_content();
		util::construct_copy_n(content, begin, n);
		len = n;
	}

	/* copy from [begin, begin+n) to content[0, n), destruct old objects  */
	CCDK_FORCEINLINE void range_fill(T const& t, size_type n) {
		destruct_content();
		util::construct_fill_n(content, t, n);
		len = n;
	}

	/*
		if content big enough to hold n elements, just copy [begin, begin+n) to it
		else allocate new big enough memory and swap with it
	*/
	template<typename InputIt>
	CCDK_FORCEINLINE void copy_or_allocate_copy(InputIt begin, size_type n) {
		if (n <= cap) range_copy(begin, n);
		else vector{ begin, n }.swap(*this);
	}

	/*
		if content big enough to hold n elements, just fill it
		else allocate new big enough memory and swap with it
	*/
	CCDK_FORCEINLINE void fill_or_allocate_fill(T const& t, size_type n) {
		if (n <= cap) range_fill(t, n); 
		else vector{ n,t }.swap(*this); 
	}

	/*
		allocate n*increase size memory to content,
		then fill [begin, begin+n) with v
	*/
	CCDK_FORCEINLINE void allocate_fill(size_type n, T const& v ) {
		if (n == 0) return;
		ccdk_increase_allocate3(n,content, cap, len);
		util::construct_fill_n(content, v, n);
	}

	/*
		allocate n*increase size memory to content, 
		then copy[begin, begin+n) to [content, content+n)
	*/
	template<typename InputIt>
	CCDK_FORCEINLINE void allocate_copy(size_type n, InputIt begin){
		if (n == 0) return;
		ccdk_increase_allocate3(n, content, cap, len);
		util::construct_copy_n(content, begin, n);
	}


	/*  
		split copy [content, content+pos) to [content,content+pos),
		[content+pos, content+len) to [memory+pos+n, memory+len+n)
	*/
	CCDK_FORCEINLINE void split_copy(T* memory, size_type pos, size_type n) noexcept{
		util::construct_move_n(memory, content, pos);
		util::construct_move_n(memory + pos + n, content + pos, len - pos);
	}

	/*
		allocate a new memory, if success 
		move [content, content+n) to this [memory, memory+n), free old content
	*/
	void reallocate_move() {
		ccdk_increase_allocate2(len, T* memory, size_type new_cap);
		util::construct_move_n(memory, content, len);
		util::destruct_n(content, len);
		allocate_type::deallocate(*this, content, cap);
		content = memory;
		cap = new_cap;
	}

	/* 
		if capacity is not big enough, realloc big enough memory to 
		hold old data and insert data, [memory+start, memory+end) stay
		empty for insert data.
		else move [content+start, content+len) backward.
	*/
	template<typename InputIt>
	this_type& insert_impl(size_type start, size_type end, InputIt begin) {
		ccdk_assert(end > start && start <= len);
		if (end <= start || start > len) return *this;
		size_type n = end - start;
		size_type new_len = len + n;
		if (new_len > cap) {
			ccdk_increase_allocate2(new_len, T* memory, size_type new_cap);
			ccdk_safe_cleanup_if_exception(
				util::construct_copy_n(memory + start, begin, n), /* may throw */
				allocate_type::deallocate(*this,memory,new_cap)
			);
			split_copy(memory, start, n);
			allocate_type::deallocate(*this, content,cap);
			content = memory;
			cap = new_cap;
		} else {
			size_type max_end = fn::max(end, len);
			util::construct_move_n(content + max_end, content + max_end - n, n);
			if (end<len) util::move_n(content + end, content + start, len - end);
			util::destruct_n(content + start, fn::min(end, len) - start);
			util::construct_copy_n(content + start, begin, n);
		}
		len = new_len;
		return *this;
	}

	/* 
		if capacity is not enough, realloc big enough memory to hold emplace construct data,
		memory[start, end) stay empty for external data to copy in.
		else move content[start, len) backward
	*/
	template<typename... Args>
	void emplace_impl(size_type start, size_type end, Args&&... args) {
		if (end <= start || start > len) return;
		size_type n = end - start;
		size_type new_len = len + n;
		if (new_len > cap){
			ccdk_increase_allocate2(new_len, T* memory, size_type new_cap);
			ccdk_safe_cleanup_if_exception(
				util::construct_n<T>(memory + start, n, util::forward<Args>(args)...),
				allocate_type::deallocate(*this, memory, new_cap)
			);
			split_copy(memory, start, n);
			allocate_type::deallocate(*this, content, cap);
			content = memory;
			cap = new_cap;
		} else {
			size_type max_end = fn::max(end, len);
			util::construct_move_n(content + max_end, content + max_end - n, n);
			if (end<len) util::move_n(content + end, content + start, len - end);
			util::destruct_n(content + start, fn::min(end, len) - start);
			util::construct_n<T>(content + start, n, util::forward<Args>(args)...);
		}
		len = new_len;
	}
};


ccdk_namespace_ct_end