#pragma once

#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/base/compatible_op.h>
#include<ccdk/mpl/util/move.h>
#include<ccdk/mpl/util/construct.h>
#include<ccdk/mpl/util/hash.h>
#include<ccdk/mpl/util/equals.h>
#include<ccdk/mpl/function/prediction.h>
#include<ccdk/mpl/fusion/local_obj.h>
#include<ccdk/memory/allocator_traits.h>
#include<ccdk/memory/simple_new_allocator.h>

#include<ccdk/container/impl/link_node.h>
#include<ccdk/container/vector.h>

ccdk_namespace_ct_start

using namespace mpl;

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

inline constexpr uint8 next_prime_index(uint8 prev_index, uint64 hint) noexcept {
	for (uint32 i = prev_index; i < kNumPrime; ++i) {
		if (kPrimeArray[i] > hint) return i;
	}
	ccdk_assert(0);
	return kNumPrime + 1;
}

template<
	typename T,
	typename Node,
	typename Container,
	typename Size
>
struct hash_iterator
{
	using this_type          = hash_iterator;
	using link_type          = Node * ;
	using value_type         = T;
	using pointer_type       = T *;
	using const_pointer_type = T const*;
	using reference          = T & ;
	using const_reference    = T const&;
	using size_type          = Size;
	using difference_type    = ptr::diff_t;
	using link_range         = fs::pair<link_type, link_type>;
	using category           = forward_category;

	Container&  buckets;
	size_type   index;
	link_type   node;

	this_type operator++(int) noexcept {
		size_type old_index = index;
		link_type old_node = node;
		++(*this);
		return { buckets, old_index, old_node };
	}

	//to next node
	this_type& operator++() noexcept {
		if (node->next) node = node->next;
		index = buckets.find_index(fn::not_null);
		node = index == buckets.size() ?
			buckets.at(index) : nullptr;
		return *this;
	}

	CCDK_FORCEINLINE reference operator*() noexcept { return *node;}
	CCDK_FORCEINLINE const_reference operator*() const noexcept { return *node; }

	CCDK_FORCEINLINE bool operator==(this_type const& other) const noexcept {
		return index == other.index && node == other.node;
	}
	CCDK_FORCEINLINE bool operator!=(this_type const& other) const noexcept {
		return index != other.index || node != other.node;
	}
};

template<
	typename Key,                                 // key type
	typename MappedType,                          // mapped type
	typename T,                                   // compose type
	typename ExactKeyFn,                          // exact key from T
	typename ExactValueFn,                        // exact value from T
	typename MaxLoadFactor = units::ratio<1,2>,   // over 0.5 load factor will rehash 
	typename Size = uint32,                       // size_type
	typename Alloc = mem::simple_new_allocator<T>,
	typename Node = forward_node<T>
>
class hash_table: public Alloc::template rebind<Node>
{
public:
	using this_type = hash_table;
	using key_type = Key;
	using node_type = Node;
	using link_type = node_type * ;
	using mapped_type = MappedType;
	using bucket_container = vector<link_type, units::ratio<1,1>, Size,
		typename Alloc::template rebind<link_type> >;
	using node_allocator_type = mem::allocator_traits<
		typename Alloc::template rebind<node_type>>;

	using value_type      = T;
	using pointer         = T * ;
	using const_pointer   = T const*;
	using reference       = T & ;
	using const_reference = T const&;
	using size_type       = Size;
	using diff_type       = ptr::diff_t;
	
	// iterator
	using iterator       = hash_iterator<T, node_type, bucket_container, size_type>;
	using const_iterator = hash_iterator<const T, node_type, bucket_container, size_type>;

private:
	static constexpr ExactKeyFn    MappingToKeyFn{};
	static constexpr ExactValueFn  MappingToValue{};

	bucket_container buckets;      // buckets 
	size_type        len;          // elements size
	uint8            mask_index;   // mask with hash result

	// mapping key to bucket index
	CCDK_FORCEINLINE size_type bucket_idx(Key const& key) const noexcept {
		return (size_type)util::hash(key) % (size_type)(kPrimeArray[mask_index]);
	}

	// get link_type's key
	CCDK_FORCEINLINE Key const& KeyOfLink(link_type node) noexcept { 
		return MappingToKeyFn(node->data); 
	}

