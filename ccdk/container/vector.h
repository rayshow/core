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
#include<ccdk/memory/allocator/simple_new_allocator.h>
#include<ccdk/memory/allocator/semi_stack_allocator.h>
#include<ccdk/memory/adapter/array_allocate_adapter.h>
#include<ccdk/container/container_mudule.h>

ccdk_namespace_ct_start

using namespace ccdk::mpl;


struct vector_actual_size_tag {};

template<
	typename T,                              /* element type */
	typename IncRatio = units::ratio<2, 1>,  /* 2X incease ratio*/
	uint32  KLeastElemntCount = 10,          /* least allocate 10 elements */
	typename Size = uint32,                  /* size type */
	typename Alloc = mem::simple_new_allocator<T,Size> /* basic allocator */
>
class vector : protected Alloc
{
public:
	using this_type       = vector;
	using value_type      = T;
	using pointer         = T *;
	using const_pointer   = T const*;
	using reference       = T &;
	using const_reference = T const&;
	using size_type       = Size;
	using difference_type = ptr::diff_t;
	using allocator_type  = mem::array_allocate_adapter<
		KLeastElemntCount, IncRatio, Alloc>;

	/* iterator */
	using iterator               = T*;
	using const_iterator         = T const*;
	using reverse_iterator       = it::reverse_iterator<T*>;
	using const_reverse_iterator = it::reverse_iterator<const T*>;
	
	template<typename, typename, uint32, typename, typename>
	friend class vector;
	
protected:
	pointer   content;
	size_type len;
	size_type cap;

	//for local vector
	CCDK_FORCEINLINE explicit vector(vector_actual_size_tag, size_type n){
		ccdk_assert(n > 0);
		content = allocator_type::allocate(*this, n);
		cap = n;
		len = 0;
	}

public:

/////////////////////////////////////////////////////////////////
//// dector / ctor

	// destruct 
	CCDK_FORCEINLINE ~vector() noexcept { destroy(); }

	// default and nullptr ctor
	CCDK_FORCEINLINE constexpr vector() : content{ nullptr }, len{ 0 }, cap{ 0 } {}
	CCDK_FORCEINLINE constexpr vector(ptr::nullptr_t) : content{ nullptr }, len{ 0 }, cap{ 0 } {}

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
		if (n == 0) return;
		allocate(n);
		util::construct_fill_n(content, t, n);
		len = n;
	}

	// copy range-n ctor
	template< typename InputIt, typename = check_t< is_iterator<InputIt>> >
	CCDK_FORCEINLINE vector(InputIt begin, ptr::size_t n){
		ccdk_assert(n > 0);
		if (n == 0) return;
		allocate(n);
		util::construct_copy_n(content, begin, n);
		len = n;
	}

	// copy range ctor
	template<typename InputIt, typename = check_t< is_iterator<InputIt>>  >
	CCDK_FORCEINLINE vector(InputIt begin, InputIt end)
		: vector{ begin, it::distance(begin, end) } {}

	// copy ctor
	CCDK_FORCEINLINE vector(vector const& other) 
		: vector{ other.begin(), other.size() } {}

	// template copy ctor
	template<typename Size2, typename Alloc2, typename Ratio2, uint32 N >
	CCDK_FORCEINLINE explicit vector(vector<T, Ratio2, N, Size2, Alloc2> const& other) 
		: vector{ other.begin(), other.size() } {}

	// array initialize, instead of initialize_list
	template<uint32 N>
	CCDK_FORCEINLINE vector(T const (&arr)[N]) 
		: vector{ arr, N } {}

	// move ctor
	CCDK_FORCEINLINE vector(vector && other) noexcept
		:content{ other.content }, len{ other.len }, cap{ other.cap } {
		other.rvalue_reset();
	}

	// template move ctor
	template<typename Size2, typename Ratio2, uint32 N>
	CCDK_FORCEINLINE vector(vector<T, Ratio2, N, Size2, Alloc> && other) noexcept
		: content{ other.content }, len{ other.len }, cap{ other.cap } {
		other.rvalue_reset();
	}

