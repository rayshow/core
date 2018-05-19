#define CCDK_PROFILE 1
//#define CCDK_NO_TRACE 1
#include<ccdk/container/unordered_map.h>
#include<ccdk/container/unordered_set.h>

using namespace ccdk::ct;
using namespace ccdk;

int main()
{
	DebugNewTitle("multi-insert");
	{
		unordered_multimap<int, const char*> mm;
		mm.insert({ 1,"xiongya" });
		mm.insert({ 1,"ask" });
		mm.insert({ 2,"lili" });
		mm.insert({ 3,"lucy" });
		mm.debug_value();
		auto range = mm.equal_range(1);
		for (auto it = range.first(); it != range.second(); ++it) {
			DebugValueIt((*it).first(), (*it).second());
		}
		DebugValueItEnd();
		auto range2 = mm.equal_range(2);
		for (auto it = range.first(); it != range.second(); ++it) {
			DebugValueIt((*it).first(), (*it).second());
		}
		DebugValueItEnd();
		ccdk_assert(2 == mm.erase(1));
		mm.debug_value();
		ccdk_assert(1 == mm.erase(2));
		mm.debug_value();
	}
	DebugNewTitle("multi-set")
	{
		unordered_multiset<int> s{ {1,1,1} };
		s.debug_value();
	}
	
	getchar();
	ccdk_open_leak_check();

	return 0;
}