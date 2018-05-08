#pragma once

#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/mcontainer/val_pack.h>
#include<ccdk/mpl/function/operator.h>
#include<ccdk/mpl/iterator/ptr_iterator.h>
#include<ccdk/mpl/iterator/reverse_iterator.h>
#include<ccdk/mpl/iterator/algorithm/distance.h>
#include<ccdk/mpl/fusion/pair.h>
#include<ccdk/mpl/util/fill.h>
#include<ccdk/mpl/util/move.h>
#include<ccdk/mpl/util/copy.h>
#include<ccdk/mpl/util/construct.h>
#include<ccdk/mpl/units/ratio.h>
#include<ccdk/container/filter/range_filter.h>
#include<ccdk/memory/simple_new_allocator.h>
#include<ccdk/memory/allocator_traits.h>

#include<ccdk/container/container_mudule.h>

ccdk_namespace_ct_start

using namespace ccdk::mpl;

//TODO add nullptr-optimize
template<
	typename T,
	typename InceaseRatio = units::ratio<2, 1>,             /* 2X incease ratio*/
	typename Size = uint32,
	typename Alloc = mem::simple_new_allocator<T>
>
class vector : protected Alloc
{
public:
	using this_type      = vector;
	using increase_ratio = InceaseRatio;
	
	using value_type = T;
	using pointer = T * ;
	using const_pointer = T const*;
	using reference = T & ;
	using const_reference = T const&;
	using size_type = Size;
	using difference_type = ptr::diff_t;
	using allocator_type = mem::allocator_traits<Alloc>;

	/* iterator */
	using iterator               = T*;
	using const_iterator         = T const*;
	using reverse_iterator       = it::reverse_iterator<T*>;
	using const_reverse_iterator = it::reverse_iterator<const T*>;
	
	template<typename T2, typename Ratio2, typename Size2, typename Alloc2>
	friend class vector;
	constexpr static uint32 kLeastElements = 10;

private:
	
	T *       content;
	size_type len;
	size_type cap;

public:
	// destruct 
	CCDK_FORCEINLINE ~vector() noexcept {
		destruct_content();
		deallocate();
	}

	// default and nullptr ctor
	CCDK_FORCEINLINE constexpr vector()
		: content{ nullptr }, len{ 0 }, cap{ 0 } {}
	CCDK_FORCEINLINE constexpr vector(ptr::nullptr_t)
		: content{ nullptr }, len{ 0 }, cap{ 0 } {}


	// reserve memory but not fill
	CCDK_FORCEINLINE explicit vector(size_type n) : len{ 0 } {
		ccdk_assert(n > 0);
		allocate(n);
	}

	// fill with nullptr( actually memset it)
	CCDK_FORCEINLINE vector(size_type n, ptr::nullptr_t) : len{ n } {
		allocate(n);
		memset(content, 0, sizeof(T)*n);
	}

	// fill ctor
	CCDK_FORCEINLINE explicit vector(size_type n, T const& t){
		ccdk_assert(n > 0);
		allocate_fill(n, t);
	}

	// copy range-n ctor
	template<
		typename InputIt,
		typename = check_t< is_iterator<InputIt>>
	>
	CCDK_FORCEINLINE vector(InputIt begin, ptr::size_t n){
		ccdk_assert(n > 0);
		allocate_copy(n, begin);
	}

	// copy range ctor
	template< 
		typename InputIt, 
		typename = check_t< is_iterator<InputIt>>  
	>
	CCDK_FORCEINLINE vector(InputIt begin, InputIt end){
		ptr::size_t n = it::distance(begin, end);
		ccdk_assert(n > 0);
		allocate_copy(n, begin);
	}

	// copy ctor
	CCDK_FORCEINLINE vector(vector const& other) {
		allocate_copy(other.len, other.content);
	}

	// template copy ctor
	template<typename Size2, typename Alloc2, typename Ratio2 >
	CCDK_FORCEINLINE explicit vector(vector<T, Ratio2, Size2, Alloc2> const& other) {
		allocate_copy(other.len, other.content);
	}

