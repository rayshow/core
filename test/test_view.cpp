#include<stdio.h>
#include<ccdk/container/array.h>
#include<ccdk/container/filter_view.h>
#include<ccdk/mpl/function/expr.h>
#include<ccdk/mpl/function/val.h>
#include<ccdk/mpl/util/fill.h>
#include<ccdk/mpl/units/ratio.h>
#include<ccdk/mpl/base/arithmetic_.h>
#include<exception>
#include<ratio>

using namespace ccdk::ct;
using namespace ccdk::mpl::fn::ph;
using namespace ccdk::mpl;
using namespace ccdk;
using namespace ccdk::mpl::units;

class test_exception
{
public:
	test_exception() { throw std::out_of_range{ "fda" }; }
};

int main()
{
	array<char, 10> buffer{ "abcdefg" };
	array<char, 20> buffer2{ 'a' };
	buffer2[-1] = 0;
	DebugValue(buffer.size());
	buffer.fit_capcity()[{ 0, -1 }] = 'a';
	printf(" buffer %s \n", buffer.data());

	
	getchar();
	return 0;
}