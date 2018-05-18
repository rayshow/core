#include<ccdk/mpl/base/bit_alg.h>

using namespace ccdk::mpl;
using namespace ccdk;

int main() {
	DebugNewTitle("uint8");
	{
		DebugValue("");
		using u8_alg = bit_alg<uint8>;
		for (uint8 i = 0; i < uint8(-1); ++i) {
			DebugValueIt(":", (uint32)i);
			DebugValueIt((uint32)u8_alg::flp2(i), (uint32)u8_alg::clp2(i));
			DebugValueItEnd();
		}

		getchar();
	}
}