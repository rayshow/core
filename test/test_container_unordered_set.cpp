#define CCDK_PROFILE 1
#include<ccdk/container/unordered_set.h>
#include<string>
using namespace ccdk;
using namespace ccdk::ct;
using namespace std;

int main() {

	DebugNewTitle("test ctor");
	{
		DebugSubTitle("empty");
		{
			unordered_set<int> s1{};
			unordered_set<const char*> s2{};
			unordered_set<string> s3{};
		}
		DebugSubTitle("copy");
		{
			unordered_set<int> s1{ {1,2,3} };
			unordered_set<const char*> s2{ {"1","2","3"} };
			unordered_set<string> s3{ {"1","2","3"} };
			s1.debug_value("s1");
			s2.debug_value("s2");
			s3.debug_value("s3");
		}
		DebugSubTitle("move");
		{
			unordered_set<int> s1{ { 1,2,3 } };
			unordered_set<const char*> s2{ { "1","2","3" } };
			unordered_set<string> s3{ { "1","2","3" } };
			auto m1{ util::move(s1) };
			auto m2{ util::move(s2) };
			auto m3{ util::move(s3) };
			m1.debug_value();
			m2.debug_value();
			m3.debug_value();
		}
		DebugSubTitle("range");
		{
			unordered_set<int> s1{ { 1,2,3 } };
			unordered_set<const char*> s2{ { "1","2","3" } };
			unordered_set<string> s3{ { "1","2","3" } };
			unordered_set<int> m1{ s1.begin(), s1.end() };
			unordered_set<const char*> m2{ s2.cbegin(), s2.cend() };
			unordered_set<string> m3{ s3.begin(), s3.end() };
			m1.debug_value();
			m2.debug_value();
			m3.debug_value();
		}
	}
	for (int i = 0; i < 10; ++i)
	{
		DebugNewTitle("assign");
		{
			//diff iterator
			//assign self's iterator
			unordered_set<int> s1{};
			unordered_set<int> s2{ { 1,2,3 } };
			s1 = util::move(s1);
			unordered_set<string> s3{ { "1","2","3" } };
			DebugValue(s3.conflict_count);
			s3.debug_it();
			ccdk_assert(s3.size() == 3);
			unordered_set<string> s4{};
			s4 = s3;
			unordered_set<const char*> s5{ { "1","2","3" } };
			unordered_set<const char*> s6{};
			s6.assign(s5.begin(), s5.end());
			unordered_set<const char*> s7{ { "1","2","3" } };
			s7 = { "4","5","6" };
			s7.debug_value();
		}
		DebugNewTitle("emplace/insert");
		{
			unordered_set<std::string> s1{};
			unordered_set<std::string> s2{ { "iphone", "apple" } };
			s1.emplace("abc");
			s1.emplace("hello");
			s1.insert("world");
			s1.insert("move");
			s1.insert({ "c++","java" });
			s1.insert(s2.begin(), s2.end());
			s1.debug_value();
			ccdk_assert(s1.size() == 8);
		}
	}
	{
		unordered_set<int> s1{ {1,2,3} };
		unordered_set<int> s2{ {2,3,4} };
		s1.intersect(s2).foreach([](int a) {
			DebugValueIt(a);
		});
		s1.unions(s2).foreach([](int a) {
			DebugValueIt(a);
		});
		s1.minus(s2).foreach([](int a) {
			DebugValueIt(a);
		});
		s2.minus(s1).foreach([](int a) {
			DebugValueIt(a);
		});
	}
	{
		unordered_set<const char*> s1{ { "hello","world","c++" } };
		s1 = { "hello","world","c++" };
		DebugValue(s1.bucket_size());
		for (auto it : s1) {
			DebugValueIt(it);
		}
		s1.emplace("java");
		ccdk_assert(s1.exists("world"));
		s1.erase("world");
		ccdk_assert(!s1.exists("world"));
		ccdk_assert(s1.exists("hello"));
		ccdk_assert(s1.find("c++")!=s1.end());
		ccdk_assert(s1.clear().empty());
	}

	getchar();
	ccdk_open_leak_check();

	return 0;
}