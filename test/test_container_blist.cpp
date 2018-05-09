#include<ccdk/container/list.h>
using namespace ccdk;
using namespace ccdk::ct;
                                                                        
int main()
{
	DebugNewTitle("test constructor ");
	{
		list<int> lst1;
		list<test_copy_assign_t<int>> lst2;
		list<test_copy_assign_t<int>> lst3{ nullptr };
		RuntimeAssertTrue(lst1.empty());
	}
	DebugNewTitle("test fill constructor ");
	{
		list<int> lst1{ 20, 1 };
		RuntimeAssertTrue(!lst1.empty());
		//RuntimeAssertTrue(lst1[19]==1);
		list<test_copy_t> lst2{ 2 };
	}
	DebugNewTitle("test copy ctor");
	{
		list<test_copy_t> lst1{ 2, test_copy_t{4} };
		list<test_copy_t> lst2{ lst1 };
		lst1.debug("1:");
		lst2.debug("2:");
	}
	DebugNewTitle("test template ctor");
	{
		list<int> lst1{ 20, 1 };
		list<int, ptr::size_t> lst2{ lst1 };
		lst2.debug("t-copy");
	}
	DebugNewTitle("test move");
	{
		list<int> lst1{ 20, 1 };
		list<int> lst2{ util::move(lst1) };
		list<int,ptr::size_t> lst3{ util::move(lst2) };
		lst3.debug("moved");
		int a = 0;
	}
	DebugNewTitle("swap");
	{
		list<int> lst1{ 20, 1 };
		list<int> lst2{ 20, 2 };
		util::swap(lst1, lst2);
		RuntimeAssertTrue(lst1.front() == 2);
		RuntimeAssertTrue(lst2.front() == 1);
	}
	DebugNewTitle("copy assign");
	{
		list<int> lst1{ 20, 1 };
		list<int> lst2{ 10, 2 };
		list<int> lst3{ 30, 3 };
		list<int, uint16> lst4{ 10, 2 };
		list<int, uint16> lst5{ 30, 3 };
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
		list<int> lst1{ 20, 1 };
		list<int> lst2{ 10, 2 };
		list<int> lst3{ 30, 3 };
		lst2 = util::move(lst1);
		lst3 = util::move(lst2);
		RuntimeAssertTrue(lst3.back() == 1 && lst3.size() == 20);
	}
	DebugNewTitle("assign fill");
	{
		list<int> lst1{ 20, 1 };
		lst1.assign(20, 2);
		RuntimeAssertTrue(lst1.back() == 2 && lst1.size() == 20);
		lst1.assign(10, 3);
		RuntimeAssertTrue(lst1.back() == 3 && lst1.size() == 10);
		lst1.assign(30, 4);
		RuntimeAssertTrue(lst1.back() == 4 && lst1.size() == 30);
	}
	DebugNewTitle("assign range");
	{
		list<int> lst1{ 20, 1 };
		list<int> lst2{ 10, 2 };
		list<int> lst3{ 25, 3 };
		lst1.assign(lst2.begin(), lst2.size());
		RuntimeAssertTrue(lst1.back() == 2 && lst1.size() == 10);
		lst1.assign(lst3.begin(), lst3.size());
		RuntimeAssertTrue(lst1.back() == 3 && lst1.size() == 25);
	}
	DebugNewTitle("initialize / front / back");
	{
		const list<int> lst1{ { 1,2,3,4,5,6,7 } };
		RuntimeAssertTrue(lst1.front() == 1);
		RuntimeAssertTrue(lst1.back() == 7);
		lst1.cdebug("test const iter:");
		lst1.crdebug("test const reverse:");
	}
	DebugNewTitle("test pop back/front");
	{
		list<int> lst1{ { 1,2,3,4,5,6,7 } };
		RuntimeAssertTrue(lst1.front() == 1 && lst1.back() == 7);
		lst1.pop_back();
		lst1.pop_front();
		RuntimeAssertTrue(lst1.front() == 2 && lst1.back() == 6 );
		RuntimeAssertTrue(lst1.size() == 5);
	}
	DebugNewTitle("test emplace back");
	{
		list<int> lst1{ {1,2,3,4} };
		lst1.emplace_back(6);
		DebugSubTitle("test move");
		list<test_copy_t> lst2{ 2, test_copy_t{3} };
		lst2.emplace_back( 4 );
		lst2.emplace_front( 1 );
		RuntimeAssertTrue(lst2.front() == 1 && lst2.back() ==4);
		lst2.debug("after emplace back");
	}
	DebugNewTitle("test emplace ");
	{
		list<int> lst1{ {1,3,5,7} };
		lst1.emplace(lst1.cbegin(), 0);
		lst1.emplace(lst1.cbegin() + 2, 2);
		lst1.emplace(lst1.cend() , 8);
		lst1.emplace(lst1.cend() - 2, 6);
		lst1.emplace(lst1.cend() - 4 , 4);
		RuntimeAssertTrue(lst1.front() == 0);
		RuntimeAssertTrue(lst1.back()  == 8);
		RuntimeAssertTrue(lst1.front() == 0);
		RuntimeAssertTrue(lst1.front() == 0);
		RuntimeAssertTrue(lst1.front() == 0);
	}
	DebugNewTitle("erase");
	{
		list<int> lst1{ { 1,2,3,4,5 } };
		ccdk_assert( lst1.end() == lst1.cend() );
		ccdk_assert( lst1.begin() == lst1.cbegin());
		ccdk_assert( lst1.end() -1 == lst1.rbegin());
		ccdk_assert( lst1.begin() - 1 == lst1.rend());
		lst1.erase(lst1.begin());
		lst1.debug("after erase:");
	}
	DebugNewTitle("erase range");
	{
		list<int> lst1{ { 1,2,3,4,5 } };
		lst1.erase(lst1.begin(), lst1.end() - 2);
		ccdk_assert(lst1.front() == 4);
		lst1.erase(lst1.begin(), lst1.end());
		ccdk_assert(lst1.size() == 0);
	}
	DebugNewTitle("cached_list");
	{
		cached_list<int> cl{ {1,2,3,4} };
		
	}

	ccdk_open_leak_check();

	getchar();
	return 0;
}