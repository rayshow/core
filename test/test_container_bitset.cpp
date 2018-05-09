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

	AssertTrue((mod_type<uint8, uint32>(1) == 1));
	AssertTrue((mod_type<uint8, uint32>(0x07) == 0x07));
	AssertTrue((mod_type<uint8, uint32>(0x09) == 0x01));
	AssertTrue((mod_type<uint8, uint32>(0x0f) == 0x07));
	AssertTrue((mod_type<uint16, uint32>(0x0f) == 0x0f));
	AssertTrue((mod_type<uint16, uint32>(0x0f) == 0x0f));
	AssertTrue((mod_type<uint16, uint32>(0x11) == 0x01));
	AssertTrue((mod_type<uint16, uint32>(0x12) == 0x02));
	AssertTrue((mod_type<uint16, uint32>(0xff) == 0x0f));
	AssertTrue((mod_type<uint32, uint32>(0x0f) == 0x0f));
	AssertTrue((mod_type<uint32, uint32>(0x0f) == 0x0f));
	AssertTrue((mod_type<uint32, uint32>(0x21) == 0x01));
	AssertTrue((mod_type<uint32, uint32>(0x22) == 0x02));
	AssertTrue((mod_type<uint32, uint32>(0x2f) == 0x0f));

	AssertTrue((div_type<uint8, uint32>(1) == 0));
	AssertTrue((div_type<uint8, uint32>(0x07) == 0x00));
	AssertTrue((div_type<uint8, uint32>(0x09) == 0x08));
	AssertTrue((div_type<uint8, uint32>(0x0f) == 0x08));
	AssertTrue((div_type<uint16, uint32>(0x0f) == 0x00));
	AssertTrue((div_type<uint16, uint32>(0x11) == 0x10));
	AssertTrue((div_type<uint16, uint32>(0x12) == 0x10));
	AssertTrue((div_type<uint16, uint32>(0xff) == 0xf0));
	AssertTrue((div_type<uint32, uint32>(0x1f) == 0x00));
	AssertTrue((div_type<uint32, uint32>(0x1f) == 0x00));
	AssertTrue((div_type<uint32, uint32>(0x21) == 0x20));
	AssertTrue((div_type<uint32, uint32>(0x22) == 0x20));
	AssertTrue((div_type<uint32, uint32>(0x2f) == 0x20));

	DebugNewTitle("test bitset ctor");
	{
		uint32 i = 17;
#define  type  uint32
		//for (uint32 i = 4; i < 66; ++i)
		{
			std::cout << i << std::endl;
			bitset<> bs1{};
			bitset<> bs2{};
			bitset<type> bs3{ i, false };
			bitset<type> bs4{ i, true };
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
		uint32 i = 16;
		//for (uint32 i = 1; i <= carr_len(seq); ++i)
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
	DebugNewTitle("test move ctor");
	{
		bitset<> bs1{ from_string_literial_c,"1010101001" };
		bitset<> bs2{ util::move(bs1) };
		bs2.debug_it();
		bs2.debug_all();
	}
	DebugNewTitle("copy assign");
	{
#define seq "11111000001010100001110101111111101111111111111110101010000111111111101"
#define type uint8
		uint32 i = 16;
		//for (uint32 i = 1; i <= carr_len(seq); ++i)
		{
			std::cout << i << std::endl;
			bitset<type> bs1{ from_string_literial_c,"1010101001" };
			bitset<type> bs2{ from_string_literial_c, i, seq };
			bitset<uint64> bs3{ from_string_literial_c, i, seq };
			bs1 = bs2;
			bs1.debug();
			bs1.debug_all();
			bs1 = bs3;
			bs1.debug();
			bs1.debug_all();
		}
	}
	DebugNewTitle("fill");
	{
#define seq "11111000001010100001110101111111101111111111111110101010000111111111101"
#define type uint8
		uint32 i = 10;
		//for (uint32 i = 1; i <= carr_len(seq); ++i)
		{
			std::cout << i << std::endl;
			bitset<type> bs1{ from_string_literial_c, i, seq };
			bs1 = false;
			bs1.debug();
			bs1.debug_all();
			bs1 = true;
			bs1.debug();
			bs1.debug_all();
			bs1.assign(16, true);
			bs1.debug();
			bs1.debug_all();
			bs1.assign(16, false);
			bs1.debug();
			bs1.debug_all();
		}
	}
	DebugNewTitle("test assign range");
	{
#define seq "11111000001010100001110101111111101111111111111110101010000111111111101"
#define type uint8
		uint32 i = 10;
		//for (uint32 i = 1; i <= carr_len(seq); ++i)
		{
			std::cout << i << std::endl;
			bitset<type> bs1{ from_string_literial_c, i, seq };
			bitset<type> bs2{ 20, true };
			bs2.assign(bs1.begin(), bs1.end());
			bs2.debug();
			bs2.debug_all();
		}
	}
	DebugNewTitle("test index");
	{
#define seq "11111000001010100001110101111111101111111111111110101010000111111111101"
#define type uint8
		uint32 i = 10;
		//for (uint32 i = 1; i <= carr_len(seq); ++i)
		{
			bitset<> bs1{ from_string_literial_c, i, seq };
			bs1.debug();
			bs1.debug_index();
		}
		{
			bitset<> bs1{ from_string_literial_c, "1010101" };
			RuntimeAssertTrue(bs1.back() == true);
		}
	}
	DebugNewTitle("test push back");
	{
#define seq "11111000001010100001110101111111101111111111111110101010000111111111101"
#define type uint8
		uint32 i = 7;
		//for (uint32 i = 1; i <= carr_len(seq); ++i)
		{
			bitset<> bs1{ from_string_literial_c, i, seq };
			bs1.push_back(false);
			RuntimeAssertTrue(bs1.back() == false);
			bs1.push_back(true);
			RuntimeAssertTrue(bs1.back() == true);
			bs1.debug();
			bs1.debug_all();
		}
	}
	DebugNewTitle("test pop");
	{
		bitset<> bs1{ from_string_literial_c, "1010101010010101" };
		RuntimeAssertTrue(bs1.back() == 1);
		bs1.pop_back();
		RuntimeAssertTrue(bs1.back() == 0);
		bs1.pop_back();
		RuntimeAssertTrue(bs1.back() == 1);
	}
	DebugNewTitle("assign bit / insert");
	{
#define seq "00000000000000000000000001"
		bitset<> bs1{ from_string_literial_c, seq };
		int  i = 15;
		//for (int i = carr_len(seq)-1; i >= 0; --i) 
		{
			bs1.at(i) = 1;
			bs1.debug();
			bs1.debug_all();
		}
		//for (int i = 0; i <= carr_len(seq)*2; i=i+2)
		{
			//std::cout << i << std::endl;
			bs1.insert(i, false);
			bs1.debug();
			bs1.debug_all();
		}
	}
	DebugNewTitle("test erase");
	{
#define seq "111110101111111110000100100111"
		uint32 n = carr_len(seq);
		bitset<> bs1{ from_string_literial_c, n, seq };
		bs1.debug();
		bs1.erase(0, 5);
		bs1.debug();
		bs1.debug_all();
		int len = bs1.size();
		for (int i = 0; i < len; ++i)
		{
			bs1.erase(0);
			bs1.debug();
			bs1.debug_all();
		}
		bitset<> bs2{ from_string_literial_c, "111110101111111110000100100111" };
		len = bs2.size();
		for (int i = 0; i < len; ++i)
		{
			bs2.erase(bs2.begin());
			bs2.debug();
			bs2.debug_all();
		}
	}
	DebugNewTitle("clear ");
	{
		bitset<> bs2{ from_string_literial_c, "111110101111111110000100100111" };
		bs2.debug();
		bs2.debug_all();
		bs2.clear();
		bs2.debug();
		bs2.debug_all();
		bs2.assign(20, true);
		bs2.debug();
		bs2.debug_all();
		bs2.erase(0, bs2.size());
		bs2.debug();
		bs2.debug_all();
	}
	DebugNewTitle("test empty assign");
	{
		bitset<> bs1{};
		bitset<> bs2{from_string_literial_c, "101010101010"};
		bs1 = false;
		bs1.assign(10, true);
	}


	ccdk_open_leak_check();
	getchar();
	return 0;
}
