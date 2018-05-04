#define CCDK_PROFILE 1
#include<ccdk/container/unordered_map.h>

using namespace ccdk::ct;
using namespace ccdk;


int main() 
{
	{
		unordered_map<int, int> a{};
		for (int i = 0; i < 90; ++i) {
			a.insert({ i,i+1 });
		}

		DebugValue("conflict:", a.conflict_count);
		for (int i = 0; i < 90; ++i) {
			DebugValue(a[i]);
		}
	}

	getchar();
	_CrtDumpMemoryLeaks();

	return 0;
}