///////////////////////////////////////////////////////////////////////////
//// swap / assign

	// swap 2 vector
	CCDK_FORCEINLINE void swap(vector& other) noexcept {
		util::swap(content, other.content);
		util::swap(len, other.len);
		util::swap(cap, other.cap);
	}

	// destruct all object and reset size, but keep memory and capcity
	CCDK_FORCEINLINE this_type& operator=(ptr::nullptr_t) noexcept { return clear(); }

	// copy assign, avoid self assign
	CCDK_FORCEINLINE this_type& operator=(vector const& other) {
		ccdk_if_not_this(other) { 
			return assign(other.begin(), other.size());
		}	
		return *this;
	}

	// template copy assign 
	template<typename Size2, typename Alloc2, typename Ratio2, uint32 N>
	CCDK_FORCEINLINE this_type& operator=(
		vector<T, Ratio2, N, Size2, Alloc2> const& other) {
		return assign(other.begin(), other.size());
	}

	// move assign ,avoid self move 
	CCDK_FORCEINLINE this_type& operator=(vector && other) {
		ccdk_if_not_this(other) {
			destroy();
			rvalue_set(other.content, other.len, other.cap);
			other.rvalue_reset();
		}
		return *this;
	}

	// template move assign 
	template<typename Size2, typename Ratio2, uint32 N>
	CCDK_FORCEINLINE this_type& operator=(
		vector<T, Ratio2, N, Size2,  Alloc> && other) {
		destroy();
		rvalue_set(other.content, other.len, other.cap);
		other.rvalue_reset();
		return *this;
	}

	// fill n elements 
	CCDK_FORCEINLINE this_type& assign(size_type n, T const& t = T() ) {
		if (!content || n > cap) { reallocate(n); }
		else { destruct_content(); }
		util::construct_fill_n(content, t, n);
		len = n;
		return *this;
	}

	// free self then copy [begin, begin+n)
	template< typename InputIt, typename = check_t< is_iterator<InputIt>>  >
	CCDK_FORCEINLINE this_type& assign(InputIt begin, ptr::size_t n){
		if (!content || n > cap) { reallocate(n); }
		else { destruct_content(); }
		util::construct_copy_n(content, begin, n);
		len = n;
		return *this;
	}

	// free self then copy [begin, end)
	template<typename InputIt, typename = check_t< is_iterator<InputIt>> >
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
	CCDK_FORCEINLINE constexpr size_type max_size() const noexcept { return size_type(-1);}
	
/////////////////////////////////////////////////////////////////
//// iterator 
	CCDK_FORCEINLINE iterator begin() noexcept{ ccdk_assert(content); return content; }
	CCDK_FORCEINLINE iterator end() noexcept { ccdk_assert(content && len > 0); return content + len; }
	CCDK_FORCEINLINE const_iterator begin() const noexcept { ccdk_assert(content); return content;}
	CCDK_FORCEINLINE const_iterator end() const noexcept { ccdk_assert(content && len > 0); return content + len; }
	CCDK_FORCEINLINE const_iterator cbegin() const noexcept { ccdk_assert(content); return content; }
	CCDK_FORCEINLINE const_iterator cend() const noexcept { ccdk_assert(content && len > 0 ); return content + len;}
	CCDK_FORCEINLINE reverse_iterator rbegin() noexcept { ccdk_assert(content); return {content+len-1}; }
	CCDK_FORCEINLINE reverse_iterator rend() noexcept { ccdk_assert(content && len > 0); return { content - 1 }; }
	CCDK_FORCEINLINE const_reverse_iterator rbegin() const noexcept { ccdk_assert(content); return { content + len - 1 }; }
	CCDK_FORCEINLINE const_reverse_iterator rend() const noexcept { ccdk_assert(content && len > 0); return { content - 1 }; }
	CCDK_FORCEINLINE const_reverse_iterator crbegin() const noexcept { ccdk_assert(content); return { content + len-1 }; }
	CCDK_FORCEINLINE const_reverse_iterator crend() const noexcept { ccdk_assert(content&& len > 0); return { content-1 }; }

