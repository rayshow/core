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
		string s1{ "012345678910111213141516171819202122232425" };
		RuntimeAssertTrue(s1.find_index('9') == 9);
		RuntimeAssertTrue(s1.find_index([](auto ch) {return ch == '8'; }) == 8);
		RuntimeAssertTrue(s1.find_index("1011") == 10);
		RuntimeAssertTrue(s1.find_index("1012") == s1.size());
		string s2{ "0001230123" };
		RuntimeAssertTrue(s2.find_index("012",-1) == 6);
		RuntimeAssertTrue(s2.find_index("012") == 2);
		RuntimeAssertTrue(s2.find_index("112",-1) == s2.size());
		RuntimeAssertTrue(s2.find_index("112",-2) == s2.size());
		RuntimeAssertTrue(s2.find_index("012",-200) == s2.size());
		RuntimeAssertTrue(s2.find_index("112",-200) == s2.size());
		string s3{ "0001230123" };
		RuntimeAssertTrue(s3.find_index("012") == 2);
		RuntimeAssertTrue(s3.find_index("0124") == s3.size());
		RuntimeAssertTrue(s3.find_index("012",2) == 6);
		RuntimeAssertTrue(s3.find_index("0124",2) == s3.size());
		RuntimeAssertTrue(s3.find_index("012",200) == s3.size());
		RuntimeAssertTrue(s3.find_index("0124",200) == s3.size());
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
	DebugNewTitle("append");
	{
		string s1{ "1234" };
		string s2{ "hello" };
		s1.append("5678");
		s1.debug_value();
		s1.append(string_literial_init_c, "91011");
		s1.debug_value();
		s1.append(s2);
		s1.debug_value();
		s1.append(4, 'i');
		s1.debug_value();
	}
	DebugNewTitle("concat");
	{
		string s1{ "hello" };
		string s2{ "world" };
		auto s3 = s1 + s2;
		s3.debug_value();
		s3.append(1);
		s3.debug_value();
		s3.append(-323LL);
		s3.debug_value();
		s3.append(543LL);
		s3.debug_value();
		s3.append(2.5343);
		s3.debug_value();
	}

	ccdk_open_leak_check();
	getchar();
	return 0;
}