
#include<ccdk/debug_type.hpp>
#include<ccdk/mpl/base/arithmetic_.h>
#include<ccdk/mpl/base/placeholder.h>
#include<ccdk/mpl/base/if_.h>
#include<ccdk/mpl/type_traits/is_float.h>
#include<ccdk/mpl/type_traits/is_same.h>
#include<ccdk/mpl/mcontainer/arg_pack.h>
#include<ccdk/mpl/mcontainer/val_pack.h>
#include<ccdk/mpl/mcontainer/range_.h>
#include<ccdk/mpl/mcontainer/slist_.h>
#include<ccdk/mpl/mcontainer/random_.h>
#include<ccdk/mpl/mcontainer/algorithm/lambda_.h>
#include<ccdk/mpl/mcontainer/algorithm/count_.h>
#include<ccdk/mpl/mcontainer/algorithm/find_.h>
#include<ccdk/mpl/mcontainer/algorithm/fold_.h>
#include<ccdk/mpl/mcontainer/algorithm/index_.h>
#include<ccdk/mpl/mcontainer/view/filter_view_.h>
#include<ccdk/mpl/mcontainer/forward_.h>
#include<ccdk/mpl/mcontainer/backward_.h>
#include<ccdk/mpl/mcontainer/random_.h>


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

struct eof_ {};

template<typename Index, typename... Args> struct arg_test;

template<typename T0, typename... Args> 
struct arg_test<uint32_<0>,T0, Args...>
{ 
	static constexpr bool value = true; 
	typedef T0 type;
};


template<typename T0, typename T1, typename... Args> 
struct arg_test<uint32_<1>,T0,T1, Args...>
{
	static constexpr bool value = true; 
	typedef T1 type;
};


template<typename T0, typename T1, typename T2, typename... Args>
struct arg_test<uint32_<2>,T0,T1,T2,Args...>
{ 
	static constexpr bool value = true;
	typedef T2 type;
};

template<typename T0, typename T1, typename T2, typename T3, typename... Args>
struct arg_test<uint32_<3>, T0, T1, T2,T3, Args...>
{
	static constexpr bool value = true;
	typedef T3 type;
};

template<typename Index, typename T0, typename T1, typename T2, typename T3, typename T4, typename... Args> 
struct arg_test< Index,T0,T1,T2,T3, T4, Args...>
{
	static constexpr bool value = false; 
	typedef typename arg_test<uint32_<Index::value-4>,T4,Args...>::type type;
};

int main()
{
	DebugNewTitle("test lambda_")
	int v = 0;
	typedef apply_t< lambda_< derive_if< is_same<__, __>, _1, _2 >>, uint32_<1>, uint32_<1>> replace1;
	typedef apply_t< lambda_< derive_if< is_same<__, __>, _1, _2 >>, uint32_<1>, uint32_<0>> replace2;
	DebugTypeName< apply_t< lambda_< is_same<__, __>>, int, float>>();
 
	AssertTrue(replace1::value);
	AssertFalse(replace2::value);

	DebugNewTitle("test count_ ");
	typedef arg_pack<int, float, char, double, float> arg1;
	typedef count_if_< arg1, is_same<__,float>  > cf1;
	typedef count_if_< arg1, add_apply_<is_float> > cf2;
	AssertTrue((count_v< arg1, char>) == 1);
	AssertTrue((count_v< arg1, wchar>) == 0);
	AssertTrue(cf1::value == 2);
	AssertTrue(cf2::value == 3);

	DebugNewTitle("test find_");
	typedef range_<uint32, 2, 10> range1;
	AssertTrue((deref_t<find_t<range1, uint32_<5>>>::value) == 5 );
	AssertTrue((deref_t<find_if_t<range1, is_same<__, uint32_<5>>>>::value) == 5);
	
	DebugNewTitle("test fold_");
	DebugTypeName< fold_left_t< arg1, eof_,  tree<__,__> >>();
	DebugTypeName< fold_right_t< arg1, eof_, tree<__,__>>>();
	DebugTypeName< reverse_fold_left_t< arg1, eof_, tree<__, __>>>();
	DebugTypeName< reverse_fold_right_t< arg1, eof_, tree<__, __>>>();

	DebugNewTitle("test index_ ");
	AssertTrue((index_v< arg1, float > == 1));
	AssertTrue((index_v< arg1, double > == 3));
	AssertTrue((index_v< arg1, wchar > == -1));
	AssertTrue((index_if_v< arg1, is_float<__> > == 1));

	DebugNewTitle("test filter view ");
	typedef arg_pack<float, int, char, double, short, long, long long, long double> arg2;
	DebugTypeName< fold_left_t< filter_view_< arg2, is_null<__>>, eof_, tree<__,__> >>();
	DebugTypeName< fold_left_t< filter_view_< arg2, is_integer<__>>, eof_, tree<__, __> >>();

	DebugNewTitle("test slist_");
	typedef slist_init_t< float, int, char, double, short> slist1;
	DebugTypeName< slist1 >();
	DebugValue((count_if_v<slist1, is_float<__>>));

	DebugNewTitle("test arg_pack");
	typedef arg_pack<int, float, double, short, long , long long> pack2;
	typedef arg_pack<char, short> pack3;
	DebugSubTitle("test backward");
	DebugTypeName<pop_back_t<pop_back_t<pack2>>>();
	DebugTypeName<push_back_t<push_back_t<pack2, double>,char>>();
	DebugTypeName<back_t<pack2>>();
	DebugSubTitle("test merge");
	DebugTypeName<merge_t<pack2, pack3>>();
	DebugSubTitle("test split");
	DebugTypeName<split_head_t<pack2, 0>>();
	DebugTypeName<split_tail_t<pack2, 0>>();
	DebugTypeName<split_head_t<pack2, 3>>();
	DebugTypeName<split_tail_t<pack2, 3>>();
	DebugTypeName<split_head_t<pack2, 5>>();
	DebugTypeName<split_tail_t<pack2, 5>>();
	DebugTypeName<split_head_t<pack2, 6>>();
	DebugTypeName<split_tail_t<pack2, 6>>();

	DebugSubTitle("test insert");
	DebugTypeName<insert_at_t<pack2, 0, char>>();
	DebugTypeName<insert_at_t<pack2, 3, char>>();
	DebugTypeName<insert_at_t<pack2, 5, char>>();
	DebugTypeName<insert_at_t<pack2, 6, char>>();
	DebugSubTitle("test at");
	DebugTypeName<at_t<pack2, 0>>();
	DebugTypeName<at_t<pack2, 3>>();
	DebugTypeName<at_t<pack2, 5>>();

	DebugSubTitle("test erase");
	DebugTypeName<erase_t<pack2, 5>>();
	DebugTypeName<erase_t<pack2, 0>>();
	DebugTypeName<erase_t<pack2, 1, 3>>();
	getchar();
	return 0;
}