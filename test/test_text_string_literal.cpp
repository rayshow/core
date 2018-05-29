
#include<ccdk/text/string_literial.h>
#include<stdio.h>

using namespace ccdk;
using namespace ccdk::mpl;
using namespace ccdk::txt;
using namespace ccdk::mpl::literals;

int main()
{
	DebugNewTitle("ctor");
	{
		auto s1 = _literal("fdas" );
		s1.debug_value();
		auto s2 = _literal("hello,world");  //default copy-ctor is enough
		s2.debug_value();
		RuntimeAssertTrue(s1.size() == 4);
		RuntimeAssertTrue(s2.size() == 11);

		for (uint32 i = 0; i < s2.size(); ++i) {
			DebugValueIt(s2[i]);
		}
		DebugValueItEnd();
		s2[0] = 'H';
		s2[1] = 'E';
		s2.debug_value();

		constexpr auto s3 = _literal("hello,world");
		auto s4 = s3;
		DebugValue(s3[1]);
		DebugValue(s3[1_th]);
		
		RuntimeAssertTrue(s4 == s3);
		RuntimeAssertTrue(s2 != s3);
	}
	DebugNewTitle("substr");
	{
		auto s1 = _literal("hello,world");
		auto ss1 = s1.substr<0,3>();
		ss1.debug_value();
	}

	getchar();
	return 0;
}