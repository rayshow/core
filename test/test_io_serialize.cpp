#include<ccdk/io/filesystem/file_serialize.h>
#include<ccdk/text/string.h>


using namespace ccdk::io;
using namespace ccdk::txt;
using namespace ccdk;

struct person {
	string name;
	float height;
	int age;
	float weight;
	
	bool write_to(writer& w) const {
		w << height << age << weight << name;
		return true;
	}

	bool read_from(reader& r) {
		r >> height >> age >> weight >> name;
		return true;
	}

	void debug_value() {
		printf("person{name:%s, height:%f, age: %d, weight: %f} \n", name.c_str(), height, age, weight);
	}

};

int main() {
	
	DebugNewTitle("save/load base type");
	{
		binary_file file{ "test.bin", binary_file::eWriteReplace };
		file_writer writer{ file };
		writer << 1 << 'A' << 3.4566f << 89084392;
		writer.close();
		
		binary_file file2{ "test.bin", binary_file::eRead };
		file_reader reader{ file2 };
		ccdk_assert(reader.is_opened());
		int first;
		char ch;
		float ft;
		int last;
		reader >> first >> ch >> ft >> last;
	}
	DebugNewTitle("struct data save/load");
	{ 
		DebugValue(is_serialize_readable_v<person, reader&>);
		DebugValue(is_serialize_readable_v<string, reader&>);
		DebugValue(is_serialize_writable_v<person, writer&>);
		DebugValue(is_serialize_writable_v<string, writer&>);

		binary_file file{ "test2.bin", binary_file::eWriteReplace };
		file_writer w{ file };
		person p1{ "xiongya", 175.5f, 27, 60.67f };
		w << p1;
		w.close();
		
		binary_file file2{ "test2.bin", binary_file::eRead };
		file_reader r{ file2 };
		person p2{};
		r >> p2;
		p2.debug_value();
		r.close();
	}
	
	ccdk_open_leak_check();
	getchar();
	return 0;
}