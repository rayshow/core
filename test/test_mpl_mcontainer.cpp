
#include<ccdk/debug_type.hpp>
#include<ccdk/mpl/base/arithmetic_.h>
#include<ccdk/mpl/base/placeholder.h>
#include<ccdk/mpl/base/if_.h>
#include<ccdk/mpl/type_traits/is_float.h>
#include<ccdk/mpl/type_traits/is_same.h>
#include<ccdk/mpl/mcontainer/arg_pack.h>
#include<ccdk/mpl/mcontainer/val_pack.h>
#include<ccdk/mpl/mcontainer/range_.h>
#include<ccdk/mpl/mcontainer/algorithm/lambda_.h>
#include<ccdk/mpl/mcontainer/algorithm/count_.h>
#include<ccdk/mpl/mcontainer/algorithm/find_.h>
#include<ccdk/mpl/mcontainer/algorithm/fold_.h>
#include<ccdk/mpl/mcontainer/algorithm/index_.h>
#include<ccdk/mpl/mcontainer/forward_.h>
#include<ccdk/mpl/mcontainer/backward_.h>


using namespace ccdk::mpl;
using namespace ccdk;

template<typename T, typename... Args>
struct replace :apply_lambda_<T, Args...>{};

template<typename Left, typename Right>
struct tree {};

struct is_same_f :lambda_< is_same<_1, _2> > {};
struct pred : lambda_< if_< is_same<__,__>, _3, _4> > {};

struct get_second
{
	template<typename T1,typename T2>
	struct apply
	{
		typedef T2 type;
	};
};

int main()
{
	DebugNewTitle("test lambda_")
	int v = 0;
	typedef replace< derive_if< is_same<__, __>, _1, _2 >, uint32_<1>, uint32_<1>> replace1;
	typedef replace< derive_if< is_same<__, __>, _1, _2 >, uint32_<1>, uint32_<0>> replace2;
	AssertTrue(replace1::value);
	AssertFalse(replace2::value);

	DebugNewTitle("test count_ ");
	typedef arg_pack<int, float, char, double, float> arg1;
	typedef count_if_< arg1, lambda_<is_same<__,float> > > cf1;
	typedef count_if_< arg1, add_apply_<is_float> > cf2;
	AssertTrue((count_v< arg1, char>) == 1);
	AssertTrue((count_v< arg1, wchar>) == 0);
	AssertTrue(cf1::value == 2);
	AssertTrue(cf2::value == 3);

	DebugNewTitle("test find_");
	typedef range_<uint32, 2, 10> range1;
	AssertTrue(( deref_t<find_t<range1, uint32_<5>>>::value) == 3 );

	DebugNewTitle("test fold_");
	typedef range_<uint32, 1, 6> range2;
	DebugTypeName< fold_left_t< range2, int, tree>>();
	DebugTypeName< fold_right_t< range2, int, tree>>();
	DebugTypeName< reverse_fold_left_t< range2, int, tree>>();
	DebugTypeName< reverse_fold_right_t< range2, int, tree>>();

	DebugNewTitle("test index_ ");
	AssertTrue((index_v< arg1, float > == 1));
	AssertTrue((index_v< arg1, double > == 3));
	AssertTrue((index_v< arg1, wchar > == -1));

	getchar();
	return 0;
}