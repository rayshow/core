#include<ccdk/container/bitset.h>
#include<bitset>
#include<stdio.h>
#include<list>

using namespace ccdk;
using namespace ccdk::mpl;
using namespace ccdk::ct;


int main()
{
	uint32 i = 17;
	using T = uint32;
	//for (uint32 i = 4; i < 66; ++i)
	{
		DebugNewTitle("test bitset ctor");
		{
			std::cout << i << std::endl;
			bitset<> bs1{};
			bitset<> bs2{};
			bitset<T> bs3{ i, false };
			bitset<T> bs4{ i, true };
			bs3.debug();
			bs3.debug_all();
			bs4.debug();
			bs4.debug_all();
			std::cout << std::endl;
		}
	}

	DebugNewTitle("test ctor from literal");
	{
		bitset<> bs1{ from_string_literial_c, "1111100000101010" };
		bs1.debug();
		bs1.debug_all();
		bitset<> bs2{ "1111101" };
		bs2.debug();
		bs2.debug_all();
	}
	DebugNewTitle("test range ctor");
	{
		bitset<> bs1{ from_string_literial_c, "1111100000101011" };
		auto begin = bs1.begin();
		auto end = bs1.end();
		bs1.debug();
		bs1.debug_it();
		bs1.debug_all();
		bitset<> bs2{ bs1.begin(), bs1.end() };
		bs2.debug();
		bs2.debug_it();
		bs2.debug_all();
	}
	getchar();
	return 0;
}