	//helper ctor, with defined bucket size
	struct mask_initialze_tag{};
	CCDK_FORCEINLINE hash_table(mask_initialze_tag, uint8 index)
		: buckets{ (size_type)kPrimeArray[index], nullptr },
		len{ 0 }, mask_index{ index } {}

public:

#if  defined(CCDK_PROFILE)
	uint16           conflict_count;
#endif

	//de-ctor
	CCDK_FORCEINLINE ~hash_table() { if (len) { clear(); mask_index = 0; } }

	//default and nullptr ctor
	CCDK_FORCEINLINE hash_table() 
		: buckets{ (size_type)kPrimeArray[0], nullptr }, len{ 0 }, mask_index{0} {}
	CCDK_FORCEINLINE hash_table(ptr::nullptr_t) 
		: buckets{ (size_type)kPrimeArray[0], nullptr }, len{ 0 }, mask_index{0} {}

	//initialize with defined bucket size
	CCDK_FORCEINLINE hash_table(uint8 prev_index, size_type bucket_size)
		: hash_table{ mask_initialze_tag{}, next_prime_index(prev_index,bucket_size) } {}

	//range-n-ctor
	template<typename InputIt, typename = check_t<is_iterator<InputIt>>>
	CCDK_FORCEINLINE hash_table(InputIt begin, size_type n)
		: buckets{}, len{ 0 }, mask_index{ next_prime_index(0, n) } {
		buckets.assign(kPrimeArray[mask_index], nullptr);
		insert(begin, n);
	}

	//range-ctor
	template<typename InputIt, typename = check_t<is_iterator<InputIt>>>
	CCDK_FORCEINLINE hash_table(InputIt begin, InputIt end)
		:hash_table{ begin, it::distance(begin, end) } {}

	//copy
	CCDK_FORCEINLINE hash_table(this_type const& other)
		: hash_table{ other.begin(), other.size() }{}
	
	//move
	CCDK_FORCEINLINE hash_table(hash_table && other) :
		buckets{ util::move(other.buckets) }, len{ other.len },
		mask_index{ other.mask_index } {
		other.len = 0;
		other.mask_index = 0;
	}

	//TODO, array initialize
	CCDK_FORCEINLINE this_type& operator=(this_type& other) {
		this->clear();
		for (size_type i = 0; i < other.bucket_size(); ++i) {
			for (link_type node = buckets.at(i); node; node = node->next) {
				this->insert(node->value);
			}
		}
	}

	CCDK_FORCEINLINE this_type& operator=(this_type&& other) {
		buckets.swap(other.buckets);
		len = other.len;
		mask_index = other.mask_index;
		other.len = 0;
		other.mask_index = 0;
	}

	template<typename InputIt>
	CCDK_FORCEINLINE this_type& assign(InputIt begin, size_type n) {
		this->clear();
		for (size_type i = 0; i < n;++i, ++begin) {
			insert(*begin);
		}
	}

	template<typename InputIt>
	CCDK_FORCEINLINE this_type& assign(InputIt begin, InputIt end) {
		this->clear();
		for (InputIt it = begin; it != end; ++it) {
			insert(*it);
		}
	}

	//index 
	CCDK_FORCEINLINE mapped_type& operator[](key_type const& key){ return at(key); }
	CCDK_FORCEINLINE mapped_type& at(key_type const& key) {
		size_type index = bucket_idx(key);
		auto p = find_node_and_prev(key, index);
		if (p.second) {
			//find key
			return MappingToValue(p.second->data);
		}
		//not found insert new one
		link_type node = new_node();
		node->next = buckets.at(index);
		buckets.at(index) = node;
		return MappingToValue(node->data);
	}

	//swap with other hash table
	CCDK_FORCEINLINE void swap(this_type& other) noexcept {
		buckets.swap(other.buckets);
		util::swap(len, other.len);
		util::swap(mask_index, other.mask_index);
	}

	//emplace construct with args at key
	template<typename... Args>
	CCDK_FORCEINLINE auto emplace(Key const& key, Args&&... args) {
		return insert_link( key, new_node(util::forward<Args>(args)...));
	}

	// insert a element
	CCDK_FORCEINLINE auto insert(T const& t) { 
		return emplace_at(MappingToKeyFn(t), t);
	}
	CCDK_FORCEINLINE auto insert(T && t) { 
		return emplace_at(MappingToKeyFn(t), util::move(t));
	}
	// insert range-n
	template<typename InputIt, typename = check_t<is_iterator<InputIt>> >
	CCDK_FORCEINLINE auto insert(InputIt begin, size_type n) {
		for (size_type i = 0; i < n;++i, ++begin) {
			insert(*begin);
		}
	}