	// move ctor
	CCDK_FORCEINLINE vector(vector && other) noexcept
		:content{ other.content }, len{ other.len }, cap{ other.cap } {
		other.rvalue_reset();
	}

	// template move ctor
	template<typename Size2, typename Alloc2, typename Ratio2>
	CCDK_FORCEINLINE vector(vector<T, Ratio2, Size2, Alloc2> && other) noexcept
		: content{ other.content }, len{ other.len }, cap{ other.cap } {
		other.rvalue_reset();
	}

	// array initialize, instead of initialize_list
	template<uint32 N>
	CCDK_FORCEINLINE vector(T const (&arr)[N] ) noexcept {
		allocate_copy(N, arr);
	}

	// swap 2 vector
	CCDK_FORCEINLINE void swap(vector& other) noexcept {
		util::swap(content, other.content);
		util::swap(len, other.len);
		util::swap(cap, other.cap);
	}

	// destruct all object and reset size, but keep memory and capcity
	CCDK_FORCEINLINE this_type& operator=(ptr::nullptr_t) noexcept { 
		return clear();
	}

	// copy assign, avoid self assign
	CCDK_FORCEINLINE this_type& operator=(vector const& other) {
		ccdk_if_not_this(other) { reallocate_copy(other.content, other.len); }	
		return *this;
	}

	// template copy assign 
	template<typename Size2, typename Alloc2, typename Ratio2>
	CCDK_FORCEINLINE this_type& operator=(vector<T, Ratio2, Size2, Alloc2> const& other) {
		reallocate_copy(other.content, other.len);
		return *this;
	}

	// move assign ,avoid self move 
	CCDK_FORCEINLINE this_type& operator=(vector && other) {
		ccdk_if_not_this(other) {
			this->~this_type();  
			rvalue_set(other.content, other.len, other.cap);
			other.rvalue_reset();
		}
		return *this;
	}

	// template move assign 
	template<typename Size2, typename Alloc2, typename Ratio2>
	CCDK_FORCEINLINE this_type& operator=(vector<T, Ratio2, Size2, Alloc2> && other) {
		this->~this_type();
		rvalue_set(other.content, other.len, other.cap);
		other.rvalue_reset();
		return *this;
	}

	// fill n elements 
	CCDK_FORCEINLINE this_type& assign(size_type n, T const& t = T() ) {
		reallocate_fill(t, n);
		return *this;
	}

	// free self then copy [begin, begin+n)
	template<
		typename InputIt,
		typename = check_t< is_iterator<InputIt>> 
	>
	CCDK_FORCEINLINE this_type& assign(InputIt begin, ptr::size_t n){
		reallocate_copy(begin, n);
		return *this;
	}

	// free self then copy [begin, end)
	template<typename InputIt,
		typename = check_t< is_iterator<InputIt>>
	>
	CCDK_FORCEINLINE this_type& assign(InputIt begin, InputIt end){
		return assign(begin, it::distance(begin, end));
	}

	// literial array initialize
	CCDK_FORCEINLINE this_type& assign(std::initializer_list<T> const& lst){
		return assign(lst.begin(), lst.size());
	}

////////////////////////////////////////////////////////////////////////
//// readonly attribute

	CCDK_FORCEINLINE size_type size() const noexcept { return len; }
	CCDK_FORCEINLINE size_type capacity() const noexcept { return cap; }
	CCDK_FORCEINLINE bool empty() const noexcept { return len == 0; }
	CCDK_FORCEINLINE constexpr size_type max_size() const noexcept { 
		static constexpr size_type kMaxSize = size_type(-1);
		return kMaxSize;
	}
	
/////////////////////////////////////////////////////////////////
//// iterator 
	CCDK_FORCEINLINE iterator begin() noexcept{ 
		ccdk_assert(content);
		return content;
	}
	CCDK_FORCEINLINE iterator end() noexcept { 
		ccdk_assert(content && len > 0); 
		return content + len;
	}
	CCDK_FORCEINLINE const_iterator cbegin() const noexcept { 
		ccdk_assert(content);
		return content;
	}
	CCDK_FORCEINLINE const_iterator cend() const noexcept {
		ccdk_assert(content && len > 0 );
		return content + len;
	}
	CCDK_FORCEINLINE reverse_iterator rbegin() noexcept {
		ccdk_assert(content);  
		return {content+len-1};
	}
	CCDK_FORCEINLINE reverse_iterator rend() noexcept {
		ccdk_assert(content && len > 0);
		return { content - 1 };
	}
	CCDK_FORCEINLINE const_reverse_iterator crbegin() const noexcept {
		ccdk_assert(content);  
		return { content + len-1 };
	}
	CCDK_FORCEINLINE const_reverse_iterator crend() const noexcept {
		ccdk_assert(content&& len > 0);  
		return { content-1 };
	}

//////////////////////////////////////////////////////////////////////
//// access elements
	
