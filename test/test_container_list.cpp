
//#include<ccdk/container/slist.h>
#include<ccdk/container/vector.h>
#include<ccdk/memory/list_allocate_adapter.h>
#include<stdio.h>
#include<list>

using namespace ccdk;
using namespace ccdk::mpl;
using namespace ccdk::ct;

struct no_trivial {
	int a = 0;

	no_trivial() { DebugValue("no_trivial construct"); }
	~no_trivial() { DebugValue("no_trivial destruct"); }
};

struct implace_test {
	int a;
	float b;

	implace_test() :a{}, b{} { DebugValue("no-parameter constructor "); }
	implace_test(int a, float b) :a{ a }, b{ b } {
		DebugValue("parameter constructor ");
	}
};

struct node {
	int a;
	node* next;
};


int main()
{
	int a[] = { 1,2,3,4,5,6,7 };
	int b[] = { 0,0,0,0,0,0,0 };
	//int* c= static_cast<int*>(memcpy(b, a, 2))+2;
	
	int* c = util::ut_impl::construct_copy_n_impl(b, a, 2, opt_lv2{});
	DebugValue(c - b);

	int v = 0;
	DebugValue(v = mem::has_valid_next<node>::value);

	getchar();
	return 0;
}