//////////////////////////////////////////////////////////////////////
//// access elements
	
	CCDK_FORCEINLINE reference       operator[](size_type index) noexcept { ccdk_assert(index < len);return content[index];  }
	CCDK_FORCEINLINE const_reference operator[](size_type index) const noexcept { ccdk_assert(index < len);return content[index];}
	CCDK_FORCEINLINE reference       at(size_type index) noexcept { ccdk_assert(index < len); return content[index];  }
	CCDK_FORCEINLINE const_reference at(size_type index) const noexcept { ccdk_assert(index < len); return content[index]; }
	CCDK_FORCEINLINE reference       front() noexcept { ccdk_assert(content); return content[0]; }
	CCDK_FORCEINLINE const_reference front() const noexcept { ccdk_assert(content); return content[0]; }
	CCDK_FORCEINLINE reference       back() noexcept { ccdk_assert(content); return content[len-1]; }
	CCDK_FORCEINLINE const_reference back() const noexcept { ccdk_assert(content); return content[len-1]; }
	CCDK_FORCEINLINE reference       back(size_type n) noexcept { ccdk_assert(content); return content[len - 1 - n]; }
	CCDK_FORCEINLINE const_reference back(size_type n) const noexcept{ ccdk_assert(content); return content[len - 1 - n]; }
	CCDK_FORCEINLINE pointer         data() noexcept { return content; }
	CCDK_FORCEINLINE const_pointer   data() const noexcept { return content; }

/////////////////////////////////////////////////////////////////////////
//// pop / push back

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
		typename ... Args,
		typename  = check_t< has_constructor< T, Args...>> >
	CCDK_FORCEINLINE this_type& emplace_back(Args&& ... args){
		if (len == cap) { reallocate_move(); }
		util::construct<T>(content + len++, util::forward<Args>(args)...);
		return *this;
	}

	// copy-construct T into back
	CCDK_FORCEINLINE this_type& push_back(T const& t) { return emplace_back(t); }
	CCDK_FORCEINLINE this_type& push_back(T&& t) { return emplace_back(util::move(t)); }

	// copy-construct range [begin, begin+n) to back 
	template<typename InputIt>
	CCDK_FORCEINLINE this_type& push_back(InputIt begin, ptr::size_t n) {
		return insert_impl(len, len + n, begin);
	}

	// copy-construct range [begin, end) to back 
	template<typename InputIt>
	CCDK_FORCEINLINE this_type& push_back(InputIt begin,InputIt end) {
		return push_back(begin, it::distance(begin, end));
	}

	// copy initialize list to back 
	CCDK_FORCEINLINE this_type& push_back(std::initializer_list<T> const& lst) {
		return push_back(lst.begin(), lst.size());
	}

/////////////////////////////////////////////////////////////////////////////////
//// emplace / insert with index

	// inplace construct and insert at index position
	template< 
		typename... Args,
		typename = check_t< has_constructor<T, Args...>> >
	CCDK_FORCEINLINE this_type& emplace(size_type pos, Args&&... args) {
		ccdk_assert(pos <= len);
		if (pos <= len) { 
			emplace_impl(pos, pos + 1, util::forward<Args>(args)...); 
		}
		return *this;
	}

	// insert at index position 
	CCDK_FORCEINLINE this_type& insert(size_type pos, T const& t) { return emplace(pos, t); }
	CCDK_FORCEINLINE this_type& insert(size_type pos, T&& t) { return emplace(pos, util::move(t)); }

	// insert at index position with range [begin, begin+n) 
	template<
		typename InputIt,
		typename = check_t< has_constructor<T, iterator_value_t<InputIt>>> >
		CCDK_FORCEINLINE this_type& insert(size_type pos, InputIt begin, size_type n) {
		return insert_impl(pos, pos + n, begin);
	}

	// insert at index position with range [begin, end) 
	template<
		typename InputIt,
		typename = check_t< has_constructor<T, iterator_value_t<InputIt>>> >
		CCDK_FORCEINLINE this_type& insert(size_type pos, InputIt begin, InputIt end) {
		return insert(pos, begin, it::distance(begin, end));
	}

	// insert at index position with initialize list
	CCDK_FORCEINLINE this_type& insert(size_type pos, std::initializer_list<T> const& lst) {
		return insert(pos, lst.begin(), lst.size());
	}

