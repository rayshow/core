#include<ccdk/container/fix_bitset.h>
#include<bitset>
#include<stdio.h>
#include<list>

using namespace ccdk;
using namespace ccdk::mpl;
using namespace ccdk::ct;

struct A {
	int a;
	constexpr A() :a{} {}
	constexpr A(A const& other) :a{ other.a } {}
};

int main()
{
	std::bitset<8> bit8;

	constexpr A a{};
	constexpr A b{ a };

	DebugNewTitle("constexpr ctor ");
	{
		constexpr fix_bitset<15>  bit15;
		constexpr fix_bitset<33>  bit33{ mpl::false_c };
		constexpr fix_bitset<11>  bit11{ constexpr_string_ctor_c, "10101010101010101" };
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
	DebugNewTitle("shift right");
	{
		fix_bitset<33> bit33{ "100000011000000110000001100000010" };
		bit33.debug();

	}

	getchar();
	return 0;
}
