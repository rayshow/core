
#include<ccdk/type.h>
#include<ccdk/mpl/function/create.h>
#include<ccdk/mpl/function/partial.h>
#include<cstdio>

#include<boost/proto/proto.hpp>
#include<boost/any.hpp>
#include<boost/lockfree/queue.hpp>

using namespace ccdk::mpl::fn;
using namespace ccdk;

void test()
{
	DebugValue("hello");
}



int main()
{
	auto p = partial(test);

	return 0;
}