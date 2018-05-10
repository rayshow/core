
#include<ccdk/container/vector.h>
#include<ccdk/mpl/function/prediction.h>
#include<stdio.h>
#include<string>

using namespace ccdk;
using namespace ccdk::mpl;
using namespace ccdk::ct;

struct no_trivial {
	int a = 0;

	no_trivial() noexcept { DebugValue("no_trivial construct"); }

	~no_trivial() { DebugValue("no_trivial destruct"); }
};

struct implace_test {
public:
	int a;
	float b;

	implace_test(implace_test const& t) : a{ t.a }, b{ t.b } {
		DebugValue("implace_test copy construct");
	}

	implace_test(implace_test && t) : a{ t.a }, b{ t.b } {
		DebugValue("implace_test move construct");
	}

	void operator=(implace_test&& t) {
		a = t.a;
		b = t.b;
		DebugValue("implace_test assign &&");
	}
	void operator=(implace_test const&) {
		DebugValue("implace_test assign const&");
	}
	implace_test() :a{}, b{} { DebugValue("no-parameter constructor "); }
	implace_test(int a, float b) :a{ a }, b{ b } {
		DebugValue("parameter constructor ");
	}
};


struct leak_test {
public:
	int * a;

	~leak_test(){ if (a) delete a; }

	operator int() {
		return *a;
	}

	leak_test(leak_test const& t) : a{ new int  } {
		*a = *t.a;
		DebugValue("implace_test copy construct");
	}

	leak_test(leak_test && t) : a{ t.a } {
		t.a = nullptr;
		DebugValue("implace_test move construct");
	}

	void operator=(leak_test&& t) {
		this->~leak_test();
		a = t.a;
		t.a = nullptr;
		DebugValue("implace_test assign &&");
	}
	void operator=(leak_test const& t) {
		*a = *t.a;
		DebugValue("implace_test assign const&");
	}
	leak_test() :a{ new int } { *a = 0; DebugValue("no-parameter constructor "); }
	leak_test(int b) :a{ new int } {
		*a = b;
		DebugValue("parameter constructor ");
	}
};


