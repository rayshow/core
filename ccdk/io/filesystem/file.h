#pragma once

#include<ccdk/text/string.h>
#include<ccdk/io/io_module.h>

ccdk_namespace_io_start

using namespace ccdk;

class binary_file {
public:
	enum mode{ 
		eRead,
		eWriteAppend,
		eWriteReplace,
	};
private:
	FILE        *handle;
	txt::string  filename;
public:
	//de-ctor
	~binary_file() { close(); }

	//ctor
	binary_file() :handle{ nullptr } {}

	//open with filename
	binary_file(txt::string const& filename, mode md = eRead){
		open(filename, md);
	}

	bool is_opened() { return handle != nullptr; }

	bool open(txt::string const& filename, mode m = eRead) {
		if (is_opened()) {
			close();
		}
		static const char* kModeString[3] = {
			"r", "a", "w"
		};
		handle = fopen(filename.c_str(), kModeString[(int)m]);
		return handle != nullptr;
	}

	ptr::size_t write(const void* buffer, ptr::size_t size) {
		ccdk_assert(handle);
		return fwrite(buffer, 1, size, handle);
	}

	ptr::size_t read(void* buffer, ptr::size_t size) {
		ccdk_assert(handle);
		return fread(buffer, 1, size, handle);
	}

	void rewind(int32 offset, int32 pos) { 
		ccdk_assert(handle && pos<3 && pos>=0); 
		constexpr static int32 map[] = { SEEK_SET, SEEK_CUR, SEEK_END };
		fseek(handle, offset, map[pos]);
	}

	void flush() { fflush(handle);}

	void close() { if (handle) { fclose(handle);} }

	bool reached_end() { return feof(handle); }
};

ccdk_namespace_io_end
