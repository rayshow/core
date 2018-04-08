#include<ccdk/mpl/fusion/local_obj.h>

using namespace ccdk;
using namespace ccdk::mpl;

struct test_local_obj {
	int a;
	
	test_local_obj() {
		DebugValue("construct");
	}

	void call_fn() {
		DebugValue("a val:", a);
	}

	~test_local_obj() {
		DebugValue("destruct");
	}

};

int main()
{
	{
		fs::local_obj<test_local_obj>  lo{};
		lo.construct();
		lo->a = 2;
		lo->call_fn();
		DebugValue(lo.address());
	}
	
	getchar();
	return 0;
}