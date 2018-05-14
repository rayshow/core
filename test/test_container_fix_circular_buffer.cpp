#include<ccdk/container/fix_circular_buffer.h>
#include<string>

using namespace ccdk;
using namespace ccdk::mpl;
using namespace ccdk::ct;

int main() {
	using fcb10 = fix_circular_buffer<std::string, 10>;

	DebugNewTitle("empty op");
	{
		DebugSubTitle("empty push back");
		{
			fcb10 cb1{};
			cb1.push_back("1");
			cb1.push_back("2");
			cb1.debug_all();
			ccdk_assert(cb1.size() == 2);
		}
		DebugSubTitle("empty push front");
		{
			fcb10 cb1{};
			cb1.push_front("0");
			cb1.push_front("-1");
			cb1.push_front("-2");
			cb1.debug_all();
			ccdk_assert(cb1.size() == 3);
		}
		DebugSubTitle("empty pop back");
		{
			fcb10 cb1{};
			cb1.pop_back();
			ccdk_assert(cb1.size() == 0);
			ccdk_assert(cb1.empty());
		}
		DebugSubTitle("empty front");
		{
			fcb10 cb1{};
			cb1.pop_front();
			ccdk_assert(cb1.size() == 0);
			ccdk_assert(cb1.empty());
		}
		DebugSubTitle("empty assign array");
		{
			fcb10 cb1{};
			cb1 = { "hello", "world" };
			cb1.debug_all();
			ccdk_assert(cb1.size() == 2);
			ccdk_assert(!cb1.empty());
		}
		DebugSubTitle("empty assign fill");
		{
			fcb10 cb1{};
			cb1.assign(5, "java");
			cb1.debug_all();
			ccdk_assert(cb1.size() == 5);
			ccdk_assert(!cb1.empty());
		}
		DebugSubTitle("empty assign copy");
		{
			fcb10 cb1{};
			fcb10 cb2{ {"hello","world"} };
			cb1 = cb2;
			cb1.debug_all();
			ccdk_assert(cb1.size() == 2);
			ccdk_assert(!cb1.empty());
		}
		DebugSubTitle("empty assign move");
		{
			fcb10 cb1{};
			fcb10 cb2{ {"hello","world"} };
			cb1 = util::move(cb2);
			cb1.debug_all();
			cb2.debug_all();
			ccdk_assert(cb1.size() == 2);
			ccdk_assert(cb2.size() == 0);
		}
	}
	DebugNewTitle("ctor");
	{
		DebugSubTitle("move");
		{
			fcb10 cb1{ { "hello","world" } };
			fcb10 cb2{ util::move(cb1) };
			cb1.debug_all();
			cb2.debug_all();
			ccdk_assert(cb1.size() == 0);
			ccdk_assert(cb2.size() == 2);
		}
		DebugSubTitle("copy");
		{
			fcb10 cb1{ { "hello","world" } };
			fcb10 cb2{ cb1 };
			cb1.debug_all();
			cb2.debug_all();
			ccdk_assert(cb1.size() == 2);
			ccdk_assert(cb2.size() == 2);
		}
	}
	DebugNewTitle("swap");
	{
		fcb10 cb1{ { "hello","world" } };
		fcb10 cb2{ { "java","c++" } };
		cb1.debug_all();
		cb2.debug_all();
		cb1.swap(cb2);
		cb1.debug_all();
		cb2.debug_all();
	}
	DebugNewTitle("assign");
	{
		DebugSubTitle("copy assign");
		{
			fcb10 cb1{ { "1","2","3" } };
			fcb10 cb2{ { "4","5" } };
			cb2.debug_all();
			cb2 = cb1;
			cb2.debug_all();
			ccdk_assert(cb2.size() == 3);
		}
		DebugSubTitle("move assign");
		{
			fcb10 cb1{ { "1","2","3" } };
			fcb10 cb2{ { "4","5" } };
			cb2.debug_all();
			cb2 = util::move(cb1);
			cb2.debug_all();
			ccdk_assert(cb2.size() == 3);
			ccdk_assert(cb1.size() == 0);
		}
		DebugSubTitle(" assign list");
		{
			fcb10 cb2{ { "4","5" } };
			cb2.debug_all();
			cb2 = { "1","2","3" };
			cb2.debug_all();
			ccdk_assert(cb2.size() == 3);
		}
		DebugSubTitle(" assign fill");
		{
			fcb10 cb2{ { "3", "4", "5" } };
			cb2.debug_all();
			cb2.assign(4, "6");
			cb2.debug_all();
		}
	}
	DebugNewTitle("element access");
	{
		DebugSubTitle("at/index");
		{
			fcb10 cb2{ { "1", "2", "3", "3", "4", "5" } };
			cb2.pop_front();
			cb2.pop_back();
			ccdk_assert(cb2[0] == "2");
			ccdk_assert(cb2[3] == "4");
		}
		DebugSubTitle("back/front");
		{
			fcb10 cb{ { "1", "2", "3", "3", "4", "5" } };
			ccdk_assert(cb.front() == "1");
			ccdk_assert(cb.back() == "5");
			ccdk_assert(cb.back(1) == "4");
			ccdk_assert(cb.back(2) == "3");
			cb.push_front("0");
			cb.push_front("-1");
			ccdk_assert(cb.front() == "-1");
			cb.pop_front();
			ccdk_assert(cb.front() == "0");
		}
	}
	DebugNewTitle("iterator");
	{
		DebugSubTitle("forward it");
		{
			fcb10 cb{ { "1", "2", "3", "3", "4", "5" } };
			for (auto it = cb.begin(); it != cb.end(); ++it) {
				DebugValueIt(*it);
			}
			DebugValueItEnd();
			ccdk_assert(cb.begin() == cb.cbegin());
			ccdk_assert(*(cb.end() - 1) == "5");
		}
		DebugSubTitle("backward it");
		{
			fcb10 cb{ { "1", "2", "3", "3", "4", "5" } };
			for (auto it = cb.rbegin(); it != cb.rend(); ++it) {
				DebugValueIt(*it);
			}
			DebugValueItEnd();
		}
	}

	ccdk_open_leak_check();
	getchar();
}