////////////////////////////////////////////////////////////////////////////////////////////
//// emplace / insert with iterator

	// inplace construct and insert at iterator position
	template<
		typename Iterator,
		typename... Args,
		typename = check_t<is_pointer_iterator<T, Iterator>>,
		typename = check_t< has_constructor<T, Args...>> >
	CCDK_FORCEINLINE this_type& emplace(Iterator it, Args&&... args) {
		return emplace(it - content, util::forward<Args>(args)...);
	}

	// insert at iterator position
	template<
		typename Iterator,
		typename = check_t<is_pointer_iterator<T, Iterator>> >
	CCDK_FORCEINLINE this_type& insert(Iterator it, T const& t){ 
		return emplace(it - content, t); 
	}

	// insert at Iterator position and move 
	template<
		typename Iterator, 
		typename = check_t<is_pointer_iterator<T,Iterator>> >
	CCDK_FORCEINLINE this_type& insert(Iterator it, T&& t) { 
		return emplace(it - content, util::move(t)); 
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

	// insert at Iterator position with range [begin, end) 
	template<
		typename Iterator,
		typename InputIt,
		typename = check_t<is_pointer_iterator<T, Iterator> >,
		typename = check_t< has_constructor<T, iterator_value_t<InputIt>>> >
	CCDK_FORCEINLINE this_type& insert(Iterator it, InputIt begin, InputIt end) {
		return insert( it - content, begin, it::distance(begin, end));
	}

	// insert at Iterator position with initialize list
	template<
		typename Iterator,
		typename = check_t<is_pointer_iterator<T, Iterator>> >
	CCDK_FORCEINLINE this_type& insert(Iterator it, std::initializer_list<T> const& lst){
		return insert(it - content, lst.begin(), lst.size());
	}

////////////////////////////////////////////////////////////////////////////
//// erase / clear

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
		typename = check_t<is_pointer_iterator<T, Iterator> > >
	CCDK_FORCEINLINE this_type& erase(Iterator begin) { return erase(begin-content, len); }

	// erase with iterator range [start, end )
	template<
		typename Iterator,
		typename = check_t<is_pointer_iterator<T, Iterator>>  >
	CCDK_FORCEINLINE this_type& erase(Iterator begin, Iterator end) {
		return erase(begin - content, end-content); 
	}

	// clear, destruct all objects 
	CCDK_FORCEINLINE this_type& clear() { destruct_content(); return *this; }

//////////////////////////////////////////////////////////////////////////////////////////
//// find / find_index ops

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
//// transform / loopover

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

/////////////////////////////////////////////////////////////////////////
//// implements
protected:
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

	// if cap > 0 , deallocate content
	CCDK_FORCEINLINE void deallocate() noexcept {
		if (cap > 0) { allocator_type::deallocate(*this, content, cap); cap = 0; }
	}

	// destruct objs and deallocate
	CCDK_FORCEINLINE void destroy() noexcept {
		destruct_content();
		deallocate();
	}
	
	//locally request n-elements and set cap if successful
	CCDK_FORCEINLINE void allocate(size_type n) {
		content = allocator_type::allocate_inc(*this, n, &cap);
	}

	//locally request n-elements and set cap if successful
	CCDK_FORCEINLINE void allocate_with_len(size_type n) {
		content = allocator_type::allocate_inc(*this, n, &cap, &len);
	}

	//locally reallocate n-elements but not copy original content
	CCDK_FORCEINLINE void reallocate(size_type n) {
		destroy();
		allocate(n);
	}


	// allocate a new memory, if success
	// move [content, content+n) to this [memory, memory+n), free old content
	CCDK_FORCEINLINE void reallocate_move() {
		size_type new_cap, new_len;
		pointer memory = allocator_type::allocate_inc(*this, len, &new_cap, &new_len);
		util::construct_move_n(memory, content, len);
		destroy();
		content = memory;
		cap = new_cap;
		len = new_len;
	}

	// split copy [content, content+pos) to [content,content+pos),
	// [content+pos, content+len) to [memory+pos+n, memory+len+n)
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
			//content space not enough, need reallocate
			size_type new_cap;
			pointer memory = allocator_type::allocate_inc(*this, new_len, &new_cap);
			ccdk_safe_cleanup_if_exception(
				util::construct_copy_n(memory + start, begin, n), /* may throw */
				allocator_type::deallocate(*this, memory,new_cap)
			); 
			split_move(memory, start, n);
			destroy();
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
			size_type new_cap;
			pointer memory = allocator_type::allocate_inc(*this, new_len, &new_cap);
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

	void debug_value(const char* str="") const noexcept {
		DebugValueItBegin(str);
		for (uint32 i = 0; i < len; ++i) {
			DebugValueIt(this->at(i));
		}
		DebugValueItEnd();
	}
};

