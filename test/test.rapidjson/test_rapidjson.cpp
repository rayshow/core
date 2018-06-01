#include<rapidjson/include/rapidjson/rapidjson.h>
#include<rapidjson/include/rapidjson/document.h>
#include<stdio.h>


constexpr union { char c[4]; int l; } End = { {'1','2','3','4'} };
const bool endian = End.l;


int main() {
	 
	getchar();
	return 0;
}