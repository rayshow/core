#pragma once

#include<ccdk/container/container_mudule.h>
#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/mcontainer/val_pack.h>
#include<ccdk/mpl/function/operator.h>
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
	
	template<typename T, typename Size2, typename Alloc2, typename Ratio2>
	friend class vector;

private:
	T *content;
	size_type len;
	size_type cap;

	/* clear but not destruct memory */
	CCDK_FORCEINLINE void rvalue_reset() noexcept {
		content = nullptr; len = 0;  cap = 0; 
	}

	/* assign from another vector, space enough, destruct old , copy new  */
	CCDK_FORCEINLINE void lvalue_assign(T* inContent, size_type inLen) {
		util::destruct_n(content, len);
		util::construct_copy_n(content, other.content, other.len);
		len = inLen;
	}

	CCDK_FORCEINLINE size_type prealloc_cap(size_type n)
	{
		size_type actual_size = incease_ratio::multiply(n);
		if (n == size_type(-1) || actual_size < n) throw std::bad_alloc{};
	}

	/* alloc n*increase_ratio elements */
	CCDK_FORCEINLINE void alloc_cap(size_type n) {
		T* memory = allocate(n);
		if (!memory) throw std::bad_cast{}
		return memory;
	}

	void realloc_move(size_type n) {
		size_type new_cap = prealloc_cap(len);
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
		size_type add_len = end - start;
		if (add_len + len > cap)
		{
			size_type new_len = len + add_len;
			size_type new_cap = prealloc_cap(new_len);
			T* memory = alloc_cap(new_cap);
			// move content[0, start) to memory[0,start)
			util::construct_move_n(memory, content, start);
			// move content[start, len) to memory[end, end+len-start) 
			util::construct_move_n(memory + end, content + start, len - start)
			// copy [begin, begin + end -start) to content[start, end)
			ccdk_safe_cleanup_if_exception(
				util::construct_copy_n(memory + start, begin, add_len), /* may throw */
				deallocate(memory)
			);
			deallocate(content);
			content = memory;
			len = new_len;
			cap = new_cap;
		}
		else {
			if (end >= len)
			{
				size_type mstart = fn::min(end, len);
				util::construct_move_n(content + mstart, content + start, add_len);
				util::copy_n(content + start, begin, add_len);
			}
			else {
				//end < len
				size_type move_pos = len - add_len;
				util::construct_move_n(content + len, content + move_pos, add_len);
				util::move_n(content + end, content+start, move_pos - start );
				util::construct_copy_n(content + start, begin, add_len),  
			}
			len += add_len;
		}
	}

	/* if capacity is not enough, realloc big enough memory to hold emplace construct data,
		memory[start, end) stay empty for external data.
	   else move content[start, len) backward
	*/
	template<typename... Args>
	void emplace_impl(size_type start, size_type end, Args&&... args)
	{
		size_type add_len = end - start;
		if (add_len + len > cap)
		{
			size_type new_len = len + add_len;
			size_type new_cap = prealloc_cap(new_len);
			T* memory = alloc_cap(new_cap);
			// move content[0, start) to memory[0,start)
			util::construct_move_n(memory, content, start);
			// move content[start, len) to memory[end, end+len-start) 
			util::construct_move_n(memory + end, content + start, len - start);
			
			// copy [begin, begin + end -start) to content[start, end)
			ccdk_safe_cleanup_if_exception(
				util::construct_n<T>(memory+start, add_len, util::forward<Args>(args)... ), /* may throw */
				deallocate(memory)
			);

			//all success, apply change
			deallocate(content);
			content = memory;
			len = new_len;
			cap = new_cap;
		}
		else {
			if (end >= len)
			{
				util::construct_move_n(content + end, content + start, add_len);
				util::destruct_n(content + start, add_len);
				util::construct_n(content + start, add_len, util::forward<Args>(args)...);
			}
			else {
				//end < len
				size_type move_pos = len - add_len;
				util::construct_move_n(content + len, content + move_pos, add_len);
				util::move_n(content + end, content + start, move_pos - start);
				util::destruct_n(content + start, add_len);
				util::construct_n(content + start, add_len, util::forward<Args>(args)... ),
			}
			len += add_len;
		}
	}


public:
	/* default */
	CCDK_FORCEINLINE constexpr vector()
		:content{ nullptr }, len{ 0 }, cap{ 0 } {}

	/* fill*/
	CCDK_FORCEINLINE explicit vector(size_type n, T const& t = T()){
		alloc_len(n);
		construct_fill_n(content, t, n);
	}

	/* copy*/
	CCDK_FORCEINLINE vector(vector const& other) {
		alloc_len(other.len);
		construct_copy_n(content, other.content, other.len);
	}

	template<typename Size2, typename Alloc2, typename Ratio2>
	CCDK_FORCEINLINE vector(vector<T, Size2, Alloc2, Ratio2> const& other) {
		alloc_len(other.len);
		construct_copy_n(content, other.content, other.len);
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

	/* assign  nullptr, first destruct objects */
	CCDK_FORCEINLINE operator=(ptr::nullptr_t) noexcept { 
		util::destruct_n(content, len);
		len = 0; 
	}

	/* copy assign */
	CCDK_FORCEINLINE operator=(vector const& other) {
		ccdk_if_not_this(other) { 
			if ( other.len <= cap) lvalue_assign(other.content, other.len);
			else  vector{ other }.swap(*this);
		}	
	}

	/* template copy assign */
	template<typename Size2, typename Alloc2, typename Ratio2>
	CCDK_FORCEINLINE operator=(vector<T, Size2, Alloc2, Ratio2> const& other) {
		if (other.len <= cap) lvalue_assign(other.content, other.len);
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
		util::destruct_n<T>(content, len);  len = 0; 
		return *this;
	}

	/* fill with nxt */
	CCDK_FORCEINLINE this_type& assign(size_type n, T const& t) {
		if (n <= cap) {
			util::destruct_n(content, len);
			util::construct_fill_n(content, t, n);
		}
		else { vector{ n,t }.swap(*this);  }
		return *this;
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
		if (len > 0) { util::destruct(content + --len); }
		return *this;
	}

	CCDK_FORCEINLINE this_type& push_back(T const& t)  {
		if (len == cap) { realloc_move(len); }
		util::construct(content + len++, t);
		return *this;
	}

	template<typename P, typename ... Args>
	CCDK_FORCEINLINE this_type& emplace_back(P&& p, Args&& ... args)
	{
		if (len == cap) { realloc_move(len); }
		util::construct(content + len++, util::forward<P>(p),
			util::forward<Args>(args)... );
		return *this;
	}

	CCDK_FORCEINLINE this_type& insert(ptr::size_t pos, T const& t)
	{
		ccdk_assert(pos < len);
		if (pos >= 0 && pos < len) { insert_impl(pos, pos + 1, util::addressof(t)); }
	}

	CCDK_FORCEINLINE this_type& emplace(ptr::size_t pos, T const& t)
	{

	}

	template<
		typename InputIt, 
		typename = check_t< has_constructor<T, iterator_value_t<InputIt>>> >
	CCDK_FORCEINLINE this_type& insert(ptr::size_t pos, InputIt begin, InputIt end)
	{
		return insert_impl(pos, pos + alg::distance(begin, end), begin);
	}



};


ccdk_namespace_ct_end