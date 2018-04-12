#include<ccdk/container/fix_bitset.h>
#include<bitset>
#include<stdio.h>
#include<list>

using namespace ccdk;
using namespace ccdk::mpl;
using namespace ccdk::ct;

int main()
{
	DebugNewTitle("constexpr ctor ");
	{
		constexpr fix_bitset<15>  bit15;
		constexpr fix_bitset<33>  bit33{ mpl::false_c };
		constexpr fix_bitset<11>  bit11{ from_string_literial_c, "10101010101010101" };
		constexpr fix_bitset<11> bit112{ bit11 };
		bit15.debug();
		bit33.debug();
		bit11.debug();
		bit112.debug();
	}
	DebugNewTitle("runtime ctor");
	{
		fix_bitset<11> bit11_false{ false };
		fix_bitset<11> bit11_true{ true };
		fix_bitset<13> bit13_char{ "11101000001" };
		fix_bitset<19> bit19{ bit13_char };
		fix_bitset<7> bit7{ bit13_char };
		bit11_false.debug();
		bit11_true.debug();
		bit13_char.debug();
		bit19.debug();
		bit7.debug();
	}
	DebugNewTitle("swap")
	{
		fix_bitset<11> bita{ "11111000000" };
		fix_bitset<11> bitb{ "00000111111" };
		bita.debug();
		bitb.debug();
		bita.swap(bitb);
		bita.debug();
		bitb.debug();
	}
	DebugNewTitle("assign")
	{
		fix_bitset<8> bita{ "11101001" };
		fix_bitset<8> bit8{ false };
		fix_bitset<9> bit9{ false };
		fix_bitset<7> bit7{ false };
		bit8.debug();
		bit9.debug();
		bit7.debug();
		bit8 = bita;
		bit7 = bita;
		bit9 = bita;
		bit8.debug();
		bit9.debug();
		bit7.debug();
		bita = false;
		bita.debug();
		bita.assign(1, 7, false);
		bita.debug();
	}
	DebugNewTitle("iterator fill");
	{
		fix_bitset<11> bit11{ "1101010101010101" };
		bit11.debug();
		bit11.debug_all();
		fix_bitset<8> bit8{ bit11.begin(), bit11.end() };
		bit8.debug();
		
	}
	DebugNewTitle("iterator ");
	{
		fix_bitset<11> bit11{ "11101000001" };
		bit11.debug_iterator();
		bit11[2] = true;
		bit11.debug_iterator();
		bit11[2] = false;
		bit11.debug_iterator();
		fix_bitset<8> bit8{ bit11.begin(), bit11.end() };
		bit8.debug();
	}
	DebugNewTitle("iterator ");
	{
		fix_bitset<11> bit11{ "11101000001" };
		bit11.debug_iterator();
		bit11.debug_const_iterator();
		bit11.debug_reverse_iterator();
		bit11.debug_const_reverse_iterator();
	}
	DebugNewTitle("const iterator ");
	{
		const fix_bitset<11> bit11{ "11011011101000011" };
		//bit11.debug_iterator();               //error
		bit11.debug_const_iterator();
		//bit11.debug_reverse_iterator();       //error
		bit11.debug_const_reverse_iterator();
	}
	DebugNewTitle("iterator op");
	{
		fix_bitset<11> bit11{ "11011011101" };
		RuntimeAssertTrue( (bit11.end() - bit11.begin()) == 11);
		RuntimeAssertTrue( (bit11.rbegin() + 11 == bit11.rend()));
		auto it = bit11.begin();
		RuntimeAssertTrue(*(it) == true);
		it++; RuntimeAssertTrue(*(it) == true);
		it++; RuntimeAssertTrue(*(it) == false );
		auto it2 = bit11.end();
		it2--; RuntimeAssertTrue(*(it2) == true); 
		it2--; RuntimeAssertTrue(*(it2) == false); 
		it2--; RuntimeAssertTrue(*(it2) == true);
		it2 -= 3; RuntimeAssertTrue(*(it2) == false);
		it2 -= 3; RuntimeAssertTrue(*(it2) == false);
		RuntimeAssertTrue(it2[3] == false);
		RuntimeAssertTrue(it2[-2] == true);	
	}
	DebugNewTitle("test set");
	{
		fix_bitset<11> bit11{ "11011011101" };
		bit11[2] = true;
		bit11.debug();
		bit11[2].flip();
		bit11.debug();
		bit11[2].reset();
		bit11.debug();
		bit11[2].set();
		bit11.debug();
	}
	DebugNewTitle("test count");
	{
		fix_bitset<11> bit11{ "11011011101" };
		RuntimeAssertTrue(bit11.count_one() == 8);
		RuntimeAssertTrue(bit11.count_zero() == 3);
	}
	DebugNewTitle("test any all none");
	{
		fix_bitset<11> none;
		fix_bitset<11> all{ true };
		fix_bitset<11> any{ "00100000000" };
		RuntimeAssertTrue(none.none());
		RuntimeAssertTrue(!none.any());
		RuntimeAssertTrue(!none.all());
		RuntimeAssertTrue(all.all());
		RuntimeAssertTrue(all.any());
		RuntimeAssertTrue(!all.none());
		RuntimeAssertTrue(any.any());
		RuntimeAssertTrue(!any.none());
		RuntimeAssertTrue(!any.all());
	}
	DebugNewTitle("test shl");
	{
		fix_bitset<66> shl{ "11111111011111111111011111111111101111111111101111111" };
		shl.debug();
		shl <<= 1;
		shl.debug();
		shl <<= 9;
		shl.debug();
		shl <<= 19;
		shl.debug();
		shl <<= 17;
		shl.debug();
		shl <<= 17;
		shl.debug();
	}
	DebugNewTitle("test shr");
	{
		fix_bitset<66> shr{ "11111111011111111111011111111111101111111111101111111" };
		shr.debug();
		shr >>= 1;
		shr.debug();
		shr >>= 9;
		shr.debug();
		shr >>= 19;
		shr.debug();
		shr >>= 17;
		shr.debug();
		shr >>= 17;
		shr.debug();
	}
	DebugNewTitle("test bit op");
	{
		fix_bitset<66> l{ "11111111011111111111011110000111101111111111101111111" };
		fix_bitset<66> r{ "00000100101010101001100000000000011111111111111111111" };
		l.debug();
		r.debug();
		(l | r).debug();
		(l & r).debug();
		(l ^ r).debug();
		l.flip().debug();
	}
	
	_CrtDumpMemoryLeaks();
	getchar();
	return 0;
}