void vector_test() {
	//for (uint32 i = 0; i < 1; ++i) {
	//	DebugNewTitle("empty constructor");
	//	{
	//		vector<int> ivec{};
	//		vector<std::string> svec{};
	//	}

	//	DebugNewTitle(" fill constructor");
	//	{
	//		DebugSubTitle("fill trivial");
	//		{
	//			vector<int> ivec(10, 0);
	//			ivec.debug_value();
	//		}

	//		DebugSubTitle("fill trivial byte");
	//		{
	//			vector<char> ivec(10, '0');
	//			ivec.debug_value();
	//		}

	//		DebugSubTitle("fill non-trivial");
	//		{
	//			vector<std::string> svec{ 10, "hello" };
	//			svec.debug_value();
	//		}
	//	}

	//	DebugNewTitle(" iterator constructor");
	//	{
	//		vector<int> ivec1(10, 2);
	//		vector<int, units::identity> ivec2(ivec1.begin(), ivec1.begin() + 3);
	//		vector<int, units::ratio<2, 1>> ivec3(ivec1.begin(), ivec1.begin() + 3);
	//		RuntimeAssertTrue(ivec2.size() == 3);
	//		//RuntimeAssertTrue(ivec2.capacity() == ivec2.kLeastElements);
	//		RuntimeAssertTrue(ivec3.size() == 3);
	//		//RuntimeAssertTrue(ivec3.capacity() == ivec3.kLeastElements);
	//		ivec2.debug_value("ivec2:");
	//		ivec3.debug_value("ivec3:");


	//		vector<std::string> svec1{ 10, "hello" };
	//		vector<std::string, units::ratio<3, 1>> svec2{ svec1.begin(), svec1.end() };
	//		svec2.debug_value("string range:");

	//	}
	//	DebugNewTitle(" copy/move constructor");
	//	{
	//		vector<int> ivec1(10, 2);
	//		vector<int> ivec2(ivec1);
	//		vector<int> ivec3(util::move(ivec1));
	//		vector<int, units::ratio<2, 1>> ivec4{ ivec2 };
	//		vector<int, units::ratio<2, 1>> ivec5{ util::move(ivec2) };
	//		RuntimeAssertTrue(ivec1.empty());
	//		RuntimeAssertTrue(ivec2.empty());
	//		ivec1.debug_value("ivec1:");
	//		ivec2.debug_value("ivec2:");
	//		ivec3.debug_value("ivec3:");
	//		ivec4.debug_value("ivec4:");
	//		ivec5.debug_value("ivec5:");
	//	}
	//	DebugNewTitle("array constructor");
	//	{
	//		vector<int> ivec1{ { 1,2 } };
	//		ivec1.debug_value();
	//		vector<std::string> svec1{ { "hello", "world" } };
	//		svec1.debug_value();
	//		RuntimeAssertTrue(svec1.end() - 1 == svec1.crbegin());
	//	}
	//	DebugNewTitle("swap");
	//	{
	//		vector<int> ivec1(10, 1);
	//		vector<int> ivec2(10, 2);
	//		RuntimeAssertTrue(ivec1[0] == 1);
	//		RuntimeAssertTrue(ivec2[0] == 2);
	//		ivec1.debug_value("ivec1:");
	//		ivec2.debug_value("ivec2:");
	//		util::swap(ivec1, ivec2);
	//		RuntimeAssertTrue(ivec1[0] == 2);
	//		RuntimeAssertTrue(ivec2[0] == 1);
	//		ivec1.debug_value("ivec1:");
	//		ivec2.debug_value("ivec2:");
	//	}
	//	DebugNewTitle("copy assign");
	//	{
	//		DebugSubTitle("copy only");
	//		{
	//			vector<int> ivec1(10, 1);
	//			vector<int> ivec2(10, 2);
	//			ivec2 = nullptr;
	//			ivec2.debug_value("ivec2:");
	//			ivec2 = ivec1;
	//			ivec2.debug_value("ivec2:");
	//		}
	//		DebugSubTitle("template copy only");
	//		{
	//			vector<int> ivec1(10, 1);
	//			vector<int, units::ratio<3, 1>> ivec2(10, 2);
	//			ivec1 = ivec2;
	//			ivec1.debug_value("ivec1:");
	//		}
	//		DebugSubTitle("allocate copy");
	//		{
	//			vector<int> ivec1(3, 1);
	//			vector<int> ivec2(10, 2);
	//			ivec1 = ivec2;
	//			ivec1.debug_value("ivec1:");
	//		}
	//		DebugSubTitle("template allocate only");
	//		{
	//			vector<int> ivec1(3, 1);
	//			vector<int, units::ratio<3, 1>> ivec2(10, 2);
	//			ivec1 = ivec2;
	//			ivec1.debug_value("ivec1:");
	//		}
	//	}
	//	DebugNewTitle("move assign");
	//	{
	//		DebugSubTitle("move only");
	//		{
	//			vector<int> ivec1(10, 1);
	//			vector<int> ivec2(10, 2);
	//			ivec2 = util::move(ivec1);
	//			RuntimeAssertTrue(ivec1.empty());
	//		}
	//		DebugSubTitle("template move only");
	//		{
	//			vector<int> ivec1(10, 1);
	//			vector<int, units::ratio<3, 1>> ivec2(10, 2);
	//			ivec1 = util::move(ivec2);
	//			RuntimeAssertTrue(ivec2.empty());
	//		}
	//	}
	//	DebugNewTitle("assign ");
	//	{
	//		vector<int> ivec1(10, 1);
	//		vector<int> ivec2(10, 1);
	//		ivec1.assign(ivec2.begin(), ivec2.end())
	//			.assign(ivec2.begin(), ivec2.size())
	//			.assign(5, 0)
	//			.assign({ 1,2,3 });
	//		ivec1.debug_value("assign:");

	//		vector<std::string> svec1(10, "world");
	//		vector<std::string> svec2(10, "hello");
	//		//std::string s = 0;  //error
	//		svec1.assign(5, "kit")
	//			.assign(svec1.cbegin(), svec1.cend())
	//			.assign({ "hello","my ","world" });
	//		svec1.debug_value("assign:");
	//	}
	//	DebugNewTitle("attribute");
	//	{
	//		vector<int, units::ratio<2, 1>> ivec1(10, 1);
	//		RuntimeAssertTrue(ivec1.size() == 10);
	//		RuntimeAssertTrue(ivec1.capacity() == 20);
	//		RuntimeAssertTrue(ivec1.max_size() == (uint32)(-1));
	//		RuntimeAssertTrue(!ivec1.empty());
	//	}
	//	DebugNewTitle("iterator");
	//	{
	//		vector<int, units::ratio<2, 1>> ivec1(3, 1);
	//		int c = 0;
	//		DebugSubTitle("iterator ");
	//		for (auto it = ivec1.begin(); it != ivec1.end(); ++it, ++c)
	//		{
	//			*it = c;
	//		}
	//		DebugSubTitle("foreach");
	//		for (auto& it : ivec1)
	//		{
	//			DebugValue(it);
	//		}
	//		DebugSubTitle("reverse iterator");
	//		for (auto it = ivec1.rbegin(); it != ivec1.rend(); ++it)
	//		{
	//			DebugValue(*it);
	//		}

	//		// *ivec1.cbegin() =1;
	//		// *ivec1.crbegin() = 1;
	//	}
	//	DebugNewTitle("pop_back");
	//	{
	//		no_trivial nt;
	//		DebugSubTitle("pop no-trivial ");
	//		{
	//			vector<no_trivial> ivec(4, nt);
	//			ivec.pop_back();
	//			RuntimeAssertTrue(ivec.size() == 3);
	//		}
	//		DebugSubTitle("pop int")
	//		{
	//			vector<int>        ivec1(4, 1);
	//			int c = 1;
	//			for (auto& i : ivec1) { i = c++; }
	//			ivec1.pop_back();
	//			RuntimeAssertTrue(ivec1.back() == 3);
	//			ivec1.pop_back();
	//			RuntimeAssertTrue(ivec1.back() == 2);
	//		}
	//	}
	//	DebugNewTitle("implace back")
	//	{
	//		implace_test a{ 2,3 };

	//		vector<implace_test> vec{ 4, implace_test{ 5,6.f } };
	//		vec.insert(0, a);
	//		for (auto& it : vec)
	//		{
	//			DebugValue(it.a, it.b);
	//		}
	//		vec.insert(0, a);
	//		for (auto& it : vec)
	//		{
	//			DebugValue(it.a, it.b);
	//		}
	//		vec.insert(0, a);
	//		for (auto& it : vec)
	//		{
	//			DebugValue(it.a, it.b);
	//		}
	//	}
	//	DebugNewTitle("insert ");
	//	{
	//		DebugSubTitle("single item");
	//		{
	//			vector<int> ivec1(4, 1);
	//			DebugValue(ivec1.size());
	//			DebugValue(ivec1.capacity());
	//			int c = 1;
	//			for (auto& i : ivec1) { i = c++; }
	//			RuntimeAssertTrue(ivec1[3] == 4);
	//			ivec1.insert(0, -1);
	//			RuntimeAssertTrue(ivec1[0] == -1);
	//			ivec1.insert(ivec1.size() - 1, -1);
	//			RuntimeAssertTrue(ivec1[ivec1.size() - 2] == -1);
	//			ivec1.insert(ivec1.size(), -2);
	//			RuntimeAssertTrue(ivec1[ivec1.size() - 1] == -2);

	//		}
	//		DebugSubTitle("iterator insert");
	//		{
	//			vector<int> ivec1(4, 1);
	//			ivec1.insert(ivec1.end(), -3);
	//			RuntimeAssertTrue(ivec1[ivec1.size() - 2] == 1);
	//			RuntimeAssertTrue(ivec1[ivec1.size() - 1] == -3);
	//		}
	//		DebugSubTitle("iterator-range-insert")
	//		{
	//			vector<int> ivec1(3, 2);
	//			vector<int> ivec2(3, 1);
	//			ivec1.insert(ivec1.begin() + 1, ivec2.begin(), ivec2.end());
	//			ivec1.debug_value("after insert:");
	//			RuntimeAssertTrue(ivec1[0] == 2);
	//			RuntimeAssertTrue(ivec1[1] == 1);
	//			RuntimeAssertTrue(ivec1[3] == 1);
	//			RuntimeAssertTrue(ivec1[4] == 2);
	//			RuntimeAssertTrue(ivec1.size() == 6);

	//		}
	//		DebugSubTitle("pos range");
	//		{
	//			vector<int> ivec1(4, 1);
	//			vector<int> ivec2(3, 2);
	//			ivec1.insert(1, ivec2.begin(), ivec2.end());
	//			RuntimeAssertTrue(ivec1[0] == 1);
	//			RuntimeAssertTrue(ivec1[1] == 2);
	//			RuntimeAssertTrue(ivec1[3] == 2);
	//			RuntimeAssertTrue(ivec1[4] == 1);
	//			RuntimeAssertTrue(ivec1.size() == 7);
	//			ivec1.insert(0, { -1,-1 });
	//			RuntimeAssertTrue(ivec1[0] == -1);
	//			RuntimeAssertTrue(ivec1.size() == 9);
	//		}
	//	}
	//	DebugNewTitle("erase");
	//	{
	//		vector<int> ivec1(5, 1);
	//		for (int i = 0; i < ivec1.size(); ++i) ivec1[i] = i;
	//		ivec1.erase(1, 3);
	//		RuntimeAssertTrue(ivec1[0] == 0);
	//		RuntimeAssertTrue(ivec1[1] == 3);
	//		RuntimeAssertTrue(ivec1.size() == 3);
	//		ivec1.erase(ivec1.begin());
	//		RuntimeAssertTrue(ivec1.empty());
	//	}
	//	DebugNewTitle("clear");
	//	{
	//		vector<int> ivec1(5, 1);
	//		ivec1.clear();
	//		RuntimeAssertTrue(ivec1.empty());
	//	}
	//	DebugNewTitle("big push back");
	//	{
	//		vector<leak_test> ivec{};
	//		for (uint32 i = 0; i < 9; ++i) {
	//			ivec.emplace(0, i);
	//			for (auto& i : ivec) {
	//				DebugValueIt(*i.a);
	//			}
	//		}
	//		DebugValue(ivec.size());
	//	}
	//	/*DebugNewTitle("big push back 2");
	//	{
	//	vector<std::string> ivec{};
	//	char str[10];
	//	for (uint32 i = 0; i < 300; ++i) {
	//	sprintf_s(str, "%d", i);
	//	ivec.emplace(0, str);
	//	}
	//	DebugValue(ivec.size());
	//	ivec.debug_value("big insert");
	//	}*/
	//	DebugNewTitle("big push back 3");
	//	{
	//		vector<std::string> ivec{};
	//		char str[10];
	//		for (uint32 i = 0; i < 20; ++i) {
	//			sprintf_s(str, "%d", i);
	//			vector<std::string> ivec2{ 2,str };
	//			ivec.insert(0, ivec2.begin(), ivec2.end());
	//		}
	//		DebugValue(ivec.size());
	//		ivec.erase(10, 30);
	//		DebugValue(ivec.size());
	//		ivec.debug_value("big insert");
	//	}

	//	DebugNewTitle("find nullptr");
	//	{
	//		vector<int*> pvec{ 5,nullptr };
	//		pvec.find_index(fn::not_null);
	//	}
	//}
}