	//index
	CCDK_FORCEINLINE T& operator[](size_type index) noexcept {
		ccdk_assert(index < len);
		return content[index]; 
	}
	//const index
	CCDK_FORCEINLINE T const& operator[](size_type index) const noexcept { 
		ccdk_assert(index < len);
		return content[index];  
	}
	
	//index
	CCDK_FORCEINLINE T& at(size_type index) noexcept {
		ccdk_assert(index < len);
		return content[index]; 
	}

	//index
	CCDK_FORCEINLINE T const& at(size_type index) const noexcept { return content[index]; }
	
	// first element
	CCDK_FORCEINLINE T& front() noexcept { return content[0]; }
	CCDK_FORCEINLINE T const& front() const noexcept { return content[0]; }
	
	// last element
	CCDK_FORCEINLINE T& back() noexcept { return content[len-1]; }
	CCDK_FORCEINLINE T const& back() const noexcept { return content[len-1]; }

	// last n-th element
	CCDK_FORCEINLINE T& back(size_type n) noexcept { return content[len - 1 - n]; }
	CCDK_FORCEINLINE T const& back(size_type n) const noexcept{ return content[len - 1 - n]; }
	
	//inner address 
	CCDK_FORCEINLINE T* data() noexcept { return content; }
	CCDK_FORCEINLINE T const* data() const noexcept { return content; }

/////////////////////////////////////////////////////////////////////////
//// elements manipulate

	//  pop back element, destruct it  
	CCDK_FORCEINLINE this_type& pop_back() noexcept { 
		if (len > 0) { util::destruct<T>(content + --len); }
		return *this;
	}

	// pop last n-elements , destruct it
	CCDK_FORCEINLINE this_type pop_back(size_type n) noexcept {
		ccdk_assert(n < len && n>0);
		if (n > 0 && n < len) { 
			util::destruct_n(content + len - n, n);
			len -= n; 
		}
		return *this;
	}

	// inplace construct with  args... 
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

	// copy-construct T into back
	CCDK_FORCEINLINE this_type& push_back(T const& t)  { return emplace_back(t); }

	// move-contruct T into back 
	CCDK_FORCEINLINE this_type& push_back(T&& t) { return emplace_back(util::move(t)); }

	// copy-construct range [begin, begin+n) to back 
	template<typename InputIt>
	CCDK_FORCEINLINE this_type& push_back(InputIt begin, ptr::size_t n) {
		return insert_impl(len, len + n, begin);
	}

	// copy-construct range [begin, end) to back 
	template<typename InputIt>
	CCDK_FORCEINLINE this_type& push_back(InputIt begin,InputIt end) {
		return push_back(begin, alg::distance(begin, end));
	}

	// copy initialize list to back 
	CCDK_FORCEINLINE this_type& push_back(std::initializer_list<T> const& lst) {
		return push_back(lst.begin(), lst.size());
	}

	// inplace construct and insert at index position
	template< 
		typename... Args,
		typename = check_t< has_constructor<T, Args...>>
	>
	CCDK_FORCEINLINE this_type& emplace(size_type pos, Args&&... args) {
		ccdk_assert(pos <= len);
		if (pos <= len) { emplace_impl(pos, pos + 1, util::forward<Args>(args)...); }
		return *this;
	}

