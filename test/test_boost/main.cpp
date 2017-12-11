
#include<ccdk/type.h>
#include<ccdk/mpl/function/create.h>
#include<ccdk/mpl/function/partial.h>
#include<cstdio>

#include<boost/proto/proto.hpp>


void test()
{
	DebugValue("hello");
}

using namespace ccdk::mpl::fn;

int main()
{
	auto p = partial(test);

	return 0;
}