#include<cstdio>
#include<utility>
#include<memory>
#include<string>
#include<typeinfo>
#include<iostream>
#include<type_traits>
#include<cassert>
#include<functional>
#include<ccdk/mpl/type_traits.h>
#include<ccdk/mpl/mcontainer/arg_pack.h>
#include<ccdk/mpl/base/integer_.h>
#include<ccdk/mpl/base/integer_.h>
#include<ccdk/mpl/type_traits/select_case.h>
#include<ccdk/mpl/function/arg.h>
#include<ccdk/mpl/function/function.h>
#include<ccdk/mpl/function/bind_mfn.h>
#include<ccdk/mpl/function/partial.h>
#include<ccdk/mpl/function/capture.h>
#include<regex>

#include<mutex>
#include<condition_variable>


using namespace ccdk;
using namespace ccdk::mpl;
using namespace ccdk::mpl::literals;


test_copy_t t{};
test_copy_t get_move()
{
	return const_cast<const test_copy_t&>(t);
}

class test_signle_op
{

};


void operator++(test_signle_op)
{
	DebugValue("++1");
}

void operator++(test_signle_op, int)
{
	DebugValue("1++");
}

int main()
{
	std::condition_variable cv;
	std::mutex mt{};

	cv.notify_one();
	cv.wait_for(std::unique_lock<std::mutex>{mt}, std::chrono::microseconds(1000));
	printf("after waiting ");


	getchar();
	return 0;
}