	// insert range
	template<typename InputIt, typename = check_t<is_iterator<InputIt>> >
	CCDK_FORCEINLINE auto insert(InputIt begin, InputIt end) {
		for (InputIt it = begin; it != end; ++it) {
			insert(*it);
		}
	}

	//erase at Key
	CCDK_FORCEINLINE auto erase(Key const& key) noexcept {
		return erase_at(bucket_idx(key), key);
	}

	//erase iterator
	iterator erase(const_iterator it) noexcept {
		size_type index = it.index;
		link_type node = it.node;
		link_type li = buckets.at(index);
		ccdk_assert(li);
		for (; li->next != node; li = li->next)
			ccdk_assert(li);
		li->next = node->next;
		return ++iterator{buckets, index, node};
	}

	//erase range
	iterator erase(const_iterator begin, const_iterator end) noexcept {
		size_type index = begin.index;
		for (const_iterator it = begin; it != end; 
			it = { buckets, index, buckets.at(index) } ) {
			//erase total list_list between [it.node, end)
			link_type end_node = nullptr;
			if (it.index == end.index) end_node = end->node;
			erase_list_range(it.index, it.node, end_node);
			index = buckets.find_index(it.index, fn::not_null);
		}
		return { buckets, end.index, end.node };
	}

	//erase total list and set all head to null
	void clear() noexcept {
		size_type last_index = buckets.size() - 1;
		size_type i = buckets.find_index(fn::not_null, 0);
		for (; i <= last_index; i = buckets.find_index(fn::not_null, i))  {
			erase_list_all(i);
		}
		len = 0;
	}

	//find 
	CCDK_FORCEINLINE iterator find(Key const& key) noexcept {
		auto p = find_node( key );
		return { buckets, p.first, p.second };
	}

	//const find 
	CCDK_FORCEINLINE const_iterator find(Key const& key) const noexcept {
		auto p = find_node(key);
		return { buckets, p.first, p.second };
	}

	// test key exist
	CCDK_FORCEINLINE bool exist(Key const& key) {
		return find_node(key).second != nullptr;
	}

	// readonly attribute
	CCDK_FORCEINLINE size_type size() const noexcept { return len; }
	CCDK_FORCEINLINE bool empty() const noexcept { return len!=0; }
	CCDK_FORCEINLINE size_type max_size() const noexcept { return buckets.max_size(); }
	CCDK_FORCEINLINE size_type bucket_size() const noexcept { return buckets.size(); }
	CCDK_FORCEINLINE float load_factor() const noexcept { 
		return cdiv<float>(len, bucket_size()); 
	}

	// iterator
	CCDK_FORCEINLINE iterator begin() noexcept { 
		size_type index = buckets.find_index(fn::not_null);
		return{ buckets, index, buckets.at(index) };
	}

	CCDK_FORCEINLINE iterator end() noexcept {
		return{ buckets, buckets.size(), nullptr };
	}

	CCDK_FORCEINLINE const_iterator cbegin() const noexcept {
		size_type index = buckets.find_index(fn::not_null);
		return{ buckets, index, buckets.at(index) };
	}

	CCDK_FORCEINLINE const_iterator cend() const noexcept {
		return{ buckets, buckets.size(), nullptr };
	}

	//resize bucket container to a large size, and reinsert 
	CCDK_FORCEINLINE void rehash(size_type new_bucket_size) {
		if (new_bucket_size < buckets.size()) return;
		hash_table tmp{ mask_index, new_bucket_size };
		size_type old_bucket_size = bucket_size();
		for (size_type i = 0; i < old_bucket_size; ++i) {
			for (link_type node = buckets.at(i); node; node = node->next) {
				tmp.emplace(MappingToKeyFn(node->data), util::move(node->data));
			}
		}
		tmp.swap(*this);
	}

//// implements 
private:
	CCDK_FORCEINLINE link_type find_head(Key const& key) noexcept {
		return buckets.at(bucket_idx(key));
	}

	// locally construct a T
	template<typename... Args>
	CCDK_FORCEINLINE link_type new_node(Args&& ... args) {
		link_type node = node_allocator_type::allocate(*this, 1);
		util::construct<T>(node, util::forward<Args>(args)...);
		return node;
	}

