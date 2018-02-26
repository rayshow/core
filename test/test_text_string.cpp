
#include<ccdk/mpl/base/derive_if.h>
#include<ccdk/mpl/type_traits/has_inner_type.h>
#include<ccdk/mpl/type_traits/has_swap.h>
#include<ccdk/memory/allocator_traits.h>
#include<memory>
#include<vector>
#include<ccdk/text/string.h>
#include<ccdk/text/string_view.h>
#include<stdio.h>

using namespace ccdk::txt;
using namespace ccdk;

struct test_swap_t
{
};

int main()
{
	DebugNewTitle("test string");

	DebugSubTitle("constructor ");
	string c1{ "hello" };
	DebugValue(c1.c_str());
	string c2{ "hello", 3 };
	DebugValue(c2.c_str());
	string c3{ c1 };
	DebugValue(c3.c_str());
	string c4{ util::move(c1) };
	DebugValue(c4.c_str());
	string c5{ c4, 1,3 };
	DebugValue(c5.c_str()); 
	string c6{ 10, 'a' };
	DebugValue(c6.c_str());

	DebugSubTitle("swap");
	string s1{ "hello" };
	string s2{ "world" };
	util::swap(s1, s2);
	DebugValue(s1.c_str());
	DebugValue(s2.c_str());
	
	DebugSubTitle("assign");
	string a1{ "hello" };
	string a2{ "world" };
	a1 = a2;
	DebugValue(a1.c_str());
	a1 = "c++";
	DebugValue(a1.c_str());
	a1.assign("bigworld", 3);
	DebugValue(a1.c_str());
	a1.assign(a2, 2, 3);
	DebugValue(a1.c_str());

	DebugSubTitle("size");
	string sz1{ "hello" };
	DebugValue(sz1.size());
	DebugValue(sz1.empty());
	DebugValue(sz1.max_size());
	DebugValue(sz1.capcity());
	sz1.clear();
	DebugValue(sz1.c_str());

	DebugSubTitle("access");
	string ac1{ "hello" };
	DebugValue(ac1[0]);
	DebugValue(ac1[-1]);
	DebugValue(ac1.at(0));
	DebugValue(ac1.at(4));
	DebugValue(ac1.front());
	DebugValue(ac1.front(2));
	DebugValue(ac1.back());
	DebugValue(ac1.back(1));
	DebugValue(ac1.back(3));

	DebugSubTitle("sequence");
	string seq1{ "hello" };
	DebugValue(seq1.push_back('a').c_str());
	DebugValue(seq1.pop_back().c_str());

	DebugSubTitle("insert");
	string ins1{ "hello" };
	string ins2{ " world" };
	string ins3{ " where are you?" };
	DebugValue(ins1.insert(0,"c++,").c_str());
	DebugValue(ins1.insert(0, "javaaa,",4).c_str());
	DebugValue(ins1.insert(-1, ins2).c_str());
	DebugValue(ins1.insert(-1, ins3, 0,6).c_str());

	DebugSubTitle("erase");
	string er1{ "hello, world" };
	string er2{ er1 };
	DebugValue(er2.erase(0, 2).c_str());
	DebugValue(er1.erase(0, -1).c_str());
	DebugValue(er1.size());

	DebugSubTitle("replace");
	string rp1{ "hello, world" };
	string rp2{ "|c++|" };
	DebugValue(rp1.replace(0, 5, "hi").c_str());
	DebugValue(rp1.replace(4, 9, rp2,1,4).c_str());

	DebugSubTitle("find");
	string fd1{ "hello" };
	string fd2{ "fjds, hello, fdsafds" };
	DebugValue(fd1.find('l'));
	DebugValue(fd1.find<-1>('l'));
	DebugValue(fd2.find("hello"));

	DebugSubTitle("trim");
	string tr1{ "		abc " };
	string tr2{ "	abc		" };
	DebugValue(tr1.trim().c_str());
	DebugValue(tr1.size());
	DebugValue(tr2.rtrim().c_str());
	DebugValue(tr2.size());
	DebugValue(tr2.ltrim().c_str());
	DebugValue(tr2.size());

	DebugSubTitle("append");
	string app1{ "hello world" };
	string app2{ ",hello world" };
	DebugValue(app1.append(app2).append(app2).append(app2).c_str());

	getchar();
	return 0;
}