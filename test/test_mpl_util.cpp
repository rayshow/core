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

	getchar();
	return 0;
}