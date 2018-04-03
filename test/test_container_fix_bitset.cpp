#include<ccdk/container/fix_bitset.h>
#include<stdio.h>
#include<list>

using namespace ccdk;
using namespace ccdk::mpl;
using namespace ccdk::ct;



int main()
{
	DebugNewTitle("constexpr attribute ");
	{
		constexpr fix_bitset<15>  bit15;
		constexpr fix_bitset<33>  bit33{ mpl::false_c };
		constexpr fix_bitset<11>  bit11{ constexpr_string_ctor_c, "10101010101010101" };
		bit15.debug();
		bit15.begin();
		bit33.debug();
		bit11.debug();
	}
	DebugNewTitle("runtime ctor");
	{
		fix_bitset<11> bit11_false{ false };
		fix_bitset<11> bit11_true{ true };
		fix_bitset<11> bit11_char{ "11011011101000011" };
		bit11_true.debug();
		bit11_false.debug();
		bit11_char.debug();
		bit11_char.debug_iterator();
		bit11_char.debug_reverse_iterator();
		bit11_char.debug_const_reverse_iterator();
	}
	DebugNewTitle("iterator fill");
	{
		fix_bitset<11> bit11{ "1101010101010101" };
		DebugValue(bit11.end() - bit11.begin());
		DebugValue( (bit11.begin()+4) - bit11.end());
		fix_bitset<8> bit8{ bit11.begin(), bit11.end() };
		bit8.debug();
	}
	


	return 0;
}
