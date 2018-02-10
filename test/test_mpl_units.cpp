#include<stdio.h>
#include<ccdk/debug_type.hpp>
#include<ccdk/mpl/units/quantity.h>
#include<ccdk/mpl/mstate_machine/state_machine.h>
#include<ccdk/mpl/mcontainer/arg_pack.h>

using namespace ccdk;
using namespace ccdk::mpl;
using namespace ccdk::mpl::units;
using namespace ccdk::mpl::msm;
int main()
{
	ulength len = 100_m;
	length<kilo> len2 = 1_km;
	DebugValue(len);
	DebugValue(len2);
	
	getchar();
	return 0;
}