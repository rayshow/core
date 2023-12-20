#pragma once

#include<ccdk/container/container_mudule.h>
#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/fusion/pair.h>
#include<ccdk/memory/allocator/simple_new_allocator.h>
#include<ccdk/container/impl/rb_tree.h>

ccdk_namespace_ct_start

using namespace mpl;

template<
	typename Key,
	typename Value,
	typename Size = uint32,
	typename Alloc = mem::simple_new_allocator<fs::pair<Key, Value>, Size>,
	typename Impl = rb_tree<false, Key, Value, fs::pair<Key, Value>, fs::get_first_t, fs::get_second_t, Size,Alloc>
>
class map: public Impl
{
	using super_type = Impl;
	using this_type  = map;
	using value_type = fs::pair<Key, Value>;
	using size_type  = Size;

public:

	using Impl::foreach;
	using Impl::print_tree;

	map() noexcept : super_type{} {}
	map(ptr::nullptr_t) noexcept : super_type{nullptr} {}

	template<typename InputIt, typename = check_t<is_iterator<InputIt>>>
	map(InputIt begin, size_type n) : super_type{ begin, n } {}

	template<uint32 N>
	map(value_type const(&arr)[N]) : map{ arr,N } {}

	void debug_value(const char* title="") {
		DebugValueItBegin(title);
		foreach([](const value_type& v) {
			DebugValueIt(v.first(), v.second());
		});
		DebugValueItEnd();
	}

};



ccdk_namespace_ct_end