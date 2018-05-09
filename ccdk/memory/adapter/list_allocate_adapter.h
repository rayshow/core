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
struct is_biward_node 
	: and_< has_attribute_next<T>, has_attribute_prev<T>> {};

template<typename T>
constexpr is_biward_node<T> is_biward_node_c{};

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
	using upstream_adapter   = allocator_traits<Alloc>;
	using value_type         = typename upstream_adapter::value_type;
	using size_type          = typename upstream_adapter::size_type;
	
	using node_type = value_type;
	using link_type = value_type * ;

	template<typename U>
	using rebind = list_allocate_adapter<U>;

	/* forward list node */
	static auto __allocate(Alloc& alloc, size_type n, mpl::false_) {
		link_type head, tail;
		head = upstream_adapter::allocate(alloc, 1);
		tail = head;
		for (uint32 i = 0; i < n - 1; ++i) {
			tail->next = upstream_adapter::allocate(alloc, 1);
			tail = tail->next;
		}
		tail->next = nullptr;
		return fs::make_pair(head, tail);
	}

	/* forward list node */
	static auto __allocate(Alloc & alloc, size_type n, mpl::true_) {
		link_type head, tail, mid;
		head = upstream_adapter::allocate(alloc, 1);
		head->prev = nullptr;
		tail = head;
		for (uint32 i = 0; i < n - 1; ++i) {
			mid = upstream_adapter::allocate(alloc, 1);
			tail->next = mid;
			mid->prev = tail;
			tail = mid;
		}
		tail->next = nullptr;
		return fs::make_pair(head, tail);
	}

	/* value_type.next must be valid */
	template<typename = check_t< has_attribute_next<node_type>>>
	static auto allocate(Alloc & alloc, size_type n)
	{
		ccdk_assert(n > 0);
		if (n == 0) return fs::pair<link_type, link_type>{};
		return __allocate(alloc, n, is_biward_node_c<node_type>);
	}

	/* value_type.next must be valid */
	template<typename = check_t< has_attribute_next<node_type>>>
	static auto deallocate(Alloc &alloc, node_type* pointer, size_type n) noexcept {
		for (uint32 i = 0; i < n; ++i) {
			ccdk_assert(pointer);
			node_type* next = pointer->next;
			upstream_adapter::deallocate(alloc, pointer, 1);
			pointer = next;
		}
		return pointer;
	}

	CCDK_FORCEINLINE static constexpr size_type max_allocate_size() noexcept {
		return size_type(-1) / sizeof(node_type);
	}

};



ccdk_namespace_memory_end