	// inplace construct and insert at iterator position
	template<
		typename Iterator,
		typename... Args,
		typename = check_t<is_pointer_iterator<T, Iterator> >, // exclude comflict with size_type
		typename = check_t< has_constructor<T, Args...>>
	>
	CCDK_FORCEINLINE this_type& emplace(Iterator it, Args&&... args) {
		return emplace(it - content, util::forward<Args>(args)...);
	}

	// insert at index position 
	CCDK_FORCEINLINE this_type& insert(size_type pos, T const& t) { return emplace(pos, t);  }

	// insert at iterator position
	template<
		typename Iterator,
		typename = check_t<is_pointer_iterator<T, Iterator> > // exclude comflict with size_type
	>
	CCDK_FORCEINLINE this_type& insert(Iterator it, T const& t){  return emplace(it - content, t); }

	// insert at index position and move 
	CCDK_FORCEINLINE this_type& insert(size_type pos, T&& t) { return emplace(pos, util::move(t)); }

	// insert at Iterator position and move 
	template<
		typename Iterator, 
		typename = check_t<is_pointer_iterator<T,Iterator> > // exclude comflict with size_type
	>
	CCDK_FORCEINLINE this_type& insert(Iterator it, T&& t) { return emplace(it - content, util::move(t)); }

	// insert at index position with range [begin, begin+n) 
	template<
		typename InputIt,
		typename = check_t< has_constructor<T, iterator_value_t<InputIt>>> 
	>
	CCDK_FORCEINLINE this_type& insert(size_type pos, InputIt begin, size_type n){
		return insert_impl(pos, pos + n, begin);
	}

	// insert at Iterator position with range [begin, begin+n) 
	template<
		typename Iterator,
		typename InputIt,
		typename = check_t< is_pointer_iterator<T, Iterator> >, // exclude comflict with size_type
		typename = check_t< has_constructor<T, iterator_value_t<InputIt>>> 
	>
	CCDK_FORCEINLINE this_type& insert(Iterator it, InputIt begin, size_type n) {
		return insert_impl(it -content, it - content + n, begin);
	}

	// insert at index position with range [begin, end) 
	template<
		typename InputIt, 
		typename = check_t< has_constructor<T, iterator_value_t<InputIt>>> 
	>
	CCDK_FORCEINLINE this_type& insert(size_type pos, InputIt begin, InputIt end){
		return insert(pos, begin, it::distance(begin, end));
	}

	// insert at Iterator position with range [begin, end) 
	template<
		typename Iterator,
		typename InputIt,
		typename = check_t<is_pointer_iterator<T, Iterator> >, // exclude comflict with size_type
		typename = check_t< has_constructor<T, iterator_value_t<InputIt>>>
	>
		CCDK_FORCEINLINE this_type& insert(Iterator it, InputIt begin, InputIt end) {
		return insert( it - content, begin, it::distance(begin, end));
	}

	// insert at index position with initialize list
	CCDK_FORCEINLINE this_type& insert(size_type pos, std::initializer_list<T> const& lst){
		return insert(pos, lst.begin(), lst.size());
	}

	// insert at Iterator position with initialize list
	template<
		typename Iterator,
		typename = check_t<is_pointer_iterator<T, Iterator> > // exclude comflict with size_type
	>
	CCDK_FORCEINLINE this_type& insert(Iterator it, std::initializer_list<T> const& lst){
		return insert(it - content, lst.begin(), lst.size());
	}

	// erase with index range [start, end)
	CCDK_FORCEINLINE this_type& erase(size_type start, size_type end) {
		ccdk_assert(start < end && end <= len);
		size_type n = end - start;
		if (n>0 && end <= len) {
			util::move_n(content + start, content + end, len-end);
			util::destruct_n(content + len-n, n);
			len -= n;
		}
		return *this;
	}

	// erase with index range [start, size())
	CCDK_FORCEINLINE this_type& erase(size_type start) { return erase(start, len); }

	// erase with iterator range [start, end())
	template<
		typename Iterator,
		typename = check_t<is_pointer_iterator<T, Iterator> > // exclude comflict with size_type
	>
	CCDK_FORCEINLINE this_type& erase(Iterator begin) { return erase(begin-content, len); }

