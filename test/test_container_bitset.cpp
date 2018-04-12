#include<ccdk/container/bitset.h>
#include<bitset>
#include<stdio.h>
#include<list>

using namespace ccdk;
using namespace ccdk::mpl;
using namespace ccdk::ct;

template<uint32 N>
constexpr uint32 carr_len(char const (&arr)[N]) { return N-1; }

int main()
{
	AssertTrue(carr_len("ab") == 2);
	AssertTrue(1 << 0 == 1);

	AssertTrue((keep_low<uint8, uint32>(1) == 1));
	AssertTrue((keep_low<uint8, uint32>(0x07) == 0x07));
	AssertTrue((keep_low<uint8, uint32>(0x09) == 0x01));
	AssertTrue((keep_low<uint8, uint32>(0x0f) == 0x07));
	AssertTrue((keep_low<uint16, uint32>(0x0f) == 0x0f));
	AssertTrue((keep_low<uint16, uint32>(0x0f) == 0x0f));
	AssertTrue((keep_low<uint16, uint32>(0x11) == 0x01));
	AssertTrue((keep_low<uint16, uint32>(0x12) == 0x02));
	AssertTrue((keep_low<uint16, uint32>(0xff) == 0x0f));
	AssertTrue((keep_low<uint32, uint32>(0x0f) == 0x0f));
	AssertTrue((keep_low<uint32, uint32>(0x0f) == 0x0f));
	AssertTrue((keep_low<uint32, uint32>(0x21) == 0x01));
	AssertTrue((keep_low<uint32, uint32>(0x22) == 0x02));
	AssertTrue((keep_low<uint32, uint32>(0x2f) == 0x0f));

	AssertTrue((keep_high<uint8, uint32>(1) == 0));
	AssertTrue((keep_high<uint8, uint32>(0x07) == 0x00));
	AssertTrue((keep_high<uint8, uint32>(0x09) == 0x08));
	AssertTrue((keep_high<uint8, uint32>(0x0f) == 0x08));
	AssertTrue((keep_high<uint16, uint32>(0x0f) == 0x00));
	AssertTrue((keep_high<uint16, uint32>(0x11) == 0x10));
	AssertTrue((keep_high<uint16, uint32>(0x12) == 0x10));
	AssertTrue((keep_high<uint16, uint32>(0xff) == 0xf0));
	AssertTrue((keep_high<uint32, uint32>(0x1f) == 0x00));
	AssertTrue((keep_high<uint32, uint32>(0x1f) == 0x00));
	AssertTrue((keep_high<uint32, uint32>(0x21) == 0x20));
	AssertTrue((keep_high<uint32, uint32>(0x22) == 0x20));
	AssertTrue((keep_high<uint32, uint32>(0x2f) == 0x20));

	DebugNewTitle("test bitset ctor");
	{
		uint32 i = 17;
		using T = uint32;
		//for (uint32 i = 4; i < 66; ++i)
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

	DebugNewTitle("test ctor from literal / range / copy ctor ");
	{
#define seq "11111000001010100001110101111111101111111111111110101010000111111111101"
#define type uint64
		//uint32 i = 16;
		for (uint32 i = 1; i <= carr_len(seq); ++i)
		{
			std::cout << i << std::endl;
			bitset<type> bs1{ from_string_literial_c, i, seq };
			bitset<> bs2{ bs1.begin(), bs1.size() };
			bitset<> bs3{ bs1 };
			bitset<type> bs4{ bs1 };
			bs2.debug();
			bs3.debug_it();
			bs4.debug_rit();
			bs1.debug_all();
		}
	}
	getchar();
	return 0;
}
