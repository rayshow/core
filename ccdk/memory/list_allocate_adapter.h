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
	static auto __allocate(Alloc const& alloc, size_type n, mpl::false_) {

		node_type *head, *tail;
		head = upstream_allocator::allocate(alloc, 1);
		tail = head;
		for (uint32 i = 0; i < n - 1; ++i) {
			tail->next = upstream_allocator::allocate(alloc, 1);
			tail = tail->next;
		}
		tail->next = nullptr;
		return fs::make_pair(head, tail);
	}

	/* forward list node */
	static auto __allocate(Alloc & alloc, size_type n, mpl::true_) {

		node_type *head, *tail, *mid;
		head = upstream_allocator::allocate(alloc, 1);
		head->prev = nullptr;
		tail = head;
		for (uint32 i = 0; i < n - 1; ++i) {
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
	static auto allocate(Alloc & alloc, size_type n)
	{
		ccdk_assert(n > 0);
		if (n == 0) return fs::pair<value_type*, value_type*>{};
		return __allocate(alloc, n, is_biward_node_c<node_type>);
	}

	static auto reset_prev(node_type* pointer, false_) {};
	static auto reset_prev(node_type* pointer, true_) { pointer->prev = nullptr; };

	/* value_type.next must be valid */
	template<typename = check_t< is_forward_node<node_type>>>
	static auto deallocate(Alloc &alloc, node_type* pointer, size_type n) noexcept {
		if (pointer && n > 0) {
			node_type* curr = pointer;
			for (uint32 i = 0; i < n; ++i) {
				node_type* next = pointer->next;
				upstream_allocator::deallocate(alloc, curr, 1);
				pointer = next;
			}
			reset_prev(pointer, is_biward_node_c<node_type>);
		}
		return pointer;
	}

	CCDK_FORCEINLINE static constexpr size_type max_allocate_size() noexcept {
		return size_type(-1) / sizeof(node_type);
	}

};



ccdk_namespace_memory_end