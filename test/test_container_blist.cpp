#include<ccdk/container/blist.h>
using namespace ccdk;
using namespace ccdk::ct;
                                                                        
int main()
{
	DebugNewTitle("test constructor ");
	{
		blist<int> lst1;
		blist<test_copy_assign_t<int>> lst2;
		blist<test_copy_assign_t<int>> lst3{ nullptr };
		RuntimeAssertTrue(lst1.empty());
	}
	DebugNewTitle("test fill constructor ");
	{
		blist<int> lst1{ 20, 1 };
		RuntimeAssertTrue(!lst1.empty());
		//RuntimeAssertTrue(lst1[19]==1);
		blist<test_copy_t> lst2{ 2 };
	}
	DebugNewTitle("test copy ctor");
	{
		blist<test_copy_t> lst1{ 2 };
		blist<test_copy_t> lst2{ lst1 };
		blist<test_copy_t> lst3{ lst1.begin(), 1 };
		blist<test_copy_t> lst4{ lst1.begin(), lst1.end() };
	}
	DebugNewTitle("test template ctor");
	{
		blist<int> lst1{ 20, 1 };
		blist<int, ptr::size_t> lst2{ lst1 };
		lst2.debug("t-copy");
	}
	DebugNewTitle("test move");
	{
		blist<int> lst1{ 20, 1 };
		blist<int> lst2{ util::move(lst1) };
		blist<int,ptr::size_t> lst3{ util::move(lst2) };
		lst3.debug("moved");
		int a = 0;
	}
	DebugNewTitle("swap");
	{
		blist<int> lst1{ 20, 1 };
		blist<int> lst2{ 20, 2 };
		util::swap(lst1, lst2);
		RuntimeAssertTrue(lst1.front() == 2);
		RuntimeAssertTrue(lst2.front() == 1);
	}
	DebugNewTitle("copy assign");
	{
		blist<int> lst1{ 20, 1 };
		blist<int> lst2{ 10, 2 };
		blist<int> lst3{ 30, 3 };
		blist<int, uint16> lst4{ 10, 2 };
		blist<int, uint16> lst5{ 30, 3 };
		lst2 = lst1;
		lst3 = lst1;
		lst4 = lst1;
		lst5 = lst1;
		RuntimeAssertTrue(lst2.back() == 1 && lst2.size()==20);
		RuntimeAssertTrue(lst3.back() == 1 && lst3.size() == 20);
		RuntimeAssertTrue(lst4.back() == 1 && lst2.size() == 20);
		RuntimeAssertTrue(lst5.back() == 1 && lst3.size() == 20);
	}
	DebugNewTitle("move assign");
	{
		blist<int> lst1{ 20, 1 };
		blist<int> lst2{ 10, 2 };
		blist<int> lst3{ 30, 3 };
		lst2 = util::move(lst1);
		lst3 = util::move(lst2);
		RuntimeAssertTrue(lst3.back() == 1 && lst3.size() == 20);
	}
	DebugNewTitle("assign fill");
	{
		blist<int> lst1{ 20, 1 };
		lst1.assign(20, 2);
		RuntimeAssertTrue(lst1.back() == 2 && lst1.size() == 20);
		lst1.assign(10, 3);
		RuntimeAssertTrue(lst1.back() == 3 && lst1.size() == 10);
		lst1.assign(30, 4);
		RuntimeAssertTrue(lst1.back() == 4 && lst1.size() == 30);
	}
	DebugNewTitle("assign range");
	{
		blist<int> lst1{ 20, 1 };
		blist<int> lst2{ 10, 2 };
		blist<int> lst3{ 25, 3 };
		lst1.assign(lst2.begin(), lst2.size());
		RuntimeAssertTrue(lst1.back() == 2 && lst1.size() == 10);
		lst1.assign(lst3.begin(), lst3.size());
		RuntimeAssertTrue(lst1.back() == 3 && lst1.size() == 25);
	}
	DebugNewTitle("initialize / front /back");
	{
		const blist<int> lst1{ { 1,2,3,4,5,6,7 } };
		RuntimeAssertTrue(lst1.front() == 1);
		RuntimeAssertTrue(lst1.back() == 7);
		lst1.cdebug("test cit:");
	}

	_CrtDumpMemoryLeaks();
	getchar();
	return 0;
}