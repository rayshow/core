
#include<ccdk/mpl/design_pattern/singleton.h>

using namespace ccdk::mpl::dp;
using namespace ccdk;

class GraphicsManager
	: public singleton<GraphicsManager>
{
public:
	void setup() {
		DebugValue("setup");
	}
	int val = 2;
};


int main()
{
	GraphicsManager::initialize();
	GraphicsManager::instance()->setup();
	DebugValue(GraphicsManager::instance()->val);
	GraphicsManager::instance()->val = 100;
	DebugValue(GraphicsManager::instance()->val);
	GraphicsManager::destory();
	getchar();
}