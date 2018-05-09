
#include<ccdk/container/slist.h>
#include<stdio.h>
#include<list>

using namespace ccdk;
using namespace ccdk::mpl;
using namespace ccdk::ct;

struct test_mem {
	int a;
	char b[0];
};

void debug_slist()
{
	DebugValue(sizeof(test_mem));
	DebugNewTitle("ctor");
	{
		DebugSubTitle("default");
		{
			slist<int> s1{};
			slist<int> s2{ nullptr };
		}
		DebugSubTitle("fill ctor");
		{
			slist<int> s1{ 5, 1 };
			s1.debug_value();
		}
		DebugSubTitle("range/range-n ctor");
		{
			DebugValue(has_attribute_next_v<forward_node<int>>);
			slist<int> s1{ 5, 1 };
			slist<int> s2{ s1.begin(), s1.end() };
			slist<int> s3{ s1.cbegin(), s1.size() };
			s2.debug_value();
			s3.debug_value();
		}
		DebugSubTitle("copy/move ctor");
		{
			slist<int> s1{ 5, 2 };
			slist<int> s2{ s1 };
			slist<int> s3{ util::move(s1) };
			s1.debug_value();
			s2.debug_value();
			s3.debug_value();
		}
		DebugSubTitle("swap");
		{
			slist<int> s1{ 5, 2 };
			slist<int> s2{ 5,1 };
			s1.debug_value();
			s2.debug_value();
			s1.swap(s2);
			s1.debug_value();
			s2.debug_value();
		}
	}
	DebugNewTitle("assign");
	{
		DebugSubTitle("copy/move assign");
		{
			slist<int> s1{ 5, 1 };
			slist<int> s2{ 3, 2 };
			slist<int> s3{ 6, 3 };
			slist<int> s4{ 10, 4 };
			s1.debug_value();
			s1 = s2;
			s1.debug_value();
			s1 = s3;
			s1.debug_value();
			s1 = util::move(s4);
			s4.debug_value();
			s1.debug_value();
		}
		DebugSubTitle("assign fill");
		{
			slist<int> s1{ 5, 1 };
			s1.debug_value();
			s1.assign(3, 2);
			s1.debug_value();
			s1.assign(6, 3);
			s1.debug_value();
		}
	}
	DebugNewTitle("it");
	{
		slist<int> s1{ { 1,2,3,4,5,4,3,2 } };
		ccdk_assert(s1.size() == 8);
		ccdk_assert(s1.begin() == s1.cbegin());
		slist<int>::iterator bg = s1.cbegin();
		for (auto &it : s1) {
			DebugValueIt(it);
		}
	}
	DebugNewTitle("pop/push");
	{
		slist<int> s1{ { 1,2,3,4,5,4,3,2 } };
		ccdk_assert(s1.front() == 1);
		s1.pop_front();
		ccdk_assert(s1.front() == 2);
		s1.push_front(3);
		ccdk_assert(s1.front() == 3);
	}
}


void debug_cached_slist()
{
	cached_slist<int> lst{ {1,2,3,4,5,6} };
	lst.pop_front();
	lst.pop_front();
	lst.push_front(2);
	lst.push_front(1);
	DebugValue("before destruct");
}

int main()
{
	debug_slist();
	debug_cached_slist();
	getchar();
	ccdk_open_leak_check();

	return 0;
}