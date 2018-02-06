#include<ccdk/mpl/base/arithmetic_.h>
#include<ccdk/mpl/base/placeholder.h>
#include<ccdk/debug_type.hpp>
#include<ccdk/mpl/mcontainer/arg_pack.h>
#include<ccdk/mpl/mcontainer/val_pack.h>
#include<ccdk/mpl/mcontainer/algorithm/lambda_.h>
#include<ccdk/mpl/base/if_.h>
#include<ccdk/mpl/mcontainer/forward_.h>
#include<ccdk/mpl/mcontainer/backward_.h>
#include<ccdk/mpl/type_traits/is_float.h>
#include<ccdk/mpl/type_traits/is_same.h>
using namespace ccdk::mpl;
using namespace ccdk;

template<typename T, typename... Args>
struct replace :lambda_<T, Args...>{};


int main()
{
	typedef arg_pack<int, float, char, double> arg1;
	typedef arg_pack<int, float, char, double> arg2;
	DebugValue(count_if_<arg_pack<__, int, __, __, float>, is_placeholder_>::value);

	typedef replace< if_< is_same<__, __>, _2, char>, float, float> test;
	DebugTypeName< typename test::type >();


	getchar();
	return 0;
}