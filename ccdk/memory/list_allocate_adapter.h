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
	typedef allocator_traits<Alloc> upstream_allocator;
	typedef typename upstream_allocator::value_type value_type;
	typedef typename upstream_allocator::size_type  size_type;
	typedef value_type  node_type;

	template<typename U>
	using rebind = list_allocate_adapter<U>;

	/* forward list node */
	static auto __allocate(Alloc const& alloc, ptr::size_t n, false_) {

		node_type *head, *tail;
		head = upstream_allocator::allocate(alloc, 1);
		tail = head;
		for (int i = 0; i < n - 1; ++i) {
			tail->next = upstream_allocator::allocate(alloc, 1);
			tail = tail->next;
		}
		tail->next = nullptr;
		return fs::make_pair(head, tail);
	}

	/* forward list node */
	static auto __allocate(Alloc const& alloc, ptr::size_t n, true_) {

		node_type *head, *tail, *mid;
		head = allocator::allocate(alloc, 1);
		head->prev = nullptr;
		tail = head;
		for (int i = 0; i < n - 1; ++i) {
			mid = upstream_allocator::allocate(alloc, 1);
			tail->next = mid;
			mid->prev = tail;
			tail = mid;
		}
		tail->next = nullptr;
		return fs::make_pair(head, tail);
	}

	/* value_type.next must be valid */
	template<typename = check_t< is_forward_node<node_type>>>
	static auto allocate(Alloc const& alloc, ptr::size_t n)
	{
		ccdk_assert(n > 0);
		if (n == 0) return nullptr;
		return __allocate(alloc, n, is_biward_node_c<node_type>);
	}

	static auto reset_prev(node_type* pointer, false_) {};
	static auto reset_prev(node_type* pointer, true_) { pointer->prev = nullptr; };

	/* value_type.next must be valid */
	template<typename = check_t< is_forward_node<node_type>>>
	static auto deallocate(Alloc const& alloc, node_type* pointer, ptr::size_t n) noexcept {
		ccdk_assert(pointer && n > 0);
		if (!pointer || n == 0) return;
		node_type* curr = pointer;
		for (int i = 0; i < n; ++i) {
			pointer = pointer->next;
			upstream_allocator::deallocate(alloc, curr, 1);
		}
		reset_prev(pointer, is_biward_node_c<node_type>) 
		return pointer;
	}

	CCDK_FORCEINLINE static constexpr size_type max_allocate_size() noexcept {
		return size_type(-1) / sizeof(node_type);
	}

};



ccdk_namespace_memory_end