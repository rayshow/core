
#include<ccdk/text/string_literial.h>

using namespace ccdk;
using namespace ccdk::mpl;
using namespace ccdk::txt;

int main()
{

	/*constexpr string */
	DebugNewTitle("string literial");
	DebugValueTypeName(_literal("hello,world"));
	constexpr auto sl = _literal("hello,world");
	DebugValue(sl);
	AssertTrue(sl[3_th] == 'l');
	AssertTrue(sl.find_first('l') == 2);
	AssertTrue(sl.find_last('l') == 9);
	AssertTrue((sl.substr<2, 5>()) == _literal("llo"));
	AssertTrue(sl.replace<sl.find_first('w')>('F') == _literal("hello,Forld"));

}