#pragma once

#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/type_traits/declval.h>
#include<ccdk/mpl/type_traits/has_attribute.h>
#include<ccdk/mpl/function/operator.h>

#include<ccdk/mpl/fusion/pair.h>

#include<ccdk/memory/memory_module.h>
#include<ccdk/memory/allocator_traits.h>

ccdk_namespace_memory_start

using namespace ccdk::mpl;

template<typename T>
struct is_biward_node : and_< has_attribute_next<T>, has_attribute_prev<T>> {};

#define ccdk_increase_allocate_lst3(n, head,tail,cap)                     \
	size_type actual_size = increase_ratio::multiply(n);                  \
	if (n == size_type(-1) || actual_size < n) throw std::bad_alloc{};    \
	tie(head,tail) = allocate_type::allocate(*this, actual_size);         \
	cap = actual_size;

#define ccdk_increase_allocate_lst4(n, head,tail,cap, len)                \
	ccdk_increase_allocate_lst3(n,head,tail,cap)                          \
	len = n;	

/*		
		list adaptor of allocator: try to minimize memory fragment
	always allocate/deallocate continuous memory.
		when allocate, if allocate n * sizeof(T)  failed , 
	try allocate small frag memory like (n-1) * sizeof(T) ... sizeof(T)
		when deallocate, try detech continuous x * sizeof(T) memory and 
	recycle
*/
template<typename Alloc>
class list_allocate_adapter
{
public:
	typedef allocator_traits<Alloc> allocator;
	typedef typename allocator::value_type value_type;
	typedef typename allocator::size_type  size_type;

	template<typename U>
	using rebind = list_allocate_adapter<U>;

	/* single list */
	static auto link_memory(value_type* memory, ptr::size_t n, false_) noexcept{
		for (ptr::size_t i = 0; i < n - 1; ++i) {
			(memory + i)->next = (memory + i + 1);
		}
	}

	/* double list */
	static auto link_memory(value_type* memory, ptr::size_t n, true_) noexcept {
		for (ptr::size_t i = 0; i < n - 1; ++i) {
			(memory + i)->next = (memory + i + 1);
			(memory + i + 1)->prev = (memory + i);
		}
	}


	/* value_type.next must be valid */
	template<typename = check_t< has_attribute_next<value_type>>>
	static auto allocate(Alloc const& alloc, ptr::size_t n)
	{
		ccdk_assert(n > 0);
		if (n == 0) return nullptr;

		ptr::size_t not_allocated_size = n;
		ptr::size_t current_allocate_size = n;
		value_type* head = nullptr;
		value_type* tail = nullptr;
		while (not_allocated_size >0 ) {
			value_type* memory = nullptr;
			try {
				memory = allocator::allocate(alloc, current_allocate_size);
			} catch (...) {
				// allocate failed, try smaller size
				current_allocate_size = fn::max(1,--current_allocate_size);
				memory = nullptr;
			}
			if (memory) {
				link_memory(memory, current_allocate_size, is_biward_node<value_type>{});
				tail = memory + current_allocate_size - 1;
				tail->next = head;
				head = memory;
				not_allocated_size -= current_allocate_size;
				current_allocate_size = fn::min(current_allocate_size, not_allocated_size);
			}
		}

		if (not_allocated_size > 0) {
			deallocate(alloc, head, n - not_allocated_size);
			throw std::bad_alloc{};    //no enough memory to alloc
		}
		return fs::make_pair(head,tail);
	}

	/* value_type.next must be valid */
	template<typename = check_t< has_attribute_next<value_type>>>
	static void deallocate(Alloc const& alloc, value_type* pointer, ptr::size_t n) noexcept {
		ccdk_assert(pointer && n > 0);
		if (!pointer || n == 0) return;

		ptr::size_t deallocated_size = 0;	
		while (pointer) {
			ptr::size_t continous_len = 1;
			value_type* head = pointer;
			for (; pointer->next && pointer->next == pointer + 1; ++pointer, ++continous_len);
			pointer = pointer->next;
			allocator::deallocate(alloc, head, continous_len);
			deallocated_size += continous_len;
		}
		ccdk_assert(deallocated_size == n);
	}

	CCDK_FORCEINLINE static constexpr size_type max_allocate_size() noexcept {
		return size_type(-1) / sizeof(value_type);
	}

};



ccdk_namespace_memory_end