#pragma once

#include<ccdk/mpl/fusion/local_obj.h>
#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/mcontainer/val_pack.h>
#include<ccdk/mpl/iterator/reverse_iterator.h>
#include<ccdk/mpl/util/move.h>
#include<ccdk/mpl/util/construct.h>
#include<ccdk/mpl/function/operator.h>
#include<ccdk/mpl/fusion/local_obj.h>
#include<ccdk/memory/allocator_traits.h>
#include<ccdk/memory/simple_new_allocator.h>

#include<ccdk/container/impl/link_node.h>
#include<ccdk/container/vector.h>
#include<ccdk/container/lazy_slist.h>

ccdk_namespace_ct_start

using namespace mpl;

// just call util::cmp

template<typename T>
struct default_cmp {

};

template<typename T>
struct default_to_key {
	
};

template<typename T>
struct default_hash {
	CCDK_FORCEINLINE operator()() {
		
	}
};

constexpr static uint32 kNumPrime = 28;
constexpr static uint64 kPrimeArray[kNumPrime] =
{
	53UL,         97UL,         193UL,       389UL,       769UL,
	1543UL,       3079UL,       6151UL,      12289UL,     24593UL,
	49157UL,      98317UL,      196613UL,    393241UL,    786433UL,
	1572869UL,    3145739UL,    6291469UL,   12582917UL,  25165643UL,
	50331653UL,   100663319UL,  201326611UL, 402653189UL, 805306457UL,
	1610612741UL, 3221225473UL, 4294967291UL
};

template<
	typename Key,
	typename T,
	typename HashFn = default_hash<T>,
	typename MapKeyFn = default_to_key<T>,
	typename CmpFn = default_cmp<T>,
	typename Size = uint32,
	typename Alloc = mem::simple_new_allocator<T>,
>
class hash_table: public Alloc
{
	using this_type        = hash_table;
	using node_type        = forward_node<T>;
	using link_type        = node_type * ;
	using key_type         = Key;
	using node_type        = T;
	using bucket_container = vector<link_type, units::ratio<2, 1>, Size, typename Alloc::rebind<link_type>>;
	using allocator_type   = allocator_traits<Alloc>;

	using size_type = Size;
	using diff_type = ptr::diff_t;

private:
	static constexpr CmpFn     KeyCmp{};
	static constexpr MapKeyFn  MappingToKey{};

	bucket_container buckets;      // buckets 
	size_type        len;          // elements size
	HashFn           HashFn;       // hash function obj
	uint8            mask_index;   // mask with hash result

#if  defined(CCDK_DEBUG)
	uint32           conflict_count;
#endif

	// mapping key to bucket index
	CCDK_FORCEINLINE size_type bucket_idx(Key const& key) {
		return (size_type)HashFn(key) & (size_type)(kPrimeArray[mask_index] - 1);
	}

	CCDK_FORCEINLINE Key const& KeyOfLink(link_type node) { return MappingToKey(node->value); }

public:

	//default
	CCDK_FORCEINLINE hash_table() 
		: buckets{ kPrimeArray[0], nullptr }, len{ 0 }, HashFn{}, mask_index{0} {}
	CCDK_FORCEINLINE hash_table(ptr::nullptr_t) 
		: buckets{ kPrimeArray[0], nullptr }, len{ 0 }, mask_index{0} {}
	
	//construct at key
	template<bool AllowEqual, typename... Args>
	CCDK_FORCEINLINE void emplace(Key const& key, Args&&... args) {
		emplace_at<AllowEqual>(
			bucket_idx(key), 
			new_node(util::forward<Args>(args)...));
	}

	CCDK_FORCEINLINE this_type& erase(Key const& key) noexcept {
		return erase_at(bucket_idx(key), key);
	}

	CCDK_FORCEINLINE size_type size() { return len; }
	CCDK_FORCEINLINE size_type bucket_size() { return buckets.size(); }

//// implements 
private:
	// locally construct a T
	template<typename... Args>
	CCDK_FORCEINLINE link_type new_node(Args&& ... args) {
		link_type node = allocator_type::allocate(*this, 1);
		util::construct<T>(node, util::forward<Args>(args)...);
		return node;
	}

	// destruct node and deallocate it's memory
	CCDK_FORCEINLINE void destroy_node(link_type node) noexcept {
		util::destruct<T>(node);
		allocator_type::deallocate(*this, 1, node);
	}

	CCDK_FORCEINLINE void erase_at(size_type index, Key const& key) {
		ccdk_assert(index < bucket_size());
		//get bucket list head
		link_type& head = buckets.at(index);
		if (head) {
			//the only node
			if (head->next == nullptr) {
				destroy_node(head);
				head = nullptr;
			}
			else {
				link_type node = head;
				link_type prev = nullptr;
				while (node && KeyCmp( key, KeyOfLink(node) )) {
					prev = node;
					node = node->next;
				}
				
				if (node && !KeyCmp(KeyOfLink(node), key)) {
					if (prev) {
						prev->next = node->next;
					}
					else {
						head = node->next;
					}
				}
				destroy_node(node);
			}
		}
	}

	// locally add a T with index
	template<bool AllowEqual, typename... Args>
	CCDK_FORCEINLINE void emplace_at(size_type index,link_node new_node) {
		ccdk_assert(index < bucket_size());
		link_type head = buckets.at(index);
		if (head) {
			//conflict, try append to list
#if defined(CCDK_DEBUG)
			++conflict_count;
#endif
			try_append(head, new_node));
		}
		else {
			// empty position, 
			head = new_node;
		}
	}

	// allow equal value
	template<bool AllowEqual>
	void try_append(link_type head, link_type node) {
		
		link_type prev = nullptr;
		//gerater than head, move to next
		while (head != nullptr && KeyCmp( KeyOfLink(node), KeyOfLink(head) )) {
			prev = head;
			head = head->next;
		}
		if (head) {
			node->next = head;
		}
		prev->next = node;
	}

	CCDK_FORCEINLINE fs::pair<link_type, link_type> find_node(link_type head, link_typo node) noexcept {
		link_type prev = nullptr;
		//gerater than head, move to next
		while (head != nullptr && KeyCmp( KeyOfLink(node), KeyOfLink(head) )) {
			//node is equal with head, destroy node and return 
			prev = head;
			head = head->next;
		}
		return { prev, head };
	}


	CCDK_FORCEINLINE link_type link_low_bound(link_type head, Key const& key) noexcept {
		link_type prev = nullptr;

	}

};


ccdk_namespace_ct_end