void local_vector_test()
{
	using ivec10 = local_vector<int, 10>;
	using svec10 = local_vector<std::string, 10>;

	DebugNewTitle("ctor");
	{
		DebugSubTitle("empty constructor");
		{
			svec10 svec{};
			svec.emplace_back("hello");
			svec.emplace_back("world");
			svec.debug_value();
		}
		DebugSubTitle("fill ctor");
		{
			svec10 svec1{ 10, "java" };
			svec1.debug_is_stack();
			svec1.debug_value();
			svec1.push_back({ "c++", "scala" });
			svec1.debug_is_stack();
			svec1.debug_value();
		}
		DebugSubTitle(" copy/move constructor");
		{
			svec10 s1{ 10, "c++" };
			svec10 s2{ util::move(s1) };
			s1.debug_value();
			s2.debug_value();
			s2.emplace_back("java");
			svec10 s3{ util::move(s2) };
			s2.debug_value();
			s3.debug_value();
		}
		DebugSubTitle("array / iterator ctor");
		{
			svec10 s1{ { "1", "2","3","4","5","6","7","8","9","10" } };
			s1.debug_value();
			svec10 s2{ s1.begin(), s1.end() };
			s2.debug_value();
		}
	}
	DebugNewTitle("swap");
	{
		DebugSubTitle("stack swap stack");
		{
			svec10 s1{ { "1", "2","3" } };
			svec10 s2{ { "4", "5","6" } };
			ccdk_assert(s1.is_stack() && s2.is_stack());
			s1.debug_value();
			s2.debug_value();
			s1.swap(s2);
			s1.debug_value();
			s2.debug_value();
		}
		DebugSubTitle("heap swap heap");
		{
			svec10 s1{ 10, "1" };
			svec10 s2{ 10, "2" };
			s1.push_back("1");
			s2.push_back("2");
			ccdk_assert(!s1.is_stack() && !s2.is_stack());
			s1.debug_value();
			s2.debug_value();
			s1.swap(s2);
			s1.debug_value();
			s2.debug_value();
			
		}
		DebugSubTitle("heap swap stack")
		{
			svec10 s1{ 3, "1" };
			svec10 s2{ 10, "2" };
			s2.push_back("2");
			ccdk_assert(s1.is_stack() && !s2.is_stack());
			
			s1.debug_value();
			s2.debug_value();
			s1.swap(s2);
			s1.debug_value();
			s2.debug_value();
		}
	}
	DebugNewTitle(" assign");
	{
		DebugSubTitle("stack copy stack");
		{
			svec10 s1(10, "c++");
			svec10 s2(10, "java");
			ccdk_assert(s1.is_stack() && s2.is_stack());
			s1 = s2;
			s1.debug_value();
			s2.debug_value();
			ccdk_assert(s1.is_stack() && s2.is_stack());
		}
		DebugSubTitle("heap copy stack");
		{
			svec10 s1(10, "c++");
			svec10 s2(10, "java");
			s1.push_back("java");
			ccdk_assert(!s1.is_stack() && s2.is_stack());
			s1 = s2;
			s1.debug_value();
			s2.debug_value();
			ccdk_assert(!s1.is_stack() && s2.is_stack());
		}
		DebugSubTitle("stack copy heap");
		{
			svec10 s1(10, "c++");
			svec10 s2(10, "java");
			s2.push_back("java");
			ccdk_assert(s1.is_stack() && !s2.is_stack());
			s1 = s2;
			s1.debug_value();
			s2.debug_value();
			ccdk_assert(!s1.is_stack() && !s2.is_stack());
		}
		DebugSubTitle("heap copy heap");
		{
			svec10 s1(10, "c++");
			svec10 s2(10, "java");
			s2.push_back("java");
			s1.push_back({ "c++", "c++" });
			ccdk_assert(!s1.is_stack() && !s2.is_stack());
			s1 = s2;
			s1.debug_value();
			s2.debug_value();
		}

		DebugSubTitle("move");
		{
			DebugSubTitle("stack move to ");
			{
				svec10 s1(10, "c++");
				svec10 s2(10, "java");
				s1 = util::move(s2);
				s1.debug_value();
				s1.debug_value();
			}
			DebugSubTitle("heap move to ");
			{
				svec10 s1(10, "c++");
				svec10 s2(10, "java");
				s2.push_back("java");
				s1 = util::move(s2);
				s1.debug_value();
				s2.debug_value();
			}
		}
		DebugSubTitle("fill-n");
		{
			svec10 s1{};
			s1.assign(10, "c++");
			ccdk_assert(s1.is_stack());
			s1.debug_value();
			s1.assign(20, "java");
			s1.debug_value();
			ccdk_assert(!s1.is_stack());
			s1.assign(5, "scala");
			s1.debug_value();
		}
	}
	//	DebugNewTitle("move assign");
	//	{
	//		DebugSubTitle("move only");
	//		{
	//			vector<int> ivec1(10, 1);
	//			vector<int> ivec2(10, 2);
	//			ivec2 = util::move(ivec1);
	//			RuntimeAssertTrue(ivec1.empty());
	//		}
	//		DebugSubTitle("template move only");
	//		{
	//			vector<int> ivec1(10, 1);
	//			vector<int, units::ratio<3, 1>> ivec2(10, 2);
	//			ivec1 = util::move(ivec2);
	//			RuntimeAssertTrue(ivec2.empty());
	//		}
	//	}
	//	DebugNewTitle("assign ");
	//	{
	//		vector<int> ivec1(10, 1);
	//		vector<int> ivec2(10, 1);
	//		ivec1.assign(ivec2.begin(), ivec2.end())
	//			.assign(ivec2.begin(), ivec2.size())
	//			.assign(5, 0)
	//			.assign({ 1,2,3 });
	//		ivec1.debug_value("assign:");

	//		vector<std::string> svec1(10, "world");
	//		vector<std::string> svec2(10, "hello");
	//		//std::string s = 0;  //error
	//		svec1.assign(5, "kit")
	//			.assign(svec1.cbegin(), svec1.cend())
	//			.assign({ "hello","my ","world" });
	//		svec1.debug_value("assign:");
	//	}
	//	DebugNewTitle("attribute");
	//	{
	//		vector<int, units::ratio<2, 1>> ivec1(10, 1);
	//		RuntimeAssertTrue(ivec1.size() == 10);
	//		RuntimeAssertTrue(ivec1.capacity() == 20);
	//		RuntimeAssertTrue(ivec1.max_size() == (uint32)(-1));
	//		RuntimeAssertTrue(!ivec1.empty());
	//	}
	//	DebugNewTitle("iterator");
	//	{
	//		vector<int, units::ratio<2, 1>> ivec1(3, 1);
	//		int c = 0;
	//		DebugSubTitle("iterator ");
	//		for (auto it = ivec1.begin(); it != ivec1.end(); ++it, ++c)
	//		{
	//			*it = c;
	//		}
	//		DebugSubTitle("foreach");
	//		for (auto& it : ivec1)
	//		{
	//			DebugValue(it);
	//		}
	//		DebugSubTitle("reverse iterator");
	//		for (auto it = ivec1.rbegin(); it != ivec1.rend(); ++it)
	//		{
	//			DebugValue(*it);
	//		}

	//		// *ivec1.cbegin() =1;
	//		// *ivec1.crbegin() = 1;
	//	}
	//	DebugNewTitle("pop_back");
	//	{
	//		no_trivial nt;
	//		DebugSubTitle("pop no-trivial ");
	//		{
	//			vector<no_trivial> ivec(4, nt);
	//			ivec.pop_back();
	//			RuntimeAssertTrue(ivec.size() == 3);
	//		}
	//		DebugSubTitle("pop int")
	//		{
	//			vector<int>        ivec1(4, 1);
	//			int c = 1;
	//			for (auto& i : ivec1) { i = c++; }
	//			ivec1.pop_back();
	//			RuntimeAssertTrue(ivec1.back() == 3);
	//			ivec1.pop_back();
	//			RuntimeAssertTrue(ivec1.back() == 2);
	//		}
	//	}
	//	DebugNewTitle("implace back")
	//	{
	//		implace_test a{ 2,3 };

	//		vector<implace_test> vec{ 4, implace_test{ 5,6.f } };
	//		vec.insert(0, a);
	//		for (auto& it : vec)
	//		{
	//			DebugValue(it.a, it.b);
	//		}
	//		vec.insert(0, a);
	//		for (auto& it : vec)
	//		{
	//			DebugValue(it.a, it.b);
	//		}
	//		vec.insert(0, a);
	//		for (auto& it : vec)
	//		{
	//			DebugValue(it.a, it.b);
	//		}
	//	}
	//	DebugNewTitle("insert ");
	//	{
	//		DebugSubTitle("single item");
	//		{
	//			vector<int> ivec1(4, 1);
	//			DebugValue(ivec1.size());
	//			DebugValue(ivec1.capacity());
	//			int c = 1;
	//			for (auto& i : ivec1) { i = c++; }
	//			RuntimeAssertTrue(ivec1[3] == 4);
	//			ivec1.insert(0, -1);
	//			RuntimeAssertTrue(ivec1[0] == -1);
	//			ivec1.insert(ivec1.size() - 1, -1);
	//			RuntimeAssertTrue(ivec1[ivec1.size() - 2] == -1);
	//			ivec1.insert(ivec1.size(), -2);
	//			RuntimeAssertTrue(ivec1[ivec1.size() - 1] == -2);

	//		}
	//		DebugSubTitle("iterator insert");
	//		{
	//			vector<int> ivec1(4, 1);
	//			ivec1.insert(ivec1.end(), -3);
	//			RuntimeAssertTrue(ivec1[ivec1.size() - 2] == 1);
	//			RuntimeAssertTrue(ivec1[ivec1.size() - 1] == -3);
	//		}
	//		DebugSubTitle("iterator-range-insert")
	//		{
	//			vector<int> ivec1(3, 2);
	//			vector<int> ivec2(3, 1);
	//			ivec1.insert(ivec1.begin() + 1, ivec2.begin(), ivec2.end());
	//			ivec1.debug_value("after insert:");
	//			RuntimeAssertTrue(ivec1[0] == 2);
	//			RuntimeAssertTrue(ivec1[1] == 1);
	//			RuntimeAssertTrue(ivec1[3] == 1);
	//			RuntimeAssertTrue(ivec1[4] == 2);
	//			RuntimeAssertTrue(ivec1.size() == 6);

	//		}
	//		DebugSubTitle("pos range");
	//		{
	//			vector<int> ivec1(4, 1);
	//			vector<int> ivec2(3, 2);
	//			ivec1.insert(1, ivec2.begin(), ivec2.end());
	//			RuntimeAssertTrue(ivec1[0] == 1);
	//			RuntimeAssertTrue(ivec1[1] == 2);
	//			RuntimeAssertTrue(ivec1[3] == 2);
	//			RuntimeAssertTrue(ivec1[4] == 1);
	//			RuntimeAssertTrue(ivec1.size() == 7);
	//			ivec1.insert(0, { -1,-1 });
	//			RuntimeAssertTrue(ivec1[0] == -1);
	//			RuntimeAssertTrue(ivec1.size() == 9);
	//		}
	//	}
	//	DebugNewTitle("erase");
	//	{
	//		vector<int> ivec1(5, 1);
	//		for (int i = 0; i < ivec1.size(); ++i) ivec1[i] = i;
	//		ivec1.erase(1, 3);
	//		RuntimeAssertTrue(ivec1[0] == 0);
	//		RuntimeAssertTrue(ivec1[1] == 3);
	//		RuntimeAssertTrue(ivec1.size() == 3);
	//		ivec1.erase(ivec1.begin());
	//		RuntimeAssertTrue(ivec1.empty());
	//	}
	//	DebugNewTitle("clear");
	//	{
	//		vector<int> ivec1(5, 1);
	//		ivec1.clear();
	//		RuntimeAssertTrue(ivec1.empty());
	//	}
	//	DebugNewTitle("big push back");
	//	{
	//		vector<leak_test> ivec{};
	//		for (uint32 i = 0; i < 9; ++i) {
	//			ivec.emplace(0, i);
	//			for (auto& i : ivec) {
	//				DebugValueIt(*i.a);
	//			}
	//		}
	//		DebugValue(ivec.size());
	//	}
	//	/*DebugNewTitle("big push back 2");
	//	{
	//	vector<std::string> ivec{};
	//	char str[10];
	//	for (uint32 i = 0; i < 300; ++i) {
	//	sprintf_s(str, "%d", i);
	//	ivec.emplace(0, str);
	//	}
	//	DebugValue(ivec.size());
	//	ivec.debug_value("big insert");
	//	}*/
	//	DebugNewTitle("big push back 3");
	//	{
	//		vector<std::string> ivec{};
	//		char str[10];
	//		for (uint32 i = 0; i < 20; ++i) {
	//			sprintf_s(str, "%d", i);
	//			vector<std::string> ivec2{ 2,str };
	//			ivec.insert(0, ivec2.begin(), ivec2.end());
	//		}
	//		DebugValue(ivec.size());
	//		ivec.erase(10, 30);
	//		DebugValue(ivec.size());
	//		ivec.debug_value("big insert");
	//	}

	//	DebugNewTitle("find nullptr");
	//	{
	//		vector<int*> pvec{ 5,nullptr };
	//		pvec.find_index(fn::not_null);
	//	}
}


int main()
{
	vector_test();
	local_vector_test();
	ccdk_open_leak_check();
	getchar();
	return 0;
}
