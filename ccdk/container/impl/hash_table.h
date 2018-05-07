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
	using this_type       = hash_iterator;
	using const_this      = hash_iterator<const T, Node, Container, Size>;
	using link_type       = Node * ;
	using value_type      = T;
	using pointer         = T *;
	using const_pointer   = T const*;
	using reference       = T & ;
	using const_reference = T const&;
	using size_type       = Size;
	using difference_type = ptr::diff_t;
	using link_range      = fs::pair<link_type, link_type>;
	using category        = forward_category;

	const Container&  buckets;
	size_type         index;
	link_type         node;

	this_type operator++(int) noexcept {
		size_type old_index = index;
		link_type old_node = node;
		++(*this);
		return { buckets, old_index, old_node };
	}

	//to next node
	this_type& operator++() noexcept {
		if (node->next) {
			node = node->next;
		}
		else {
			index = buckets.find_index(fn::not_null, index + 1);
			node = (index != buckets.size()) ?
				buckets.at(index) : nullptr;
		}
		return *this;
	}

	CCDK_FORCEINLINE reference operator*() noexcept { ccdk_assert(node); return node->data; }
	CCDK_FORCEINLINE const_reference operator*() const noexcept { ccdk_assert(node);  return node->data; }

	CCDK_FORCEINLINE bool operator==(this_type const& other) const noexcept {
		return index == other.index && 
			node == other.node &&
			buckets.data() == other.buckets.data();
	}

	//convert to const_iterator
	CCDK_FORCEINLINE operator const_this() const noexcept {
		return { buckets, index, node };
	}
};

// const_iterator == iterator
template<typename T, typename Node,typename Container,typename Size>
CCDK_FORCEINLINE bool operator==(
	hash_iterator<T,Node,Container,Size> const& left,
	hash_iterator<T const, Node, Container, Size> const& right) {
	return left.index == right.index &&
		left.node == right.node &&
		left.buckets.data() == right.buckets.data();
}

// iterator == const_iterator
template<typename T, typename Node, typename Container, typename Size>
CCDK_FORCEINLINE bool operator==(
	hash_iterator<T const, Node, Container, Size> const& left,
	hash_iterator<T, Node, Container, Size> const& right ) {
	return left.index == right.index &&
		left.node == right.node &&
		left.buckets.data() == right.buckets.data();
}

