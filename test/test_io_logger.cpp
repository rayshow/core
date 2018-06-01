#include<ccdk/io/logger.h>

using namespace ccdk;
using namespace ccdk::io;


int main() {
	
	ccdk_open_leak_check();

	logger::initialize();
	logger::instance()->open("io.log");
	CCDK_INFO("hello,world");
	CCDK_WARNING("this is a warning");
	CCDK_ERROR("a error occur");
	logger::destory();


	getchar();

	return 0;
}