 
#include<ccdk/mpl/function/prediction.h>
#include<ccdk/container/map.h>
#include<ccdk/mpl/util/impl/has_member_decl.h>
#include<stdio.h>
#include<string>

using namespace ccdk;
using namespace ccdk::mpl;
using namespace ccdk::ct;

struct no_less_t {};
struct has_less_no_ret_t { void less(int) const {  } };
struct has_less_t { bool less(int) const { return true; } };
struct has_greater_t { bool greater(int) const { return true; } };

int main()
{
	using simap = map<std::string, int>;
	using iimap = map<int, int>;

	DebugNewTitle("less");
	{
		DebugValue(has_less_v<int, int, bool>);
		DebugValue(has_less_v<int, float, bool>);
		DebugValue(has_less_v<std::string, std::string, bool>);
		util::less(std::string{}, std::string{});
		//util::less(no_less_t{}, 0);
		util::less(has_less_t{}, 0);
		//util::less(0, has_less_t{});
		//util::less(has_less_no_ret_t{}, 0);
		util::less(0, has_greater_t{});
		DebugValue(util::less("abcd", "abcc"));
		DebugValue(util::less("abcd", "abce"));
		DebugValue(util::less("abcd", "abcd"));
	}
	DebugNewTitle("ctor");
	{
		DebugSubTitle("default");
		{
			simap m{};
		}
		DebugNewTitle("array / range ctor");
		{
			simap m{ {{"red",1},{"green",2}, {"blue",3}} };
			m.debug_value();
			m.print_tree();
		}
		DebugNewTitle("path test");
		{
			/*iimap m{};
			for (int i = 1; i < 100; ++i) {
				DebugValue(i);
				m.insert({ i,i });
			}*/
		}
	}

	
	getchar();
	ccdk_open_leak_check();
	return 0;
}


