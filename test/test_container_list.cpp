
//#include<ccdk/container/slist.h>
#include<ccdk/container/vector.h>
#include<ccdk/memory/list_allocate_adapter.h>
#include<stdio.h>
#include<list>

using namespace ccdk;
using namespace ccdk::mpl;
using namespace ccdk::ct;

template<typename T>
struct slist_node
{
	T              data;
	slist_node<T>* next;
	explicit slist_node(T const& t) :data{ t }, next{ nullptr } {}
	explicit slist_node(T&& t) :data{ util::move(t) }, next{ nullptr } {}
};

struct A {

	A() { DebugValue("default ctor"); }
	A(int a):a { a } { DebugValue("1 ctor"); }

	int a;
};

int main()
{
	mem::simple_new_allocator<slist_node<A>> alloc;
	slist_node<A> *mem = alloc.allocate(4);
	construct<A>(mem, 1);
	DebugValue(mem->data.a);

	getchar();
	return 0;
}