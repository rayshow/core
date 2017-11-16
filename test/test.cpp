#include<iostream>
#include<string>
#include<type_traits>
#include<tuple>
#include<ccdk/preprocessor/stringize.h>

#include"test_type_traits.hpp"
#include"test_vector.hpp"
#include"test_float.hpp"
#include"test_boost.hpp"
#include<immintrin.h>
#include<inttypes.h>
#include<float.h>

using namespace std;

  
#if defined(AURORCCDK_COMPILER_MSVC)
//4514: un-used inline function had been removed
//4710: function had not been inlined
#pragma warning(disable:4514 4710)
#endif

int main()
{
	
	//ProcessTool();
	//TestProprecessor();
	TestTypeTraits();
	//TestMpl();
	//TestVector();
	//TestFloat();
	getchar();
	return 0;
}