	// erase with iterator range [start, end )
	template<
		typename Iterator,
		typename = check_t<is_pointer_iterator<T, Iterator> > // exclude comflict with size_type
	>
	CCDK_FORCEINLINE this_type& erase(Iterator begin, Iterator end) {
		return erase(begin - content, end-content); 
	}

	// clear, destruct all objects 
	CCDK_FORCEINLINE this_type& clear() { destruct_content(); return *this; }

//////////////////////////////////////////////////////////////////////////////////////////
//// find ops

	// find Times-th element index confirm with Pred function from start
	template<uint32 Times = 1, typename Pred>
	CCDK_FORCEINLINE size_type find_index(Pred const& pred, size_type start = 0) const noexcept {
		//ccdk_assert(start < size());
		uint32 count = 0;
		for (size_type i = start; i < len; ++i)
			if (pred(content[i])) {
				if (++count == Times) return i;
			}
		return len;
	}

	// find Times-th element confirm with Pred function 
	template<uint32 Times = 1, typename Pred>
	CCDK_FORCEINLINE iterator find(Pred const& pred, size_type start = 0) noexcept {
		return content + find_index<Times>(pred, start);
	}
	
	// find Times-th const element confirm with Pred function 
	template<uint32 Times = 1, typename Pred>
	CCDK_FORCEINLINE const_iterator find(Pred const& pred, size_type start = 0) const noexcept {
		return content + find_index<Times>(pred, start);
	}

//////////////////////////////////////////////////////////////////////////////
//// transform

	//inner loop
	template<typename Fn>
	CCDK_FORCEINLINE void foreach(Fn const& fn) const noexcept {
		for (size_type i = 0; i < len; ++i) {
			fn(content[i]);
		}
	}

	//mapping to new vector
	template<typename Fn>
	CCDK_FORCEINLINE auto map(Fn const& fn) const noexcept {
		using type = decltype(fn(declval<T>()));
		vector<type> ret{len};
		for (size_type i = 0; i < len; ++i) {
			ret.emplace_back(fn(content[i]));
		}
		return ret;
	}

	//fold left
	template<typename Fn>
	CCDK_FORCEINLINE auto fold_left(Fn const& fn, T const& init = T()) const noexcept {
		T ret{init};
		for (size_type i = 0; i < len; ++i) {
			ret = fn(ret, content[i]);
		}
		return ret;
	}

	//fold right
	template<typename Fn>
	CCDK_FORCEINLINE auto fold_right(Fn const& fn, T const& init = T()) const noexcept {
		T ret{ init };
		for (difference_type i = len-1; i >=0; --i) {
			ret = fn(ret, content[i]);
		}
		return ret;
	}

private:
	// reset but not deallocate memory 
	CCDK_FORCEINLINE void rvalue_reset() noexcept {
		content = nullptr; len = 0;  cap = 0;
	}

	// set but not allocate 
	CCDK_FORCEINLINE void rvalue_set(pointer otherContent,
		size_type otherLen, size_type otherCap) noexcept {
		content = otherContent;
		len = otherLen;
		cap = otherCap;
	}

	// if len > 0 , destruct objects 
	CCDK_FORCEINLINE void destruct_content() noexcept {
		if (len > 0) { util::destruct_n(content, len); len = 0; }
	}

	//locally request n-elements 
	CCDK_FORCEINLINE void allocate(size_type n) {
		ccdk_increase_allocate2(n, content, cap);
	}

	//locally request n-elements and set len = n
	CCDK_FORCEINLINE void allocate_set_length(size_type n) {
		ccdk_increase_allocate3(n, content, cap, len);
	}

	//locally reallocate n-elements
	CCDK_FORCEINLINE void reallocate(size_type n) {
		this->~this_type();
		allocate_set_length(n);
	}

	//recycle memory
	CCDK_FORCEINLINE void deallocate() noexcept {
		ccdk_assert((content && cap > 0) || (!content && cap == 0));
		if (content && cap > 0) {
			allocator_type::deallocate(*this, content, cap);
			cap = 0;
		}
	}

