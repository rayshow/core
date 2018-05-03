#include<ccdk/container/unordered_map.h>

using namespace ccdk::ct;

int main() 
{
	DebugNewTitle("empty ctor");
	{
		unordered_map<int, int> a{};
	}
	{
		unordered_map<int, int> a{};
		a.insert({ 1,2 });
	}

	_CrtDumpMemoryLeaks();

	return 0;
}