/*
DebugNewTitle("shift right");
{
DebugSubTitle("test store in 8");
{
fix_bitset<7> bita{ "1001" };
fix_bitset<7> bitb{ "100001" };
fix_bitset<7> bitc{ "1000001" };
bita.debug();
bita.debug_iterator();
bita.debug_const_iterator();
bita.debug_reverse_iterator();
bita.debug_const_reverse_iterator();
bitb.debug();
bitb.debug_iterator();
bitb.debug_const_iterator();
bitb.debug_reverse_iterator();
bitb.debug_const_reverse_iterator();
bitc.debug();
bitc.debug_iterator();
bitc.debug_const_iterator();
bitc.debug_reverse_iterator();
bitc.debug_const_reverse_iterator();
}
DebugSubTitle("test store in 16");
{
fix_bitset<9> bita{ "100001" };
fix_bitset<9> bitb{ "1000001" };
fix_bitset<9> bitc{ "10000001" };
bita.debug();
bita.debug_iterator();
bita.debug_const_iterator();
bita.debug_reverse_iterator();
bita.debug_const_reverse_iterator();
bitb.debug();
bitb.debug_iterator();
bitb.debug_const_iterator();
bitb.debug_reverse_iterator();
bitb.debug_const_reverse_iterator();
bitc.debug();
bitc.debug_iterator();
bitc.debug_const_iterator();
bitc.debug_reverse_iterator();
bitc.debug_const_reverse_iterator();
}
DebugSubTitle("test store in 32");
{
fix_bitset<17> bita{ "10000011000001" };
fix_bitset<17> bitb{ "1000000110000001" };
fix_bitset<17> bitc{ "100000001100000001" };
bita.debug();
bita.debug_iterator();
bita.debug_const_iterator();
bita.debug_reverse_iterator();
bita.debug_const_reverse_iterator();
bitb.debug();
bitb.debug_iterator();
bitb.debug_const_iterator();
bitb.debug_reverse_iterator();
bitb.debug_const_reverse_iterator();
bitc.debug();
bitc.debug_iterator();
bitc.debug_const_iterator();
bitc.debug_reverse_iterator();
bitc.debug_const_reverse_iterator();
}
DebugSubTitle("test store in 64");
{
fix_bitset<33> bita{ "1000001100000110000011000001" };
fix_bitset<33> bitb{ "10000001100000011000000110000001" };
fix_bitset<33> bitc{ "100000001100000001100000001100000001" };
bita.debug();
bita.debug_iterator();
bita.debug_const_iterator();
bita.debug_reverse_iterator();
bita.debug_const_reverse_iterator();
bitb.debug();
bitb.debug_iterator();
bitb.debug_const_iterator();
bitb.debug_reverse_iterator();
bitb.debug_const_reverse_iterator();
bitc.debug();
bitc.debug_iterator();
bitc.debug_const_iterator();
bitc.debug_reverse_iterator();
bitc.debug_const_reverse_iterator();
}
DebugSubTitle("test store in multi-8");
{
fix_bitset<73> bita{ "1000001100000110000011000001100000110000011000001100001" };
fix_bitset<73> bitb{ "100000110000011000001100000110000011000001100000110000011000001" };
fix_bitset<73> bitc{ "100000011000000110000001100000011000000110000001100000011000000110000001" };
bita.debug();
bita.debug_iterator();
bita.debug_const_iterator();
bita.debug_reverse_iterator();
bita.debug_const_reverse_iterator();
bitb.debug();
bitb.debug_iterator();
bitb.debug_const_iterator();
bitb.debug_reverse_iterator();
bitb.debug_const_reverse_iterator();
bitc.debug();
bitc.debug_iterator();
bitc.debug_const_iterator();
bitc.debug_reverse_iterator();
bitc.debug_const_reverse_iterator();
}
}
*/