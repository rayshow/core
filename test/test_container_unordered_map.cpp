#define CCDK_PROFILE 1
//#define CCDK_NO_TRACE 1
#include<ccdk/container/unordered_map.h>

using namespace ccdk::ct;
using namespace ccdk;


struct equals_test {
	bool equals(int a) const noexcept {
		return a == 0;
	}
};

int main() 
{
	DebugNewTitle("insert");
	{
		unordered_map<int, int> a{};
		unordered_map<int, int> b{};
		constexpr int N = 666;
		for (int i = 0; i < N; ++i) {
			a.insert({ i,i+1 });
		}

		DebugNewTitle("attrite");
		RuntimeAssertTrue(a.size() == N);
		DebugValue("conflict:", a.conflict_count);
		DebugValue("loadfactor:", a.load_factor());
		for (int i = 0; i < N; ++i) {
			DebugValue(a[i]);
		}
		RuntimeAssertTrue(b.empty());
		RuntimeAssertTrue(!a.empty());

		DebugNewTitle("iterator");
		for (auto it = a.cbegin(); it != a.cend(); ++it) {
			DebugValue((*it).first(), (*it).second());
		}
		RuntimeAssertTrue(a.begin() == a.cbegin());
		decltype(a.cbegin()) it = a.begin();
		b.assign(a.begin(), a.size());
		b.assign(a.begin(), a.end());
		b.assign(a.cbegin(), a.cend());
	}
	DebugNewTitle("const char* map test");
	{
		unordered_map<const char*, int> a{
			{
				{"hello", 1},
				{"world", 2},
				{"java", 3},
				{"c++", 4}
			}
		};
		a.insert({ "hello", 1 });
		a.emplace("scale",5);
		a.insert({
			{"what", 5},
			{"is", 6}
		});
		
		a.foreach([&](fs::pair<const char*,int> const& p) {
			DebugValueIt(p.first(), p.second());
		});

		ccdk_assert(a.exists("hello"));
		ccdk_assert( 1== a.erase("hello"));
		ccdk_assert(!a.exists("hello"));
		ccdk_assert(a.at("java") == 3);
		ccdk_assert(a["c++"] == 4);
		ccdk_assert(a.clear().empty());
	}
	getchar();
	ccdk_open_leak_check();

	return 0;
}