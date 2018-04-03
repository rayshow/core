#include<ccdk/container/fix_bitset.h>
#include<stdio.h>
#include<list>

using namespace ccdk;
using namespace ccdk::mpl;
using namespace ccdk::ct;



int main()
{
	DebugNewTitle("constexpr attribute ");
	constexpr fix_bitset<15>  bit15;
	constexpr fix_bitset<101> bit101{mpl::false_c};
	constexpr fix_bitset<11>  bit11{ constexpr_string_ctor_c, "1" };

	DebugNewTitle("rutime ctor");
	{
		fix_bitset<11> bit11_false{ false };
		fix_bitset<11> bit11_true{ true };
		fix_bitset<11> bit11_char{ "112" };
		int a = 0;
	}
	


	return 0;
}