	/*
		allocate n*increase size memory to content,
		then fill [begin, begin+n) with v
	*/
	CCDK_FORCEINLINE void allocate_fill(size_type n, T const& v ) {
		if (n == 0) return;
		allocate_set_length(n);
		util::construct_fill_n(content, v, n);
	}

	/*
		allocate n*increase size memory to content, 
		then copy[begin, begin+n) to [content, content+n)
	*/
	template<typename InputIt>
	CCDK_FORCEINLINE void allocate_copy(size_type n, InputIt begin){
		if (n == 0) return;
		allocate_set_length(n);
		util::construct_copy_n(content, begin, n);
	}

	/*
		if content big enough to hold n elements, just copy [begin, begin+n) to it
		else reallocate new big enough memory and copy
	*/
	template<typename InputIt>
	CCDK_FORCEINLINE void reallocate_copy(InputIt begin, size_type n) {
		if (!content || n > cap) {
			reallocate(n);
		}
		else {
			destruct_content();
		}
		util::construct_copy_n(content, begin, n);
		len = n;
	}

	/*
		if content big enough to hold n elements, just fill it
		else allocate new big enough memory and fill
	*/
	CCDK_FORCEINLINE void reallocate_fill(T const& t, size_type n) {
		if (!content || n > cap) {
			reallocate(n);
		}
		else {
			destruct_content();
		}
		util::construct_fill_n(content, t, n);
		len = n;
	}

	/*
		allocate a new memory, if success
		move [content, content+n) to this [memory, memory+n), free old content
	*/
	CCDK_FORCEINLINE void reallocate_move() {
		ccdk_increase_allocate3(len, T* memory,
			size_type new_cap, size_type new_len);
		util::construct_move_n(memory, content, len);
		this->~this_type();
		content = memory;
		cap = new_cap;
		len = new_len;
	}

	/*  
		split copy [content, content+pos) to [content,content+pos),
		[content+pos, content+len) to [memory+pos+n, memory+len+n)
	*/
	CCDK_FORCEINLINE void split_move(T* memory, size_type pos, size_type n) noexcept{
		util::construct_move_n(memory, content, pos);
		util::construct_move_n(memory + pos + n, content + pos, len - pos);
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
				allocator_type::deallocate(*this,memory,new_cap)
			); 
			split_move(memory, start, n);
			this->~this_type();
			content = memory;
			cap = new_cap;
		} else {
			//insert inner [0, start, end, len)
			if (end < len) {
				util::construct_move_n(content + len, content + len - n, n);
				util::move_n(content + end, content + start, len - end);
			}
			// insert like [0, start, len, end) or [0, start=len, end)
			else  {
				util::construct_move_n(content + len, content + start, len - start);
			}
			util::destruct_n(content + start, n);
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
			ccdk_increase_allocate2(new_len,T* memory, size_type new_cap);
			ccdk_safe_cleanup_if_exception(
				util::construct_n<T>(memory + start, n, util::forward<Args>(args)...),
				allocator_type::deallocate(*this, memory, new_cap)
			);
			split_move(memory, start, n);
			this->~this_type();
			content = memory;
			cap = new_cap;
		} else {
			//insert inner [0, start, end, len)
			if (end < len) {
				util::construct_move_n(content + len, content + len - n, n);
				util::move_n(content + end, content + start, len - end);
			}
			// insert like [0, start, len, end) or [0, start=len, end)
			else {
				util::construct_move_n(content + len, content + start, len - start);
			}
			util::destruct<T>(content + start);
			util::construct_n<T>(content+start, n, util::forward<Args>(args)...);
		}
		len = new_len;
	}

////////////////////////////////////////////////////////////////////////////////////
//// debug
public:

	void debug_all(const char* str="") const noexcept {
		DebugValueItBegin(str);
		for (uint32 i = 0; i < len; ++i) {
			DebugValueIt(this->at(i));
		}
		DebugValueItEnd();
	}
};


ccdk_namespace_ct_end