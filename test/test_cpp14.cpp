#include<cstdio>
#include<utility>
#include<memory>
#include<string>
#include<typeinfo>
#include<iostream>
#include<type_traits>
#include<cassert>
#include<functional>
#include<thread>
#include<vector>
#include<type_traits>
#include<unordered_map>
#include<map>
#include<bitset>
#include<hash_map>

#include<ccdk/type.h>
using namespace ccdk;
int main()
{
	std::map<int, int> a;
	std::bitset<19> bs;
	std::multimap<int, int> mm;
	std::hash_map<int, int> n;
	a.insert(std::make_pair(1, 1));
	a[1];
	a.find(1);
	n.insert(std::make_pair(1,1) );
	std::hash<int>();
	getchar();
	return 0;
}