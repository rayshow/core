#include<ccdk/io/filesystem/file_serialize.h>
#include<ccdk/text/string.h>

using namespace ccdk;
using namespace ccdk::io;
using namespace ccdk::txt;
using namespace ccdk::ct;


struct person {
	string name;
	float height;
	int age;
	float weight;
	
	writer& write_to(writer& w) const {
		w << height << age << weight << name;
		return w;
	}

	reader& read_from(reader& r) {
		r >> height >> age >> weight >> name;
		return r;
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
		DebugValue(is_serializible_v<person>);
		DebugValue(is_serializible_v<string>);
		DebugValue(is_deserializible_v<person>);
		DebugValue(is_deserializible_v<string>);

		binary_file file{ "person.bin", binary_file::eWriteReplace };
		file_writer w{ file };
		person p1{ "xiongya", 175.5f, 27, 60.67f };
		w << p1;
		w.close();
		
		binary_file file2{ "person.bin", binary_file::eRead };
		file_reader r{ file2 };
		person p2{};
		r >> p2;
		p2.debug_value();
		r.close();
	}
	DebugNewTitle("vector read/save");
	{
		DebugValue(is_serializible_v<vector<int>>);
		DebugValue(is_serializible_v<vector<string>>);
		DebugValue(is_deserializible_v<vector<int>>);
		DebugValue(is_deserializible_v<vector<string>>);

		DebugSubTitle("vector write");
		{
			binary_file file{ "vector.bin", binary_file::eWriteReplace };
			file_writer w{ file };
			vector<int>    ints{ { 45,12,998,3,1,5 } };
			vector<string> names{ { "lily","lucy", "tom", "kite" } };
			w << ints << names;
			w.close();
		}
		DebugSubTitle("vector read");
		{
			binary_file file2{ "vector.bin", binary_file::eRead };
			file_reader r{ file2 };
			vector<int>    ints2{};
			vector<string> names2{};
			r >> ints2;
			ints2.debug_value("read ints:");
			r >> names2;
			DebugValueItBegin("names:");
			for (auto& name : names2) {
				DebugValueIt(name.c_str());
			}
			DebugValueItEnd();
			r.close();
		}
	}
	
	ccdk_open_leak_check();
	getchar();
	return 0;
}