template<
	typename T,                                          /* element type */
	uint32  KLeastElemntCount = 32,                      /* least allocate 128 elements */
	typename IncRatio = units::ratio<2, 1>,              /* 2X incease ratio*/
	typename Size = uint32,                              /* size type */
	typename Alloc = mem::simple_new_allocator<T, Size>, /* basic allocator */
	typename Super = vector<T, IncRatio, KLeastElemntCount, Size,
	mem::semi_stack_allocator< KLeastElemntCount, Alloc>>
>
class local_vector : public Super
{
protected:
	using Super::content;
	using Super::len;
	using Super::cap;
	using Super::rvalue_set;
	using Super::rvalue_reset;
	using Super::destruct_content;
	using Super::destroy;
public:
	using this_type = local_vector;
	using size_type = Size;
	using allocator_type = typename Super::allocator_type;
	using pointer = typename Super::pointer;

	using Super::operator[];
	using Super::at;
	using Super::begin;
	using Super::end;
	using Super::cbegin;
	using Super::cend;
	using Super::rbegin;
	using Super::rend;
	using Super::crbegin;
	using Super::crend;
	using Super::size;
	using Super::capacity;
	using Super::max_size;
	using Super::empty;
	using Super::front;
	using Super::back;
	using Super::data;
	using Super::pop_back;
	using Super::push_back;
	using Super::emplace_back;
	using Super::emplace;
	using Super::insert;
	using Super::erase;
	using Super::clear;
	using Super::map;
	using Super::foreach;

	constexpr static vector_actual_size_tag ActualSizeInit{};
	
	//use stack memory, set capcity, but not set length
	local_vector() : Super{ ActualSizeInit, KLeastElemntCount } {}

	//0-fill-ctor, use stack memory  and set length = n
	local_vector(size_type n, ptr::nullptr_t) : local_vector{} {
		ccdk_assert(n <= KLeastElemntCount); //if trigger, you should incease the KLeastElemntCount
		memset(content, 0, n*sizeof(T));
		len = n;
	}

	//fill-ctor  , use stack memory and set length = n
	local_vector(size_type n, T const& t) :local_vector{} {
		ccdk_assert(n <= KLeastElemntCount); //if trigger, you should incease the KLeastElemntCount
		util::construct_fill_n(content, t, n);
		len = n;
	}

	//range-n
	template<typename InputIt, typename = check_t< is_iterator<InputIt>> >
	local_vector(InputIt begin, size_type n) :local_vector{} {
		ccdk_assert(n <= KLeastElemntCount); //if trigger, you should incease the KLeastElemntCount
		util::construct_copy_n(content, begin, n);
		len = n;
	}

	//range-ctor
	template<typename InputIt, typename = check_t< is_iterator<InputIt>> >
	local_vector(InputIt begin, InputIt end) 
		:local_vector{begin, it::distance(begin, end)} {}

	//copy-ctor
	local_vector(local_vector const& other)
		:local_vector{ other.begin(), other.size() } {}

