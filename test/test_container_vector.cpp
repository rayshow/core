
#include<ccdk/container/vector.h>
#include<stdio.h>

using namespace ccdk;
using namespace ccdk::mpl;
using namespace ccdk::ct;

int main()
{
	DebugValue("empty constructor");
	vector<int> ivec1{};

	DebugValue(" fill constructor");
	vector<int> ivec2(10, 0);
	vector<char> ivec3(10, '0');
	vector<vector<int>> ivec4(10,vector<int>(10,2));
	auto& ref = ivec4[9];

	getchar();
	return 0;
}