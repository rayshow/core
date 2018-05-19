#include<ccdk/mpl/util/equals.h>
#include<ccdk/mpl/util/hash.h>

using namespace ccdk::mpl::util;
using namespace ccdk;

class to_hash_test {
public:
	ptr::size_t to_hash() const {
		return 0;
	}
};

class equals_test {
public:

	bool equals(uint32 a) const noexcept {
		return false;
	}
};

struct test_not_equal {
	int a = 0;
};

bool operator==(test_not_equal t, int i) { return t.a == i; }
bool operator!=(test_not_equal t, int i) {
	DebugValue("special not equal");
	return t.a != i;
}


int main()
{
	DebugNewTitle("test hash.");
	DebugValue(hash(to_hash_test{}));
	DebugValue(hash(1));
	DebugValue(hash(2.0f));
	
	DebugNewTitle("test equals.");
	DebugValue(equals(1, 2));
	DebugValue(equals(1, 1));
	DebugValue(equals(1, 1.0f));
	DebugValue(equals(equals_test{}, 1));

	DebugNewTitle("equals");
	{
		DebugValue(equals(1, 2));
		DebugValue(equals("hello", "world"));
		const char *a = "hello";
		const char *b = "hello";
		DebugValue(equals(a, b));
		equals_test et{};
		DebugValue(equals(et, 1));
		DebugValue(equals(et, 0));
		DebugValue(equals(0, et));
	}
	DebugNewTitle("test not equal");
	{
		test_not_equal a{};
		a != 1;
	}

	getchar();
	return 0;
}