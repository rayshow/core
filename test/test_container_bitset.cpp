#include<ccdk/container/bitset.h>
#include<bitset>
#include<stdio.h>
#include<list>

using namespace ccdk;
using namespace ccdk::mpl;
using namespace ccdk::ct;


int main()
{
	DebugNewTitle(" test ctor ");
	{
		bitset<> bs1{};
		bitset<> bs2{};
		bitset<> bs3{ 17,false };
		bs3.debug();
		bs3.debug_all();
		/*for (uint32 i = 7; i < 66; ++i)
		{
			bitset<> bs3{ i,false };
			bs3.debug();
			bs3.debug_all();
		}*/
		bitset<> bs4{ "1010101010101001111" };
		bs4.debug();
		bs4.debug_all();
	} 
	_CrtDumpMemoryLeaks();
	getchar();
	return 0;
}
