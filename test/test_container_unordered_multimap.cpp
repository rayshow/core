#define CCDK_PROFILE 1
//#define CCDK_NO_TRACE 1
#include<ccdk/container/unordered_map.h>

using namespace ccdk::ct;
using namespace ccdk;


int main()
{

	DebugNewTitle("multi-insert");
	{
		unordered_multimap<int, const char*> mm;
		mm.insert({ 1,"xiongya" });
		mm.insert({ 1,"ask" });
		mm.debug_value();
	}
	
	getchar();
	ccdk_open_leak_check();

	return 0;
}