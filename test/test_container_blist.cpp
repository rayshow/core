#include<ccdk/container/blist.h>

using namespace ccdk;
using namespace ccdk::ct;

//! ~ * - + T 
	namespace operator_detail                                                    
	{                                                                            
		template<typename T, typename Ret>                                       
		struct has_deref_impl                                         
		{                                                                        
			template<typename U, typename P,                                     
			typename = decltype(makeval<P>() = * declval<U>())>             
			static constexpr bool sfinae(int) { return true; }                   
			template<typename U, typename P>                                    
			static constexpr bool sfinae(...) { return false; }                  
			static constexpr bool value = sfinae<T, Ret>(0);                     
		};                                                                       
	}                                                                            

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


	getchar();
	return 0;
}