	//array-ctor
	template<uint32 N>
	local_vector(T const (&arr)[N])
		: local_vector{ arr, N } {}

	//move
	local_vector(local_vector && other) noexcept {
		move_impl(util::move(other));
	}

//////////////////////////////////////////////////////////////////////////////
////  swap 

	void swap(this_type& other) {
		if (other.is_stack() && is_stack()) {
			uint32 max_byte = fn::max(other.size(), size()) * sizeof(T);
			uint8 tmp[KLeastElemntCount*sizeof(T)];
			memcpy(tmp, other.content, max_byte);
			memcpy(other.content, content, max_byte);
			memcpy(content, tmp, max_byte);
		} 
		else if (!other.is_stack() && !is_stack()) {
			util::swap(content, other.content);
		}
		else if (other.is_stack() && !is_stack()) {
			swap_stack_and_heap(other);
		}
		else {
			other.swap_stack_and_heap(*this);
		}
		util::swap(len, other.len);
		util::swap(cap, other.cap);
	}

//////////////////////////////////////////////////////////////////////////////////////
//// assign 

	//clear content
	this_type& operator=(ptr::nullptr_t) {
		destruct_content();
	}

	//assign copy
	this_type& operator=(this_type const& other) {
		ccdk_if_not_this(other) {
			return assign(other.begin(), other.size());
		}
		return *this;
	}

	//assign move
	this_type& operator=(this_type && other) {
		ccdk_if_not_this(other) {
			destroy();
			move_impl(util::move(other));
		}
		return *this;
	}

	//assign array
	this_type& operator=(std::initializer_list<T> const& lst) {
		return assign(lst.begin(), lst.size());
	}

	//range-n
	template<typename InputIt, typename = check_t<is_iterator<InputIt>>>
	this_type& assign(InputIt begin, size_type n) {
		assign_set_enough_memory(n);
		util::construct_copy_n(content, begin, n);
		len = n;
		return *this;
	}

	//range
	template<typename InputIt, typename = check_t<is_iterator<InputIt>>>
	this_type& assign(InputIt begin, InputIt end) {
		return assign(begin, it::distance( begin, end));
	}

	//fill-n
	this_type& assign(size_type n, T const& t) {
		assign_set_enough_memory(n);
		util::construct_fill_n(content, t, n);
		len = n;
		return *this;
	}

	CCDK_FORCEINLINE bool is_stack() const noexcept { return KLeastElemntCount == cap; }

private:

	void assign_set_enough_memory(size_type n) {
		if (capacity() == 0 && n <= KLeastElemntCount) {
			//first allocate and stack is big enough
			content = allocator_type::allocate(*this, KLeastElemntCount);
			cap = KLeastElemntCount;
		}
		else if (n > capacity()) {
			//memory not enough, reallocate it
			destroy();
			content = allocator_type::allocate_inc(*this, n, &cap);
		}
		else {
			//memory enough, destruct old content
			destruct_content();
		}
	}

	//this is heap based
	void swap_stack_and_heap(local_vector& stack) {
		pointer tmp = content;
		content = allocator_type::allocate(*this, KLeastElemntCount);
		if(stack.size()) memcpy(content, stack.content, stack.size() * sizeof(T));
		stack.content = tmp;
	}
	
	void move_impl(local_vector&& other) noexcept {
		if (other.size() == KLeastElemntCount) {
			//in stack, move content to this stack
			content = allocator_type::allocate(*this, KLeastElemntCount);
			memcpy(content, other.begin(), sizeof(T)*other.size());
			memset(other.begin(), 0, sizeof(T)*other.size());
			cap = KLeastElemntCount;
			len = other.size();
			other.rvalue_reset();
		}
		else {
			//in heap, transfer from other to this, and reset other
			rvalue_set(other.content, other.len, other.cap);
			other.rvalue_reset();
		}
	}

public:
	using Super::debug_value;

	void debug_is_stack() {
		DebugValue("is stack:", is_stack());
	}
};


ccdk_namespace_ct_end