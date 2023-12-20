#include<ccdk/mpl/util/scope_destructor.h>
#include<stdio.h>


using namespace ccdk::mpl::util;

int main()
{
	printf("out start\n");
	{
		printf("inner start\n");
		scope_destructor { []() { printf("in destructor\n");  } };
		printf("inner end\n");
	}
	printf("out end\n");
	getchar();
	return 0;
}