//TODO add clone
template<
	bool AllowEqualKey,
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
class hash_table: protected Alloc::template rebind<Node>
{
public:
	using this_type   = hash_table;
	using key_type    = Key;
	using node_type   = Node;
	using link_type   = node_type * ;
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
	using difference_type = ptr::diff_t;
	
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
	CCDK_FORCEINLINE Key const& KeyOfLink(link_type node) const noexcept { 
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
	CCDK_FORCEINLINE hash_table( size_type bucket_size )
		: hash_table{ mask_initialze_tag{}, next_prime_index(0,bucket_size) } {}

	//move ctor
	CCDK_FORCEINLINE hash_table(this_type&& other) noexcept
		: buckets{ util::move(other.buckets) },
		len{ other.len }, mask_index{ other.mask_index} { 
		other.len = 0;
		other.mask_index = 0;
	}

	//move assign, avoid self assign
	CCDK_FORCEINLINE void assign(this_type&& other) noexcept {
		ccdk_if_not_this(other) {
			buckets = util::move(other.buckets);
			len = other.len;
			mask_index = other.mask_index;
			other.len = 0;
			other.mask_index = 0;
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


///////////////////////////////////////////////////////////////////////////////
////  emplace / insert 

	//emplace construct with args at key
	template<
		typename... Args,
		typename = check_t< has_constructor<T,Args...>>
	>
	CCDK_FORCEINLINE auto emplace_unique(Args&&... args) {
		link_type node = new_node(util::forward<Args>(args)...);
		return insert_unique_link( KeyOfLink(node),node);
	}

	template<
		typename... Args,
		typename = check_t< has_constructor<T, Args...>>
	>
	CCDK_FORCEINLINE auto emplace_multiple(Args&&... args) {
		link_type node = new_node(util::forward<Args>(args)...);
		return insert_multiple_link(KeyOfLink(node), node);
	}

	// insert a element
	CCDK_FORCEINLINE auto insert_unique(T const& t) { 
		return emplace_at(MappingToKeyFn(t), t);
	}

	// insert a r-value element
	CCDK_FORCEINLINE auto insert_unique(T && t) { 
		return emplace_at(MappingToKeyFn(t), util::move(t));
	}

	// insert range-n
	template<
		typename InputIt,
		typename = check_t<is_iterator<InputIt>> >
	CCDK_FORCEINLINE auto insert_unique(InputIt begin, size_type n) {
		for (size_type i = 0; i < n;++i, ++begin) {
			insert_unique(*begin);
		}
	}

	// insert range
	template<
		typename InputIt, 
		typename = check_t<is_iterator<InputIt>> >
	CCDK_FORCEINLINE auto insert_unique(InputIt begin, InputIt end) {
		for (InputIt it = begin; it != end; ++it) {
			insert_unique(*it);
		}
	}

	//insert initialize_list
	CCDK_FORCEINLINE auto insert_unique(std::initializer_list<T> const& list) {
		for (auto it : list) {
			insert_unique(util::fmove(it));
		}
	}

	// insert a element
	CCDK_FORCEINLINE auto insert_multiple(T const& t) {
		link_type node = new_node(t);
		return insert_multiple_link(KeyOfLink(node), node);
	}

	// insert a r-value element
	CCDK_FORCEINLINE auto insert_multiple(T && t) {
		link_type node = new_node(util::move(t));
		return insert_multiple_link(KeyOfLink(node), node);
	}

	// insert range-n
	template<
		typename InputIt,
		typename = check_t<is_iterator<InputIt>> >
		CCDK_FORCEINLINE auto insert_multiple(InputIt begin, size_type n) {
		for (size_type i = 0; i < n; ++i, ++begin) {
			insert_multiple(*begin);
		}
	}

	// insert range
	template<
		typename InputIt,
		typename = check_t<is_iterator<InputIt>> >
		CCDK_FORCEINLINE auto insert_multiple(InputIt begin, InputIt end) {
		for (InputIt it = begin; it != end; ++it) {
			insert_multiple(*it);
		}
	}

	//insert initialize_list
	CCDK_FORCEINLINE auto insert_multiple(std::initializer_list<T> const& list) {
		for (auto it : list) {
			insert_multiple(util::fmove(it));
		}
	}

///////////////////////////////////////////////////////////////////////////////
//// erase

	//erase at Key
	CCDK_FORCEINLINE size_type erase(Key const& key) noexcept {
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
	this_type& clear() noexcept {
		size_type last_index = buckets.size() - 1;
		size_type i = buckets.find_index(fn::not_null, 0);
		for (; i <= last_index; i = buckets.find_index(fn::not_null, i))  {
			erase_list_all(i);
		}
		len = 0;
		return *this;
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

	//equal range
	CCDK_FORCEINLINE fs::pair<iterator,iterator>
		equal_range(Key const& key) noexcept {
		auto p = find_node(key);
		link_type last_link = p.second;
		while (last_link->next && 
			util::equals(KeyOfLink(last_link->next), key)) {
			last_link = last_link->next;
		}
		iterator last = { buckets, p.first, last_link };
		return { { buckets, p.first, p.second}, ++last };
	}


	// test key exist
	CCDK_FORCEINLINE bool exists(Key const& key) const {
		return find_node(key).second  != nullptr;
	}

	// readonly attribute
	CCDK_FORCEINLINE size_type size() const noexcept { return len; }
	CCDK_FORCEINLINE bool empty() const noexcept { return len==0; }
	CCDK_FORCEINLINE constexpr size_type max_size() const noexcept { return size_type(-1); }
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
	template<bool AllowEqual>
	CCDK_FORCEINLINE void rehash(size_type new_bucket_size) {
		if (new_bucket_size < buckets.size()) return;
		hash_table tmp{ 
			mask_initialze_tag{},
			next_prime_index(mask_index, new_bucket_size)
		};
		size_type old_bucket_size = bucket_size();
		for (size_type i = 0; i < old_bucket_size; ++i) {
			for (link_type node = buckets.at(i); node; node = node->next) {
				tmp.insert_unique(util::move(node->data));
			}
		}
		tmp.swap(*this);
#if defined(CCDK_PROFILE)
		conflict_count = 0;
#endif
	}

	template<>
	CCDK_FORCEINLINE void rehash<true>(size_type new_bucket_size) {
		if (new_bucket_size < buckets.size()) return;
		hash_table tmp{ 
			mask_initialze_tag{},
			next_prime_index(mask_index, new_bucket_size)
		};
		size_type old_bucket_size = bucket_size();
		for (size_type i = 0; i < old_bucket_size; ++i) {
			for (link_type node = buckets.at(i); node; node = node->next) {
				tmp.insert_multiple(util::move(node->data));
			}
		}
		tmp.swap(*this);
#if defined(CCDK_PROFILE)
		conflict_count = 0;
#endif
	}

////////////////////////////////////////////////////////////////////
//// transform

	//efficient then iterator
	template<typename Pred>
	CCDK_FORCEINLINE void foreach(Pred const& pred) const noexcept {
		size_type bks = bucket_size();
		for (size_type i = 0; i < bks; ++i) {
			for (link_type node = buckets.at(i); node; node = node->next) {
				pred(node->data);
			}
		}
	}

	//efficient than copy iterator-range
	CCDK_FORCEINLINE this_type clone() const noexcept {
		this_type ret{};
		ret.buckets.assign(buckets.size(), nullptr);
	}

//// implements 
private:
	CCDK_FORCEINLINE link_type find_head(Key const& key) noexcept {
		return buckets.at(bucket_idx(key));
	}

	// locally construct a T
	template<
		typename... Args,
		typename = check_t< has_constructor<T,Args...>>
	>
	CCDK_FORCEINLINE link_type new_node(Args&& ... args) {
		link_type node = node_allocator_type::allocate(*this, 1);
		util::construct<T>(node, util::forward<Args>(args)...);
		node->next = nullptr;
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
				link_type next_node = node->next;
				if (AllowEqualKey) {
					while (next_node && util::equals(key, KeyOfLink(next_node))) {
						link_type to_delete = next_node;
						next_node = next_node->next;
						destroy_node(to_delete);
						++count;
					}
				}
				if (prev) prev->next = next_node;
				else head = next_node;
				++count;
				destroy_node(node);
			}
		}
		return count;
	}

	// find key and it's prev node
	// if it is the first node, prev == nullptr
	// if no key found, it == nullptr
	CCDK_FORCEINLINE fs::pair<link_type, link_type>
		find_node_and_prev(key_type const& key, size_type index) noexcept {
		link_type head = buckets.at(index);
		link_type prev = nullptr;
		link_type it = head;
		for (; it && !util::equals(KeyOfLink(it), key); prev = it, it = it->next  );
		return { prev, it };
	}

	CCDK_FORCEINLINE fs::pair<size_type,link_type>
		find_node(Key const& key) const noexcept {
		size_type index = bucket_idx(key);
		link_type it = buckets.at(index);
		while (it && !util::equals(KeyOfLink(it), key)) {
			it = it->next;
		}
		//if (it) index = buckets.size();
		return { index, it };
	}

	// test weather need rehash
	CCDK_FORCEINLINE void test_rehash()  {
		//need rehash 
		if (cdiv<float>(len + 1, bucket_size()) >= MaxLoadFactor::value) {
			rehash<AllowEqualKey>(MaxLoadFactor::DivAsFactor(bucket_size()) + 1);
		}
	}

	// locally add a link from local constructed node
	// not allow multi-equal-key node
	template<typename... Args>
	CCDK_FORCEINLINE fs::pair<iterator, bool>
		insert_unique_link(key_type const& key, link_type node) {

		//weather need rehash
		test_rehash();

		//find key 
		auto p = find_node(key);
		if (p.second) {
			destroy_node(node);
			return { { buckets, p.first, p.second }, false };
		}

#if defined(CCDK_PROFILE)
		if (buckets.at(p.first)) {
			++conflict_count;
		}
#endif

		//insert at first
		node->next = buckets.at(p.first);
		buckets.at(p.first) = node;
		++len;
		return { { buckets, p.first, node }, true };
	}

	// locally add a link from local constructed
	// allow multi-equal-key node
	template<typename... Args>
	CCDK_FORCEINLINE fs::pair<iterator, bool>
	insert_multiple_link(key_type const& key, link_type node) {
		//weather need rehash
		test_rehash();
		//get buckets pos
		size_type pos = bucket_idx(key);
		//search key
		auto p = find_node_and_prev(key, pos);
		//found key 
		if (p.second && p.first) {
			node->next = p.second;
			p.first->next = node;
		}
		else {
			// not found, insert at head
			node->next = buckets.at(pos);
			buckets.at(pos) = node;
		}
		++len;
		// iterator
		return { { buckets, pos, node }, true };
	}

	// locally add a Node with args... no order
	// if key exists avoid a construct
	template<typename... Args>
	CCDK_FORCEINLINE fs::pair<iterator, bool> 
	emplace_at(key_type const& key, Args&& ... args) {

		//weather need rehash
		test_rehash();

		//find key 
		auto p = find_node(key);
		if (p.second) {
			return { {buckets, p.first, p.second }, false };
		}

#if defined(CCDK_PROFILE)
		if (buckets.at(p.first)) {
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