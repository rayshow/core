#include<ccdk/container/unordered_set.h>

using namespace ccdk;
using namespace ccdk::ct;

int main() {
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
		s1.erase("world");
		ccdk_assert(s1.exists("hello"));
		ccdk_assert(s1.find("c++")!=s1.end());
		ccdk_assert(s1.clear().empty());
	}
	getchar();
	ccdk_open_leak_check();

	return 0;
}