
#include<ccdk/mpl/base/derive_if.h>
#include<ccdk/mpl/type_traits/has_inner_type.h>
#include<ccdk/memory/allocator_traits.h>
#include<memory>
#include<vector>
#include<ccdk/text/string.h>
#include<stdio.h>

using namespace ccdk::txt;
using namespace ccdk;

struct test_swap_t{};

int main()
{
	DebugNewTitle("constructor ");
	{
		DebugSubTitle("empty");
		{
			string e1{};
			string e2{ nullptr };
		}
		DebugSubTitle("fill");
		{
			string s{ 31,'i' };
			s.debug_value();
		}
		DebugNewTitle("copy c-string");
		{
			string s1{ "123456789"};
			s1.debug_value();
			RuntimeAssertTrue(s1.size() == 9);
			string s2{ "123456789",5 };
			s2.debug_value();
			RuntimeAssertTrue(s2.size() == 5);
			string s3{ string_literial_init_c, "123456789"};
			s3.debug_value();
			RuntimeAssertTrue(s3.size() == 9);
		}
		DebugNewTitle("copy/move/range ctor");
		{
			string s1{ "123456789" };
			s1.debug_value();
			RuntimeAssertTrue(s1.size() == 9);
			string s2{ s1 };
			s2.debug_value();
			RuntimeAssertTrue(s2.size() == 9);
			string s3{ s1.begin(), s1.end() };
			s3.debug_value();
			RuntimeAssertTrue(s3.size() == 9);
			string s4{ util::move(s1) };
			s4.debug_value();
			s1.debug_value();
			RuntimeAssertTrue(s4.size() == 9);
			RuntimeAssertTrue(s1.size() == 0);
		}
	}
	DebugNewTitle("swap")
	{
		string s1{ "123456789" };
		string s2{ "987654321" };
		s1.debug_value();
		s2.debug_value();
		util::swap(s1, s2);
		s1.debug_value();
		s2.debug_value();
	}
	DebugNewTitle("empty assign");
	{
		string s1{};
		s1 = "xiongya";
		s1.debug_value();
		RuntimeAssertTrue(s1.size() == 7);
		string s2{};
		s2.assign(7, 'i');
		s2.debug_value();
		RuntimeAssertTrue(s2.size() == 7);
	}
	DebugNewTitle("assign");
	{
		string s1{ "123456789" };
		string s2{ "helloworld" };
		string s3{};
		s1.debug_value();
		s1 = "xiongya";
		s1.debug_value();
		RuntimeAssertTrue(s1.size() == 7);
		s1 = s2;
		s1.debug_value();
		RuntimeAssertTrue(s1.size() == s2.size());
		s3 = util::move(s1);
		RuntimeAssertTrue(s1.empty());
		s3.debug_value();
		RuntimeAssertTrue(s3.size() == s2.size());
		s3.assign(10, 'i');
		s3.debug_value();
		RuntimeAssertTrue(s3.size() == 10);
		s3.assign(string_literial_init_c, "literial init");
		s3.debug_value();
		RuntimeAssertTrue(s3.size()==13);
		s3.assign("hello,world", 5);
		s3.debug_value();
		RuntimeAssertTrue(s3.size() == 5);
	}
	DebugNewTitle("pop back/push back");
	{
		string s1{ "123456789" };
		while (!s1.empty()) {
			DebugValueIt(s1.back());
			s1.pop_back();
		}
		DebugValueItEnd();
		RuntimeAssertTrue(s1.size() == 0);
		string s2{};
		for (char c = 'a'; c <= 'z'; ++c) {s2.push_back(c);}
		s2.debug_value();
	}
	DebugNewTitle("insert");
	{
		string s1{ "123456789" };
		s1.insert(8, 'a');
		s1.debug_value();
		s1.insert(3, "hello,world");
		s1.debug_value();
		s1.insert(2, string_literial_init_c, "xiongya");
		s1.debug_value();
		s1.insert(1, string{ "c++" });
		s1.debug_value();
		s1.insert(s1.size() - 2, "java,scala", 4);
		s1.debug_value();
		s1.insert(s1.size() - 1, 5,'i');
		s1.debug_value();
	}
	DebugNewTitle("erase");
	{
		string s1{ "12345678910111213141516171819202122232425" };
		s1.erase(1,4);
		s1.debug_value();
		s1.erase(s1.size()-2);
		s1.debug_value();
	}
	DebugNewTitle("replace");
	{
		string s1{ "012345678910111213141516171819202122232425" };
		s1.replace(2, 9, "abcdefgh");
		s1.debug_value();
		s1.replace(s1.size() - 6, s1.size(), "#$@");
		s1.debug_value();
		s1.replace(1, 4, 5, 'i');
		s1.debug_value();
	}
	DebugNewTitle("find");
	{
		//string s1{ "012345678910111213141516171819202122232425" };
		//RuntimeAssertTrue(s1.find_index('9') == 9);
		//RuntimeAssertTrue(s1.find_index([](auto ch) {return ch == '8'; }) == 8);
		//RuntimeAssertTrue(s1.find_index("1011") == 10);
		//RuntimeAssertTrue(s1.find_index("1012") == s1.size());
		string s2{ "0001230123" };
		RuntimeAssertTrue(s2.find_index<-1>("012") == 6);
		RuntimeAssertTrue(s2.find_index<-2>("012") == 2);
		RuntimeAssertTrue(s2.find_index<-1>("112") == s2.size());
		RuntimeAssertTrue(s2.find_index<-2>("112") == s2.size());
		RuntimeAssertTrue(s2.find_index<-200>("012") == s2.size());
		RuntimeAssertTrue(s2.find_index<-200>("112") == s2.size());
		string s3{ "0001230123" };
		RuntimeAssertTrue(s3.find_index("012") == 2);
		RuntimeAssertTrue(s3.find_index("0124") == s3.size());
		RuntimeAssertTrue(s3.find_index<2>("012") == 6);
		RuntimeAssertTrue(s3.find_index<2>("0124") == s3.size());
		RuntimeAssertTrue(s3.find_index<200>("012") == s3.size());
		RuntimeAssertTrue(s3.find_index<200>("0124") == s3.size());
	}
	DebugNewTitle("trim");
	{
		string s1{ "  1123  " };
		string s2{ "  1123  " };
		string s3{ "  1123  " };
		s1.ltrim().debug_content();
		s2.rtrim().debug_content();
		s3.trim().debug_content();
		ccdk_assert(s1.size() == 6);
		ccdk_assert(s2.size() == 6);
		ccdk_assert(s3.size() == 4);
		DebugValue(encoding<ascii_encoding>::is_space(' '));
		DebugValue(encoding<ascii_encoding>::is_space('\n'));
		DebugValue(encoding<ascii_encoding>::is_space('\r'));
	}

	/*string c1{ "hello" };
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
*/

	ccdk_open_leak_check();
	getchar();
	return 0;
}