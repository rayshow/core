#define CCDK_PROFILE 1
//#define CCDK_NO_TRACE 1
#include<ccdk/container/unordered_map.h>

using namespace ccdk::ct;
using namespace ccdk;

struct test_not_equal {
	int a = 0;
};

bool operator==(test_not_equal t, int i) { return t.a == i; }
bool operator!=(test_not_equal t, int i) { 
	DebugValue("special not equal");
	return t.a != i; 
}

struct equals_test {
	bool equals(int a) const noexcept {
		return a == 0;
	}
};

int main() 
{
	DebugNewTitle("equals");
	{
		DebugValue(util::equals(1, 2));
		DebugValue(util::equals("hello", "world"));
		const char *a = "hello";
		const char *b = "hello";
		DebugValue(util::equals(a, b));
		equals_test et{};
		DebugValue(util::equals(et, 1));
		DebugValue(util::equals(et, 0));
		DebugValue(util::equals(0, et));
	}
	DebugNewTitle("test not equal");
	{
		test_not_equal a{};
		a != 1;
	}

	{
		DebugNewTitle("insert");
		unordered_map<int, int> a{};
		unordered_map<int, int> b{};
		for (int i = 0; i < 90; ++i) {
			a.insert({ i,i+1 });
		}

		DebugNewTitle("attrite");
		RuntimeAssertTrue(a.size() == 90);
		DebugValue("conflict:", a.conflict_count);
		DebugValue("loadfactor:", a.load_factor());
		for (int i = 0; i < 92; ++i) {
			DebugValue(a[i]);
		}
		RuntimeAssertTrue(b.empty());
		RuntimeAssertTrue(!a.empty());

		DebugNewTitle("iterator");
		for (auto it = a.cbegin(); it != a.cend(); ++it) {
			DebugValue((*it).first, (*it).second);
		}
		RuntimeAssertTrue(a.begin() == a.cbegin());
		decltype(a.cbegin()) it = a.begin();
		b.assign(a.begin(), a.size());
		b.assign(a.begin(), a.end());
		b.assign(a.cbegin(), a.cend());
	}
	{
		DebugNewTitle("const char* map");
		unordered_map<const char*, int> a{
			{
				{"hello", 1},
				{"world", 2},
				{"java", 3},
				{"c++", 4}
			}
		};
		a.insert({ "hello", 1 });
		a.emplace("scale", "scale",5);
		a.insert({
			{"what", 5},
			{"is", 6}
		});
		
		a.foreach([&](const char* first, int second) {
			DebugValueIt(first, second);
		});

		ccdk_assert(a.exist("hello"));
		ccdk_assert( 1== a.erase("hello"));
		ccdk_assert(!a.exist("hello"));
		ccdk_assert(a.at("java") == 3);
		ccdk_assert(a["c++"] == 4);
		ccdk_assert(a.clear().empty());
	}

	getchar();
	_CrtDumpMemoryLeaks();

	return 0;
}