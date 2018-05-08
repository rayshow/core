
#include<ccdk/container/slist.h>
#include<stdio.h>

using namespace ccdk;
using namespace ccdk::mpl;
using namespace ccdk::ct;

int main()
{
	DebugNewTitle("ctor");
	{
		DebugSubTitle("default");
		{
			slist<int> s1{};
			slist<int> s2{nullptr};
		}
		DebugSubTitle("fill ctor");
		{
			slist<int> s1{5, 1};
		}
	}
	getchar();
	ccdk_open_leak_check();

	return 0;
}