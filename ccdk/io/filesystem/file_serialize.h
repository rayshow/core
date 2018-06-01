#pragma once

#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/base/logic_.h>
#include<ccdk/io/filesystem/file.h>
#include<ccdk/io/io_predef.h>

ccdk_namespace_io_start

using namespace ccdk::mpl;

class file_writer : public writer {

private:
	binary_file& file;
public:
	file_writer(binary_file& file) noexcept : file{ file } { ccdk_assert(file.is_opened()); }

	virtual ptr::size_t write(const void* data, ptr::size_t size) override {
		return file.write(data, size);
	}

	virtual void rewind(rewind_mode mode = rewind_mode::current, uint32 offset = 0) {
		file.rewind(offset, (int)mode);
	}

	virtual void close() { file.close(); };
	virtual bool is_opened() { return file.is_opened(); }
	virtual bool reached_end() { return file.reached_end(); }
};

class file_reader : public reader {
private:
	binary_file& file;
public:
	file_reader(binary_file& file) noexcept : file{ file } {}

	virtual ptr::size_t read(void* data, ptr::size_t size) override {
		return file.read(data, size);
	}

	virtual void rewind(rewind_mode mode = rewind_mode::current, uint32 offset = 0) override {
		file.rewind(offset, (int)mode);
	}

	virtual void close() override { file.close(); };
	virtual bool is_opened() override { return file.is_opened(); }
	virtual bool reached_end() override { return file.reached_end(); }
};


ccdk_namespace_io_end