#include<iostream>
#include<type_traits>
#include<iostream>
using namespace std;
#define Output(x) std::cout<<x<<std::endl;


class A { public:A() {} };
class B {};
class C { public: virtual void fn() {} };

class test_default_ctor1 { public:int i; int*p; };
class test_default_ctor2 { public:int i; int*p; A a; };
class test_default_ctor3 :public A { public:int i; int*p; };
class test_default_ctor4 :public B { public:int i; int*p; };
class test_default_ctor5 :public C { public:int i; int*p; };

void test_default_ctor_generation() {

	Output("test_default_ctor_generation");

	//case 1: generate default non-trivial ctor
	test_default_ctor1 A1{1};
	test_default_ctor2 A2{};

	//default ctor will generate when class is non-trivial
	Output(is_trivial_v<test_default_ctor1>);
	Output(is_trivial_v<test_default_ctor2>);  //compiler will generate ctor call a.A::A()
	Output(is_trivial_v<test_default_ctor3>);  //compiler will generate ctor call a.A::A()
	Output(is_trivial_v<test_default_ctor4>);
	Output(is_trivial_v<test_default_ctor5>);  //compiler will generate ctor set vtable element
	Output(is_pod_v<test_default_ctor1>);
	Output(is_pod_v<test_default_ctor2>);  //compiler will generate ctor call a.A::A()
	Output(is_pod_v<test_default_ctor3>);  //compiler will generate ctor call a.A::A()
	Output(is_pod_v<test_default_ctor4>);
	Output(is_pod_v<test_default_ctor5>);  //compiler will generate ctor set vtable element

	int a = 0;
	
}

class empty_A {};
class derive1 : public empty_A {  };
class derive2 : public empty_A { int a; };
class derive3 : public virtual empty_A { };
class derive4 : public virtual empty_A { int a; };
class derive5 : public virtual derive1, virtual derive2{ };

void test_virtual_derived(){
	Output("test_virtual_derived");
	Output(sizeof(empty_A));
	Output(sizeof(derive1));
	Output(sizeof(derive2));
	Output(sizeof(derive3));
	Output(sizeof(derive4));
	Output(sizeof(derive5));
};


class member_data { public: int a; };
class member_data2 { public: int a; virtual void fn() {} };

void test_member_data()
{
	Output("member_data");
	member_data md{};
	Output(&member_data::a);
	void *pa = &md.a;
	void *pmd = &md;
	Output(int(pa)-int(pmd));
	member_data2 md2{};
	Output(&member_data2::a);
	pa = &md2.a;
	pmd = &md2;
	Output(int(pa) - int(pmd));
	
}


int main()
{
	test_default_ctor_generation();
	test_virtual_derived();
	test_member_data();

	getchar();
	return 0;
}