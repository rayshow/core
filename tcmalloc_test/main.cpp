#include <Windows.h> 
#include <iostream>

#define COUNT 2000*100
void func()
{
	size_t j = 0;
	for (size_t i = 0; i < COUNT; ++i)
	{
		if (j > 1001)
		{
			j = 0;
		}
		int * pInt = (int*)malloc(i * sizeof(int));
		free(pInt);
	}
}

void main()
{
	DWORD tStart, tEnd;

	tStart = timeGetTime();
	func();
	tEnd = timeGetTime();

	printf("%lu\n", tEnd - tStart);
	getchar();
}