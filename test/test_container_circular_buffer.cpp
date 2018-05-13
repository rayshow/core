#include<ccdk/container/circular_buffer.h>
#include<string>

using namespace ccdk;
using namespace ccdk::mpl;
using namespace ccdk::ct;

int main() 
{
	using scb = circular_buffer<std::string> ;

	DebugNewTitle("empty operation");
	{
		DebugSubTitle("empty push_back");
		{
			scb cb{};
			ccdk_assert(cb.size() == 0);
			ccdk_assert(cb.capcity() == 0);
			ccdk_assert(cb.empty());
		}

		DebugSubTitle("empty push_back");
		{
			scb cb{};
			ccdk_assert(cb.capcity() == 0);
			cb.push_back("hello");
			ccdk_assert(cb.size() == 1);
			cb.debug_all("empty push back");
			DebugValue("cap:", cb.capcity());
		}
		DebugSubTitle("empty push_front");
		{
			scb cb{};
			cb.push_front("hello");
			ccdk_assert(cb.size() == 1);
			cb.debug_all("empty push front");
			DebugValue("cap:", cb.capcity());
		}
		DebugSubTitle("empty assign");
		{
			scb cb{};
			cb = { "hello", "world" };
			ccdk_assert(cb.size() == 2);
			cb.debug_all("empty assign");
			DebugValue("cap:", cb.capcity());
			cb.assign(18, "java");
			cb.debug_all("fill-18");
			ccdk_assert(cb.size() == 18);

		}
		DebugSubTitle("empty pop front");
		{
			scb cb1{};
			cb1.pop_front();
			ccdk_assert(cb1.size() == 0);
			ccdk_assert(cb1.empty());
			scb cb2{ {"hello"} };
			cb2.pop_front();
			ccdk_assert(cb1.size() == 0);
			ccdk_assert(cb1.empty());
			cb2.debug_all("empty pop front");
		}
		DebugSubTitle("empty pop back front");
		{
			scb cb1{};
			cb1.pop_back();
			ccdk_assert(cb1.size() == 0);
			ccdk_assert(cb1.empty());
			scb cb2{ { "hello" } };
			cb2.pop_back();
			ccdk_assert(cb1.size() == 0);
			ccdk_assert(cb1.empty());
			cb2.debug_all("empty pop back");
		}
	}
	DebugNewTitle("ctor");
	{
		DebugNewTitle("fill-n");
		{
			scb cb{ 4,"1" };
			cb.debug_all("cb4");
			ccdk_assert(cb.size() == 4);
		}
		DebugNewTitle("copy / move ctor");
		{
			scb cb{ {"1","2","3","4"} };
			cb.debug_all("cb1-4");
			scb cb2{ cb };
			cb2.debug_all("copy");
			ccdk_assert(cb2.size() == 4);
			scb cb3{ util::move(cb) };
			cb.debug_all("move from");
			cb3.debug_all("move to");
			ccdk_assert(cb.size() == 0 && cb.empty());
			ccdk_assert(cb3.size() == 4);
		}
	}
	DebugNewTitle("swap");
	{
		scb cb{ { "1","2","3","4" } };
		scb cb2{ { "5","6","7" } };
		cb.debug_all("before swap 1");
		cb2.debug_all("before swap 2");
		cb.swap(cb2);
		cb.debug_all("after swap 1");
		cb2.debug_all("after swap 2");
	}
	DebugNewTitle("assign");
	{
		DebugSubTitle("copy / move assign");
		{
			scb cb{ { "1","2","3","4" } };
			scb cb2{ { "5","6","7","8" } };
			scb cb3{ { "9","10"} };
			cb = cb2;
			cb.debug_all();
			cb2.debug_all();
			cb = util::move(cb3);
			cb.debug_all();
			cb3.debug_all();
			ccdk_assert(cb3.size() == 0);
			ccdk_assert(cb3.empty());
		}
		DebugSubTitle("initialize_list assign");
		{
			scb cb{ { "1","2","3","4" } };
			cb = { "5","6","7","8" };
			cb.debug_all();
			ccdk_assert(cb.size() == 4);
		}
		DebugSubTitle("fill assign");
		{
			scb cb{ { "1","2","3","4" } };
			cb.assign(6, "5");
			ccdk_assert(cb.size() == 6);
			cb.debug_all("fill-assign");
		}
	}
	DebugNewTitle("front op");
	{
		scb cb{};
		for (uint32 i = 0; i < 33; ++i) {
			cb.push_front(std::to_string(i));
		}
		while (!cb.empty()) {
			DebugValueIt(cb.front());
			cb.pop_front();
		}
	}
	DebugNewTitle("back op");
	{
		scb cb{};
		for (uint32 i = 0; i < 33; ++i) {
			cb.push_back(std::to_string(i));
		}
		while (!cb.empty()) {
			DebugValueIt(cb.back());
			cb.pop_back();
		}
	}
	DebugNewTitle("iterator");
	{
		DebugSubTitle("forward visit");
		{
			scb cb{};
			for (uint32 i = 0; i < 33; ++i) {
				cb.push_back(std::to_string(i));
			}
			for (auto it = cb.begin(); it != cb.end(); ++it) {
				DebugValueIt(*it);
			}
			DebugValueItEnd();
			ccdk_assert(*(cb.begin() + 2) == "2");
			ccdk_assert(*(cb.end() - 2) == "31");
			ccdk_assert(cb.begin() == cb.cbegin());
			scb::const_iterator cit = cb.begin();
		}
		DebugSubTitle("reverse visit");
		{
			scb cb{};
			for (uint32 i = 0; i < 33; ++i) {
				cb.push_back(std::to_string(i));
			}
			for (auto it = cb.rbegin(); it != cb.rend(); ++it) {
				DebugValueIt(*it);
			}
			DebugValueItEnd();
		}
	}
	getchar();
	ccdk_open_leak_check();
}