	// destruct node and deallocate it's memory
	CCDK_FORCEINLINE void destroy_node(link_type node) noexcept {
		util::destruct<T>(node);
		node_allocator_type::deallocate(*this, node, 1);
	}

	//erase node at bucket[index] 
	CCDK_FORCEINLINE size_type erase_at(size_type index, Key const& key) {
		ccdk_assert(index < bucket_size());
		//get bucket list head
		link_type& head = buckets.at(index);
		size_type count = 0;
		if (head) {
			link_type prev = nullptr;
			link_type node = head;
			while (node && !util::equals( key, KeyOfLink(node))) {
				prev = node;
				node = node->next;
			}
			if (node) {
				if (prev) prev->next = node->next;
				else head = node->next;
				count = 1;
				destroy_node(node);
			}
		}
		return count;
	}

	CCDK_FORCEINLINE fs::pair<link_type, link_type>
		find_node_and_prev(key_type const& key, size_type index) noexcept {
		link_type head = buckets.at(index);
		link_type prev = nullptr;
		link_type it = head;
		for (; it && !util::equals(KeyOfLink(it), key); prev = it, it = it->next  );
		return { prev, it };
	}

	CCDK_FORCEINLINE fs::pair<size_type,link_type>
		find_node(Key const& key) noexcept {
		size_type index = bucket_idx(key);
		link_type it = buckets.at(index);
		while (it && !util::equals(KeyOfLink(it), key)) {
			it = it->next;
		}
		//if (it) index = buckets.size();
		return { index, it };
	}

	// locally add a link from local constructed
	template<typename... Args>
	CCDK_FORCEINLINE fs::pair<iterator, bool>
		insert_link(key_type const& key, link_type node) {

		//need rehash 
		if (cdiv<float>(len + 1, bucket_size()) >= MaxLoadFactor::value) {
			rehash(MaxLoadFactor::DivAsFactor(bucket_size()) + 1);
		}

		//find key 
		auto p = find_node(key);
		if (p.second) {
			destroy_node(node);
			return { { buckets, p.first, p.second }, false };
		}

#if defined(CCDK_PROFILE)
		if (buckets.at(p.first))
		{
			++conflict_count;
		}
#endif

		//insert at first
		node->next = buckets.at(p.first);
		buckets.at(p.first) = node;
		++len;

		return { { buckets, p.first, node }, true };
	}
	
	// locally add a Node with args...
	template<typename... Args>
	CCDK_FORCEINLINE fs::pair<iterator, bool> 
		emplace_at(key_type const& key, Args&& ... args) {

		//need rehash 
		if (cdiv<float>(len + 1, bucket_size()) >= MaxLoadFactor::value) {
			rehash(MaxLoadFactor::DivAsFactor(bucket_size()) + 1);
		}

		//find key 
		auto p = find_node(key);
		if (p.second) {
			return { {buckets, p.first, p.second }, false };
		}

#if defined(CCDK_PROFILE)
		if (buckets.at(p.first))
		{
			++conflict_count;
		}
#endif
		// not found, insert at head
		ccdk_assert(p.first < buckets.size());
		link_type node = new_node(util::forward<Args>(args)...);
		node->next = buckets.at(p.first);
		buckets.at(p.first) = node;
		++len;

		return { {buckets, p.first, node }, true };
	}

	CCDK_FORCEINLINE link_type find_prev_node(link_type head, link_type node) noexcept {
		link_type it = head;
		ccdk_assert(it);
		for (;it->next != node && it; it = it->next);
		return it;
	}

	CCDK_FORCEINLINE void erase_list_all(size_type index) noexcept {
		link_type head = buckets.at(index);
		ccdk_assert(head);
		link_type next = nullptr;
		for (link_type it = head; it; it = next) {
			next = it->next;
			destroy_node(it);
		}
		buckets.at(index) = nullptr;
	}

	CCDK_FORCEINLINE void erase_list_range(size_type index, 
		link_type begin, link_type end) noexcept {
		link_type head = buckets.at(index);
		link_type it = begin;
		ccdk_assert(head && begin);

		if (begin == head && end == nullptr ) {
			erase_list_all(head);
		}
		else {
			for (; it->next!=end; it = it->next) {
				ccdk_assert(it);
				destroy_node(it);
			}
			if (begin == head) {
				head = end;
			}
			else {
				link_type prev = find_prev_node(head, it);
				ccdk_assert(prev);
				prev->next = end;
			}
		}
	}
}; 


ccdk_namespace_ct_end