#include<ccdk/io/filesystem/file_serialize.h>
#include<ccdk/text/string.h>


using namespace ccdk::io;
using namespace ccdk::txt;
using namespace ccdk;

int main() {
	
	binary_file file{ "test.bin", binary_file::eWriteReplace };

	file_writer writer{ file };
	writer << 1 << 'A' <<3.4566f<<89084392;
	writer.close();

	file_reader reader{ "test.bin", binary_file::eRead };
	ccdk_assert(reader.is_opened());
	int first;
	char ch;
	float ft;
	int last;
	reader >> first >> ch >> ft >> last;
	
	ccdk_open_leak_check();
	getchar();
	return 0;
}