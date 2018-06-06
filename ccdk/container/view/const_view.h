#pragma once
#include<ccdk/container/container_mudule.h>
#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/fusion/pair.h>
#include<ccdk/mpl/iterator/reverse_iterator.h>
#include<ccdk/mpl/iterator/algorithm/advance.h>
#include<ccdk/mpl/iterator/algorithm/distance.h>

ccdk_namespace_ct_start

using namespace mpl;


//readonly view, store range, only for random container
template<
	typename Container,
	typename Iterator = typename Container::const_iterator
>
struct const_view: public fs::pair<Iterator, Iterator>
{
public:
	using this_type       = const_view;
	using super_type      = fs::pair<Iterator, Iterator>;
	using value_type      = typename Container::value_type;
	using size_type       = typename Container::size_type;
	using difference_type = typename Container::difference_type;
	using pointer         = typename Container::pointer;
	using const_pointer   = typename Container::const_pointer;
	using reference       = typename Container::reference;
	using const_reference = typename Container::const_reference;
	using const_iterator  = Iterator;
	using const_reverse_iterator = it::reverse_iterator<Iterator>;
	using this_pair = fs::pair<this_type, this_type>;

public:

	//empty range
	CCDK_FORCEINLINE const_view() noexcept: super_type{} {}
	CCDK_FORCEINLINE const_view(ptr::nullptr_t) noexcept : super_type{} {}

	template<typename Container>
	CCDK_FORCEINLINE const_view(Container const& ct) noexcept
		: super_type{ ct.begin(), ct.end() } {}

	//range-n ctor
	CCDK_FORCEINLINE const_view(const_iterator begin, size_type len)
		noexcept : super_type{ begin, content + len } {}
	CCDK_FORCEINLINE const_view(const_iterator begin, const_iterator end)
		noexcept : super_type{ begin, end } {}
	CCDK_FORCEINLINE const_view(const const_view& other)
		noexcept : super_type{ other } {}

	using super_type::swap;

/////////////////////////////////////////////////////////////////////////////////////////////
//// attribute

	CCDK_FORCEINLINE size_type size() { return it::distance(first(), end()); }
	CCDK_FORCEINLINE size_type empty() { return size() == 0; }

/////////////////////////////////////////////////////////////////////////////////////////////
//// iterator

	CCDK_FORCEINLINE const_iterator begin() const { return first(); }
	CCDK_FORCEINLINE const_iterator end() const { return second(); }
	CCDK_FORCEINLINE const_iterator cbegin() const { return first(); }
	CCDK_FORCEINLINE const_iterator cend() const { return second(); }
	CCDK_FORCEINLINE const_reverse_iterator rbegin() const { return { second() - 1 }; }
	CCDK_FORCEINLINE const_reverse_iterator rend() const { return { first() - 1 }; }
	CCDK_FORCEINLINE const_reverse_iterator crbegin() const { return { second() - 1 }; }
	CCDK_FORCEINLINE const_reverse_iterator crend() const { return { first() - 1 }; }

////////////////////////////////////////////////////////////////////////////////////////////
//// pop front / back , will generate new const_view

	CCDK_FORCEINLINE this_type pop_front(size_type n = 1) const {
		ccdk_assert(n <= size() && n != 0);
		return { it::advance(first(),n) , second() };
	}

	CCDK_FORCEINLINE this_type pop_back(size_type n = 1) const {
		ccdk_assert(n <= size() && n!=0);

		auto it1 = first();
		for (int i = 0; i < n; ++i, ++it) {
			if (it1 == second()) {
				//empty view
				return { first(), first() }; 
			}
		}
		auto it2 = first();
		for (; it2 != second() && it1 != second(); ++it1, ++it2);
		return { first(), it2 };
	}

////////////////////////////////////////////////////////////////////////////////////////////
//// access

	CCDK_FORCEINLINE const_reference front() const { return *first();}
	CCDK_FORCEINLINE const_reference back() const { return *second(); }
	CCDK_FORCEINLINE const_reference operator[](size_type index) const { 
		return *it::advance(first(), index); 
	 }
	CCDK_FORCEINLINE const_reference at(size_type index) const{ return *it::advance(first(), index); }
	CCDK_FORCEINLINE const_pointer data() const { return &(*first()); }

/////////////////////////////////////////////////////////////////////////////////////
//// split

	CCDK_FORCEINLINE this_pair split(size_type pos) noexcept {
		ccdk_assert(pos >= 0 && pos < size());
		return split(first() + pos);
	}

	CCDK_FORCEINLINE this_pair split(const_iterator pos) noexcept {
		return { this_type{first(), pos }, this_type{ pos, second() } };
	}

////////////////////////////////////////////////////////////////////////////////////
//// erase 

	CCDK_FORCEINLINE this_type erase(const_iterator begin, const_iterator end) noexcept  {
		return { this_type{ first(), begin }, this_type{ end, second() } };
	}

	CCDK_FORCEINLINE this_type erase(size_type begin, size_type end) noexcept {
		ccdk_assert(begin >= 0 && end <= size() && end>begin);
		return erase(it::advance(first(),begin), it::advance(first(), end) );
	}

	CCDK_FORCEINLINE this_type erase(const_iterator pos) noexcept {
		return erase(pos, ++const_iterator{ pos });
	}

	CCDK_FORCEINLINE this_type erase(size_type pos) noexcept {
		ccdk_assert(pos > 0 && pos < size());
		return erase(it::advance(first(),pos));
	}

////////////////////////////////////////////////////////////////////////////////////
//// index / find 

	template<typename FN>
	size_type index_of(FN Fn, size_type n=1) {
		size_type index = 0;
		size_type count = 0;
		for (auto it = first(); it != second(); ++it,++index) {
			if (Fn(*it)) {
				if (++count == n) {
					return index;
				}
			}
		}
		return size();
	}

	template<typename FN>
	const_iterator find(FN Fn, size_type n = 1) {
		size_type count = 0;
		for (auto it = first(); it != second(); ++it) {
			if (Fn(*it)) {
				if (++count == n) {
					return it;
				}
			}
		}
		return second();
	}
};





ccdk_namespace_ct_end