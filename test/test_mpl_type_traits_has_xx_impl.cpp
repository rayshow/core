
#include<ccdk/type.h>
#include<ccdk/mpl/base/type_.h>
#include<ccdk/mpl/type_traits/declval.h>
#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/base/logic_.h>
#include<ccdk/mpl/type_traits/is_same.h>
#include<ccdk/mpl/type_traits/impl/has_member_decl.h>
#include<ccdk/mpl/type_traits/impl/has_attribute_decl.h>
#include<ccdk/container/slist.h>

using namespace ccdk;
using namespace ccdk::mpl;

class test1 { public: };
class test2 { public: void test() {} };
class test3 { public: bool test() { return false; } };
class test4 { public: void test(int) {} };
class test5 { public: bool test(int) { return false; } };

// test weather ret T.test(args...) is exists
CCDK_TT_HAS_MEMBER_WITH_RET_DECL(has_member_test_ret, test, bool);
CCDK_TT_HAS_MEMBER_DECL(has_member_test, test);

void test_has_member()
{
	DebugNewTitle("with no type");
	DebugValue(has_member_test_v<test1, void>);
	DebugValue(has_member_test_v<test2, void>);
	DebugValue(has_member_test_v<test2, int>);
	DebugValue(has_member_test_v<test3, bool>);
	DebugValue(has_member_test_v<test3, void>);
	DebugValue(has_member_test_v<test4, void, int>);
	DebugValue(has_member_test_v<test4, void>);
	DebugValue(has_member_test_v<test5, bool, int>);
	DebugValue(has_member_test_v<test5, bool, float>);
	DebugNewTitle("with ret defined type");
	DebugValue(has_member_test_ret_v<test1>);
	DebugValue(has_member_test_ret_v<test2>);
	DebugValue(has_member_test_ret_v<test3>);
	DebugValue(has_member_test_ret_v<test4, int>);
	DebugValue(has_member_test_ret_v<test5, int>);

	DebugNewTitle("with ingore ret type");
	DebugValue(has_member_test_ignore_ret_v<test1>);
	DebugValue(has_member_test_ignore_ret_v<test2>);
	DebugValue(has_member_test_ignore_ret_v<test3>);
	DebugValue(has_member_test_ignore_ret_v<test4, int>);
	DebugValue(has_member_test_ignore_ret_v<test5, int>);
}

CCDK_TT_HAS_ATTRIBUTE_DECL(has_value, value);
CCDK_TT_HAS_ATTRIBUTE_DECL(has_next, next);
CCDK_TT_HAS_ATTRIBUTE_WITH_TYPE_DECL(has_prev_link, prev, T*);
CCDK_TT_HAS_ATTRIBUTE_WITH_TYPE_DECL(has_next_link, next, T*);

struct test_value1 {};
struct test_value2 { public: int value; };
struct test_value3 { public: bool value; };
struct test_node { public: test_node * next; };
struct test_node2 { public: test_node2 * prev; };
struct test_biward_node { public: test_biward_node * prev; test_biward_node * next; };
template<typename T>
struct is_biward : and_< has_prev_link<T>, has_next_link<T>> {};

void test_has_attribute()
{
	DebugNewTitle("with type");
	DebugValue(has_value_v<test_value1, int>);
	DebugValue(has_value_v<test_value2, int>);
	DebugValue(has_value_v<test_value3, int>);
	DebugValue(has_value_v<test_value3, bool>);
	DebugNewTitle("ignore type");
	DebugValue(has_value_v<test_value1>);
	DebugValue(has_value_v<test_value2>);
	DebugValue(has_value_v<test_value3>);
	DebugValue(has_value_v<test_value3>);
	DebugNewTitle("test next type");
	DebugValue(has_next_v<test_node,test_node*>);
	DebugNewTitle("test next link");
	DebugValue(has_prev_link_v<test_node2>);
	DebugValue(has_prev_link_v<test_biward_node>);
	DebugNewTitle("test bi-ward node");
	DebugValue(is_biward<test_biward_node>::value);
	DebugValue(is_biward<test_node2>::value);
}

int main()
{
	test_has_member();
	test_has_attribute();

	getchar();
	return 0;
}